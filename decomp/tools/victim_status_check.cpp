#include <cstdint>
#include <limits>
#include <string>
#include <cassert>
#include <string>
#include <vector>

#include <cmath>
#include <cstring>
#include <stdexcept>
#include <utility>
struct V{float x=0,y=0,z=0;float& operator[](int i){return i==0?x:i==1?y:z;}float operator[](int i)const{return i==0?x:i==1?y:z;}};
#include "../src/extended.h"
#include "../src/victim_exit.h"
#include "../src/victim_status.h"

struct Ops {
    std::vector<std::string> calls;
    uint32_t clock = 10, counted = 0;
    uint32_t statTotal = 0;
    uint32_t sample = 77;
    std::string shown;
    float duration = 0;
    uint32_t formattedCount = 0;
    void setVictPickStatusLayer(bool v){assert(v);calls.push_back("pick-layer-on");clock=20;}
    uint32_t readClock(){calls.push_back("clock");return clock;}
    uint32_t sampleVictPickList(float t){assert(t==0);calls.push_back("sample-list");return sample;}
    void updateVictPickStatus(uint32_t s){assert(s==sample);calls.push_back("update-pick");clock=30;}
    void setStatusKey(const char* key,bool enabled){assert(enabled);calls.push_back(std::string("key:")+key);}
    void hideVictDropDetail(){calls.push_back("hide-drop-detail");clock=40;}
    void setVictDropStatusLayer(bool v){assert(!v);calls.push_back("drop-layer-off");clock=50;}
    void prepareVictDropMessage(){calls.push_back("prepare-drop-message");}
    uint32_t countType2Entities(){calls.push_back("count-type2");return counted;}
    void setRescueStatisticTotal(uint32_t n){calls.push_back("stat-total");statTotal=n;}
    void refreshMembership(){calls.push_back("refresh-membership");}
    void showVictDropMessageOne(const char* fmt,float seconds){
        calls.push_back("show-message");duration=seconds;shown=fmt;
    }
    void showVictDropMessageMany(const char* fmt,float seconds,uint32_t n){
        calls.push_back("show-message");duration=seconds;shown=fmt;formattedCount=n;
    }
};

int main(){
    RescueExitState rescue;TrainingVictimStatusState status{rescue};Ops pick;
    assert(trainingVictimStatusAction(status,1,pick)==TrainingVictimStatusResult::handled);
    assert((pick.calls==std::vector<std::string>{"pick-layer-on","clock","sample-list","update-pick","key:RC_VictPick"}));
    assert(status.victimPickTimestamp==20); // clock sampled after layer callback, before list/update callbacks

    RescueExitState lazy;lazy.field3530=1;lazy.rescues=2;
    TrainingVictimStatusState drop{lazy};Ops many;many.counted=9;
    assert(trainingVictimStatusAction(drop,2,many)==TrainingVictimStatusResult::handled);
    assert((many.calls==std::vector<std::string>{"hide-drop-detail","drop-layer-off","clock","prepare-drop-message","count-type2","stat-total","refresh-membership","show-message","key:RC_VictDrop"}));
    assert(drop.victimDropTimestamp==50); // sampled after both preceding UI callbacks
    assert(lazy.total==9 && many.statTotal==9 && many.formattedCount==6 && many.duration==5.0f);

    RescueExitState one;one.total=4;one.field3530=1;one.rescues=2;
    TrainingVictimStatusState oneStatus{one};Ops single;
    assert(trainingVictimStatusAction(oneStatus,2,single)==TrainingVictimStatusResult::handled);
    assert(single.shown=="There's still one survivor remaining!" && single.duration==5.0f);
    assert(single.calls[single.calls.size()-2]=="show-message" && single.calls.back()=="key:RC_VictDrop");

    RescueExitState none;none.total=3;none.field3530=1;none.rescues=2;
    TrainingVictimStatusState noneStatus{none};Ops zero;
    assert(trainingVictimStatusAction(noneStatus,2,zero)==TrainingVictimStatusResult::noPositiveRemainder);
    assert(zero.calls.back()=="prepare-drop-message"); // no message/key at zero
    RescueExitState negative;negative.total=1;negative.field3530=2;
    TrainingVictimStatusState negativeStatus{negative};Ops neg;
    assert(trainingVictimStatusAction(negativeStatus,2,neg)==TrainingVictimStatusResult::noPositiveRemainder);
    assert(neg.calls.back()=="prepare-drop-message"); // wrapped high-bit result is not signed-positive

    RescueExitState unchanged;TrainingVictimStatusState ignored{unchanged};Ops ignoredOps;
    assert(trainingVictimStatusAction(ignored,7,ignoredOps)==TrainingVictimStatusResult::ignoredAction);
    assert(ignoredOps.calls.empty());
}
