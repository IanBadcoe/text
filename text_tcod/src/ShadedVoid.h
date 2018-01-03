#pragma once

#include "Terrain.h"

class ShadedVoid : public Terrain {
public:
	ShadedVoid(std::istringstream& in);
	ShadedVoid(int depth) : Terrain(EntityType::ShadedVoid, SerialiseOrder::TerrainDontCare, false, true), _depth(depth)
	{
		assert(_depth >= 0 && _depth < 4);

		SetDisplayChar(DisplayChar('x', s_foreground[_depth]));
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
	virtual void CalcDisp(const Terrain* surrounds[8]) override;

	// Inherited via Terrain
	virtual bool DrawCompatWith(const Terrain* other) const override;
};
