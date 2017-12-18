#pragma once

#include "Terrain.h"

#include <sstream>

class DisplayChar;

class Rock : public Terrain {
public:
	enum class Type {
		Hard,
		Soft,
		Earth,
		RedOre,
		GreenOre,
		BlueOre,

		NumRockTypes
	};

	Rock(std::istringstream& in);
	Rock(Type t) : Terrain(EntityType::Rock, false, false), _t(t) {}

	// Inherited via Terrain
	virtual void SerialiseTo(std::ostringstream& out) const override;

	// Inherited via Terrain
	virtual void CalcDisp(const Terrain* surrounds[8]) override;
	virtual bool DrawCompatWith(const Terrain* other) const override;

private:
	Type _t;

	static TCODColor s_foregrounds[(int)Type::NumRockTypes];

    static EntityCreator s_creator;
};
