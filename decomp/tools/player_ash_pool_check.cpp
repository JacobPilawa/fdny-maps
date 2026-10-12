#include "../src/player_ash_pool.h"
#include <cassert>

int main() {
    PlayerAshPool pool;
    int descriptors=0, constructions=0, presents=0, spawns=0, resets=0;
    Random rng(1);
    PlayerAshPoolServices services;
    services.addDescriptor=[&](size_t c,uint32_t flags,size_t,
                               const PlayerAshDescriptor& d,Random&) {
        assert(c<2 && flags==PlayerAshPool::descriptorFlagsDuringAdd && d.id==200+c);
        ++descriptors;
    };
    services.constructOpaqueParticle=[&](size_t i,size_t c,PlayerAshParticle& p,Random&) {
        assert(c==(i&1) && p.active && p.animationEnabled && p.activationScale==1.0f);
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
    services.resetAnimation=[&](PlayerAshParticle&,Random&) { ++resets; };
    pool.initialize(services,rng);
    pool.initialize(services,rng);
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
    assert(spawns==50 && resets==50);
    for(size_t i=0;i<pool.particles.size();i++) assert(pool.particles[i].active==(i<50));
    for(size_t i=0;i<50;i++) {
        assert(pool.particles[i].deadline==0);
        assert(pool.particles[i].opacityScale>=0.5f && pool.particles[i].opacityScale<=1.5f);
        assert(pool.particles[i].spawnPosition.has_value());
        assert(pool.particles[i].vector7c[0]==0.0f && pool.particles[i].vector7c[2]==0.0f);
    }

    pool.setDensity(2.0f); assert(pool.density==1.0f);
    pool.setDensity(-1.0f); assert(pool.density==0.0f);
}
