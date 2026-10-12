#include "../src/player_ash_pool.h"
#include <cassert>

int main() {
    PlayerAshPool pool;
    int descriptors=0, constructions=0, presents=0, spawns=0;
    Random rng(1);
    PlayerAshPoolServices services;
    services.addDescriptor=[&](size_t c,uint32_t flags,size_t,
                               const PlayerAshDescriptor& d,Random&) {
        assert(c<2 && flags==PlayerAshPool::descriptorFlagsDuringAdd && d.id==200+c);
        ++descriptors;
    };
    services.constructOpaqueParticle=[&](size_t i,size_t c,PlayerAshParticle& p,Random&) {
        assert(c==(i&1) && p.active && p.animationEnabled && p.activationScale==1.0f);
        assert(p.animationFlags==PlayerAshPool::descriptorFlagsAfterSetup);
        assert(p.animationIndex==0 && p.nextAnimationIndex==1 && p.animationTimestamp==1);
        assert(p.positionInitialized && !p.vectorsDirty);
        assert(p.opacityScale==1.0f && p.deadline==0);
        assert((p.vector7c==std::array<float,3>{} && p.vector88==std::array<float,3>{}));
        assert(!p.spawnPosition.has_value());
        ++constructions;
    };
    services.present=[&](PlayerAshParticle&,Random&) { ++presents; };
    services.spawnPoint=[&](float angle,float distance,Random&) {
        ++spawns; return std::array<float,3>{angle,distance,0.0f};
    };
    pool.initialize(services,1,rng);
    pool.initialize(services,1,rng);
    assert(descriptors==10 && constructions==100 && pool.density==1.0f);
    assert(PlayerAshPool::descriptorFlagsDuringAdd==0x35);
    assert(PlayerAshPool::descriptorFlagsAfterSetup==0x3c);
    assert(pool.descriptorFlags[0]==0x3c && pool.descriptorFlags[1]==0x3c);
    assert(PlayerAshPool::descriptorFor(0,0).scalarBits[0]==0x3f000000);
    assert(PlayerAshPool::descriptorFor(1,4).id==201);

    pool.tick(0,rng,services);
    assert(rng.drawCount==400 && presents==100); // first due-update of 100 active objects
    for (const auto& p : pool.particles) {
        assert(p.deadline==PlayerAshPool::maximumLifetime);
        assert(p.vectorsDirty);
    }

    pool.setDensity(0.5f);
    for(auto& p:pool.particles) p.active=false;
    presents=0;
    pool.tick(1,rng,services);
    assert(rng.drawCount==650 && presents==100);
    assert(spawns==50);
    for(size_t i=0;i<pool.particles.size();i++) assert(pool.particles[i].active==(i<50));
    for(size_t i=0;i<50;i++) {
        assert(pool.particles[i].deadline==0);
        assert(pool.particles[i].opacityScale>=0.5f && pool.particles[i].opacityScale<=1.5f);
        assert(pool.particles[i].animationIndex==0 && pool.particles[i].nextAnimationIndex==1 && pool.particles[i].animationTimestamp==1);
        assert(pool.particles[i].spawnPosition.has_value());
        assert(pool.particles[i].vector7c[0]==0.0f && pool.particles[i].vector7c[2]==0.0f);
    }

    pool.setDensity(2.0f); assert(pool.density==1.0f);
    pool.setDensity(-1.0f); assert(pool.density==0.0f);

    PlayerAshParticle finite;
    finite.animationFlags=PlayerAshPool::descriptorFlagsAfterSetup;
    finite.nextAnimationIndex=1;
    PlayerAshPool::updateAnimation(finite,499);
    assert(finite.animationIndex==0 && finite.animationTimestamp==0 && finite.active);
    PlayerAshPool::updateAnimation(finite,500); // record 0: 0.5 s
    assert(finite.animationIndex==1 && finite.nextAnimationIndex==2 && finite.animationTimestamp==500 && finite.active);
    PlayerAshPool::updateAnimation(finite,1000); // record 1: 0.5 s
    assert(finite.animationIndex==2 && finite.animationTimestamp==1000 && finite.active);
    PlayerAshPool::updateAnimation(finite,1500); // record 2: 0.5 s
    assert(finite.animationIndex==3 && finite.animationTimestamp==1500 && finite.active);
    PlayerAshPool::updateAnimation(finite,2500); // record 3: 1.0 s
    assert(finite.animationIndex==4 && finite.animationTimestamp==2500 && finite.active);

    PlayerAshPool lifecycle;
    lifecycle.initialized=true;
    lifecycle.descriptorFlags={0x3c,0x3c};
    lifecycle.density=0.02f; // trunc(100 * 0.02f) selects one entry
    for (auto& p : lifecycle.particles) p.animationEnabled=false;
    auto& last=lifecycle.particles[0];
    last.active=true;last.animationEnabled=true;last.animationFlags=0x3c;
    last.animationIndex=4;last.nextAnimationIndex=4;last.animationTimestamp=2500;
    last.deadline=0xffffffffu;last.activationScale=1.0f;
    bool presentedAfterDeactivation=false;int respawns=0;
    PlayerAshPoolServices lifecycleServices;
    lifecycleServices.present=[&](PlayerAshParticle& p,Random&) {
        if (&p==&last) presentedAfterDeactivation=!p.active;
    };
    lifecycleServices.spawnPoint=[&](float angle,float distance,Random&) {
        ++respawns;return std::array<float,3>{angle,distance,0.0f};
    };
    lifecycle.tick(3500,rng,lifecycleServices); // record 4: 1.0 s; deactivation precedes scan
    assert(presentedAfterDeactivation && respawns==1 && last.active);
    assert(last.animationIndex==0 && last.nextAnimationIndex==1 && last.animationTimestamp==3500);
}
