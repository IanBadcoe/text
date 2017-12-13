#pragma once

#include "Entity.h"

#include "DisplayChar.h"

#include <sstream>

class DisplayChar;

class Wall : public Terrain {
public:
    Wall(std::istringstream& in);
	Wall(int str) : Terrain(EntityType::Wall, false, false), _str(str) {}

	virtual DisplayChar Disp() const {
		if (_str > 500) {
			return s_strong_wall;
		}
		else
		{
			return s_weak_wall;
		}
	}

	int GetStr() const { return _str; }

	// Inherited via Terrain
	virtual void SerialiseTo(std::ostringstream& out) const override;

private:
	int _str;

	static DisplayChar s_strong_wall;
	static DisplayChar s_weak_wall;

    static EntityCreator s_creator;
};
