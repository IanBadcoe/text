#pragma once

#include "Terrain.h"

class DisplayChar;

class Wall : public Terrain {
public:
    Wall(std::istringstream& in);
	Wall(int str) : Terrain(EntityType::Wall, SerialiseOrder::TerrainDontCare, false, false), _str(str) {}

	int GetStr() const { return _str; }

	// Inherited via Terrain
	virtual void SerialiseTo(std::ostringstream& out) const override;

	// Inherited via Terrain
	virtual void CalcDisp(const Terrain* surrounds[8]) override;
	virtual bool DrawCompatWith(const Terrain* other) const override;

private:
	int _str;

	static DisplayChar s_strong_wall;
	static DisplayChar s_weak_wall;

    static EntityCreator s_creator;
};
