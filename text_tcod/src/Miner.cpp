#include "precompiled.h"

#include "Miner.h"

static Entity* CreateMiner(std::istringstream& in, World* w) {
	return new Miner(in);
}

EntityCreator Miner::s_creator(EntityType::Miner, CreateMiner);

Miner::Miner(std::istringstream & in) : PlayerOwned(in) {
}

float Miner::InnerStep() {
	return 0.0f;
}

void Miner::BecomeIdle() {
}
