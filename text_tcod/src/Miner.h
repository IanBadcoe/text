#pragma once

#include "PlayerOwned.h"

class Miner : public PlayerOwned
{
public:
	Miner(std::istringstream& in, World* w);
	Miner(World* w, const Coord& pos, Player* p) :
		PlayerOwned(EntityType::Miner, SerialiseOrder::ActorAfterPlayer, w, pos, p) {}

	// Inherited via Actor
	virtual float InnerStep() override;
	virtual void BecomeIdle() override;

private:
	static EntityCreator s_creator;

	// Inherited via PlayerOwned
	virtual void SerialiseTo(std::ostringstream& out) const override;
};
