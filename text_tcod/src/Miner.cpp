#include "precompiled.h"

#include "Miner.h"
#include "Player.h"

static Entity* CreateMiner(std::istringstream& in, World* w) {
	return new Miner(in, w);
}

EntityCreator Miner::s_creator(EntityType::Miner, CreateMiner);

Miner::Miner(std::istringstream& in, World* w) : PlayerOwned(in, w) {
}

Miner::Miner(World * w, const Coord & pos, Player * p) :
	PlayerOwned(EntityType::Miner, SerialiseOrder::ActorAfterPlayer, w, pos, p) {
	SetDisplayChar(DisplayChar('m', p->GetColour()));
}

float Miner::InnerStep() {
    // do we even need this function?
    return Actor::InnerStep();
}

void Miner::BecomeIdle() {
}

void Miner::SerialiseTo(std::ostringstream& out) const {
	PlayerOwned::SerialiseTo(out);
}
