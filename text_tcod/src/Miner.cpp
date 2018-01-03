#include "precompiled.h"

#include "Miner.h"

static Entity* CreateMiner(std::istringstream& in, World* w) {
	return new Miner(in, w);
}

EntityCreator Miner::s_creator(EntityType::Miner, CreateMiner);

Miner::Miner(std::istringstream& in, World* w) : PlayerOwned(in, w) {
}

float Miner::InnerStep() {
	return 0.0f;
}

void Miner::BecomeIdle() {
}

void Miner::SerialiseTo(std::ostringstream& out) const {
	PlayerOwned::SerialiseTo(out);
}
