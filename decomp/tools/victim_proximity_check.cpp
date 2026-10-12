#include <cassert>
#include <array>
#include <cmath>
#include <functional>
#include <optional>
#include <string>
#include <vector>

struct V { float x=0,y=0,z=0; };
struct Player {
    V upper{30,85,30};
    V maxs() const { return upper; }
};
#include "../src/victim_pickup.h"
#include "../src/victim_proximity.h"

struct Probe {
    std::vector<std::string> calls;
    Player player;
    EntityIdRegistry::Handle route;
    int routeReads=0;
    std::function<void()> onBox;
    V worldOrigin{};
    bool hasBox=true;
    V center{0,0,0};
    float radius=60;
    int fullReports=0;
    std::optional<VictimPickupBox> getBox(){calls.push_back("player-world-box");if(onBox)onBox();if(!hasBox)return {};return victimPickupBox(player,worldOrigin);}
    float getRadius(){calls.push_back("radius");return radius;}
    V getCenter(){calls.push_back("victim-center");return center;}
    EntityIdRegistry::Handle getRoute(){calls.push_back("recipient-route");routeReads++;return route;}
    void full(){calls.push_back("queue-full");fullReports++;}
};

static VictimProximityResult publish(Probe& p,int state,int type,
    const void* payload,FireMessageQueue& queue){
    return publishVictimProximity(state,type,payload,queue,
        [&]{return p.getBox();},[&]{return p.getRadius();},[&]{return p.getCenter();},
        [&]{return p.getRoute();},
        [&]{p.full();});
}

int main(){
    EntityIdRegistry ids;int playerObject=1;EntityIdRegistry::Handle route;
    assert(ids.registerObject(&playerObject,route));
    std::array<int,511> routeFillers{};
    for(auto& filler:routeFillers){EntityIdRegistry::Handle h;assert(ids.registerObject(&filler,h));}
    int replacementPlayer=2;EntityIdRegistry::Handle changedRoute;
    FireMessageQueue queue;int victimA=191,victimB=194;Probe p;p.route=route;

    assert(publish(p,5,1,&victimA,queue)==VictimProximityResult::rejectedVictimState);
    assert(p.calls.empty()&&p.routeReads==0);
    assert(publish(p,4,2,&victimA,queue)==VictimProximityResult::rejectedRecipientType);
    assert(p.calls.empty()&&p.routeReads==0); // rejects skip Player getter and route lookup

    p.hasBox=false;
    assert(publish(p,4,1,&victimA,queue)==VictimProximityResult::noPlayerWorldBox);
    assert((p.calls==std::vector<std::string>{"player-world-box"}));
    assert(p.routeReads==0);
    p.calls.clear();p.hasBox=true;

    // Inclusive source edge at Player X max 30 + authored radius 60.
    p.center={90,0,0};p.onBox=[&]{
        assert(ids.release(route));
        assert(ids.registerObject(&replacementPlayer,changedRoute));
        p.route=changedRoute;
    };
    assert(publish(p,4,1,&victimA,queue)==VictimProximityResult::queued);
    assert((p.calls==std::vector<std::string>{"player-world-box","radius","victim-center","recipient-route"}));
    assert(changedRoute.route==route.route&&changedRoute.generation!=route.generation);
    assert(queue.records[queue.pending.back().slot].route0==changedRoute.route);
    assert(queue.records[queue.pending.back().slot].route1==changedRoute.generation);
    route=changedRoute;p.onBox={};p.route=route;
    p.calls.clear();p.center.x=90.001f;
    assert(publish(p,4,1,&victimB,queue)==VictimProximityResult::outsidePickupBounds);
    assert((p.calls==std::vector<std::string>{"player-world-box","radius","victim-center"}));
    assert(p.routeReads==1); // only first overlapping case resolves the handle
    p.calls.clear();p.center={0,145,0}; // upper Y 85 plus radius 60 is also inclusive
    assert(publish(p,4,1,&victimB,queue)==VictimProximityResult::queued);
    p.calls.clear();p.center={0,0,90}; // Z axis source check
    assert(publish(p,4,1,&victimB,queue)==VictimProximityResult::queued);

    assert(queue.pending.size()==3);
    for(const auto& pending:queue.pending)assert(pending.flag==0);
    std::vector<const void*> delivered;std::vector<uint32_t> messages;
    queue.drain([&](uint32_t r,uint32_t g)->FireMessageQueue::Receiver{
        auto* target=ids.resolve(r,g);if(!target)return {};
        return [&](uint32_t m,const void* payload){messages.push_back(m);delivered.push_back(payload);};
    },[](const FireMessageQueue::Delivery&){});
    assert((messages==std::vector<uint32_t>{0x7532,0x7532,0x7532}));
    assert((delivered==std::vector<const void*>{&victimA,&victimB,&victimB}));

    // Same route recycled with a new generation while the old record is pending.
    FireMessageQueue staleQueue;Probe staleProbe;int oldObject=0;EntityIdRegistry::Handle oldHandle;
    EntityIdRegistry staleIds;assert(staleIds.registerObject(&oldObject,oldHandle));
    int pendingVictim=9;
    staleProbe.route=oldHandle;
    assert(publish(staleProbe,4,1,&pendingVictim,staleQueue)==VictimProximityResult::queued);
    const uint32_t staleRoute=oldHandle.route,staleGeneration=oldHandle.generation;
    assert(staleIds.release(oldHandle));
    int replacement=0;std::array<int,512> fillers{};EntityIdRegistry::Handle replacementHandle;
    for(int i=0;i<511;i++){EntityIdRegistry::Handle h;assert(staleIds.registerObject(&fillers[i],h));}
    assert(staleIds.registerObject(&replacement,replacementHandle));
    assert(replacementHandle.route==staleRoute&&replacementHandle.generation!=staleGeneration);
    int staleDeliveries=0;
    staleQueue.drain([&](uint32_t r,uint32_t g)->FireMessageQueue::Receiver{
        auto* target=staleIds.resolve(r,g);if(!target)return {};
        return [&](uint32_t,const void*){staleDeliveries++;};
    },[](const FireMessageQueue::Delivery&){});
    assert(staleDeliveries==0);

    // Real queue and registry implementations: fill all 64 mapping slots.
    FireMessageQueue fullQueue;int sink=0;
    for(int i=0;i<FireMessageQueue::capacity;i++)
        assert(fullQueue.enqueue(100,1,0x1234,&sink,0));
    Probe fullProbe;
    fullProbe.route=route;
    assert(publish(fullProbe,4,1,&victimA,fullQueue)==VictimProximityResult::queueFull);
    assert(fullProbe.fullReports==1&&fullQueue.pending.size()==FireMessageQueue::capacity);
}
