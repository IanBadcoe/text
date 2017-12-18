#pragma once

#include "Entity.h"

#include "DisplayChar.h"

#include "color.hpp"

class ShadedVoid : public Terrain {
public:
	ShadedVoid(std::istringstream& in);
	ShadedVoid(int depth) : Terrain(EntityType::Floor, false, true), _depth(depth)
	{
		assert(_depth >= 0 && _depth < 4);
	}

	// Inherited via Terrain
	virtual void SerialiseTo(std::ostringstream& out) const override;

private:
	int _depth;

	DisplayChar _disp;
	static TCODColor s_foreground[4];
	static TCODColor s_background[4];

    static EntityCreator s_creator;

	// Inherited via Terrain
	virtual void CalcDisp(uint8_t code) override;

	// Inherited via Terrain
	virtual bool DrawCompatWith(const Terrain * other) const override;
};
