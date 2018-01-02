#pragma once

#include "PlayerOwned.h"

class Miner : public PlayerOwned
{
public:
	Miner(std::istringstream& in);
	Miner(World* w, const Coord& pos) : PlayerOwned(EntityType::Miner, w, pos) {}

	// Inherited via Actor
	virtual float InnerStep() override;
	virtual void BecomeIdle() override;

private:
	static EntityCreator s_creator;
};
