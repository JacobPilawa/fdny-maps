#include <cassert>
#include <cmath>
#include <cstdint>
#include <cstring>
#include <stdexcept>
#include <string>
#include <utility>
#include <vector>

struct V{float x=0,y=0,z=0;float& operator[](int i){return i==0?x:i==1?y:z;}float operator[](int i)const{return i==0?x:i==1?y:z;}};
#include "../src/extended.h"
#include "../src/victim_exit.h"
#include "../src/player_herodown_tail.h"

struct Ops {
    std::vector<std::string> calls;
    PlayerHeroDownSpawn staged{};
    uint32_t statistic=0;
    int cleanupMode=3;
    RescueExitState* shared=nullptr;
    uint32_t countSeenAtSpawn=0,countSeenAtStatistic=0;
    int boundsReports=0;
    int cleanupState=0,cleanupCollectionCount=0;
    bool mutateCleanupOnSpawn=false;
    void spawnHeroDown(const PlayerHeroDownSpawn& s){calls.push_back("spawn");staged=s;countSeenAtSpawn=shared->total;if(mutateCleanupOnSpawn){cleanupState=4;cleanupCollectionCount=4;}}
    void setRescueStatisticTotal(uint32_t n){calls.push_back("stat");statistic=n;countSeenAtStatistic=shared->total;assert(n==shared->total);}
    void logFirefightersDown(int n){calls.push_back("log");assert(n>=0);}
    int readCleanupState(){calls.push_back("read-cleanup-state");return cleanupState;}
    int readCleanupCollectionCount(){calls.push_back("read-cleanup-count");return cleanupCollectionCount;}
    void reportCleanupArrayBounds(){calls.push_back("bounds-diagnostic");boundsReports++;}
    int cleanupSubobjectMode(){calls.push_back("cleanup-mode");return cleanupMode;}
    void transitionCleanupSubobjectTo2(){calls.push_back("transition-2");}
    void refreshSelection(){calls.push_back("refresh-selection");}
    void resetPassoutPlayerView(){calls.push_back("reset-view");}
    void finishPlayerRescueStatus(){calls.push_back("finish-status");}
};

int main(){
    VictimPassoutPlayerState player;player.position={1.25f,2.5f,3.75f};player.passoutCount=2;
    RescueExitState rescue;rescue.total=2;Ops ops;ops.shared=&rescue;
    std::vector<PlayerHeroDownEntity> entities={
      {2,"HeroDown",4},{2,"herodown",7},{2,"HERODOWN",3},
      {1,"herodown",4},{2,"other",4}};
    ops.mutateCleanupOnSpawn=true;
    auto result=continuePlayerHeroDownTail(player,rescue,2,0,entities,ops);
    assert(result.countBranchEntered&&result.spawned&&result.firefightersDown==3);
    assert(rescue.total==3&&ops.statistic==3);
    assert(ops.countSeenAtSpawn==2&&ops.countSeenAtStatistic==3);
    assert(ops.staged.position==player.position);
    assert(ops.staged.word0==1000&&std::string(ops.staged.argumentFormat)=="%i");
    assert(ops.staged.countMinusOneBits==1&&ops.staged.countMinusOne==1);
    assert(ops.staged.formattedSuffix=="1"&&ops.staged.generatedName=="POFighter1");
    assert(std::string(ops.staged.fighterName)=="POFighter");
    assert(std::string(ops.staged.scriptName)=="herodown"&&ops.staged.word6==0);
    assert(ops.staged.word7==20.0f&&std::string(ops.staged.entityClass)=="RCS_VictimRescue");
    assert(ops.staged.classLookupArgument==100);
    assert(ops.boundsReports==1);
    assert((ops.calls==std::vector<std::string>{"spawn","stat","log","read-cleanup-state","read-cleanup-state","read-cleanup-count","bounds-diagnostic","cleanup-mode","transition-2","refresh-selection","reset-view","finish-status"}));

    RescueExitState atLimit;atLimit.total=10;Ops noSpawn;noSpawn.shared=&atLimit;
    std::vector<PlayerHeroDownEntity> three={{2,"herodown",4},{2,"herodown",7},{2,"herodown",4}};
    noSpawn.cleanupState=1;
    auto limit=continuePlayerHeroDownTail(player,atLimit,2,0,three,noSpawn);
    assert(limit.countBranchEntered&&!limit.spawned&&limit.firefightersDown==3);
    assert(atLimit.total==10&&noSpawn.calls[0]=="log");
    assert((noSpawn.calls==std::vector<std::string>{"log","read-cleanup-state","read-cleanup-state","refresh-selection","reset-view","finish-status"}));

    RescueExitState gated;Ops gateOps;gateOps.shared=&gated;
    auto skippedMode=continuePlayerHeroDownTail(player,gated,1,0,{},gateOps);
    assert(!skippedMode.countBranchEntered&&skippedMode.firefightersDown==-1);
    assert((gateOps.calls==std::vector<std::string>{"read-cleanup-state","reset-view","finish-status"}));
    gateOps.calls.clear();
    auto skippedArgument=continuePlayerHeroDownTail(player,gated,2,1,{},gateOps);
    assert(!skippedArgument.countBranchEntered&&gateOps.calls.size()==3);

    Ops cleanupOnly;cleanupOnly.cleanupMode=2;
    cleanupOnly.cleanupState=4;cleanupOnly.cleanupCollectionCount=5;
    continuePlayerHeroDownTail(player,gated,0,0,{},cleanupOnly);
    assert((cleanupOnly.calls==std::vector<std::string>{"read-cleanup-state","read-cleanup-state","read-cleanup-count","cleanup-mode","refresh-selection","reset-view","finish-status"}));

    RescueExitState wrap;wrap.total=0xffffffffu;Ops wrapOps;wrapOps.shared=&wrap;
    player.passoutCount=0;
    auto wrapped=continuePlayerHeroDownTail(player,wrap,2,0,{{2,"herodown",4},{2,"herodown",7}},wrapOps);
    assert(wrapped.spawned&&wrap.total==0&&wrapOps.statistic==0);
    assert(wrapOps.countSeenAtSpawn==0xffffffffu&&wrapOps.countSeenAtStatistic==0);
    assert(wrapOps.staged.countMinusOneBits==0xffffffffu&&wrapOps.staged.countMinusOne==-1);
    assert(wrapOps.staged.formattedSuffix=="-1"&&wrapOps.staged.generatedName=="POFighter-1");
    assert((wrapOps.calls==std::vector<std::string>{"spawn","stat","log","read-cleanup-state","reset-view","finish-status"}));

    Ops shortCollection;shortCollection.cleanupMode=3;
    shortCollection.cleanupState=4;shortCollection.cleanupCollectionCount=4;
    continuePlayerHeroDownTail(player,gated,0,1,{},shortCollection);
    assert(shortCollection.boundsReports==1);
    assert((shortCollection.calls==std::vector<std::string>{"read-cleanup-state","read-cleanup-state","read-cleanup-count","bounds-diagnostic","cleanup-mode","transition-2","refresh-selection","reset-view","finish-status"}));
}
