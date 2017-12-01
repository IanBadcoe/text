#pragma once

#include "Entity.h"

class DisplayChar;

class Wall : public Entity {
public:
	Wall(int str) : Entity(EntityType::Wall), _str(str) {}

	virtual DisplayChar* Disp() const {
		if (_str > 500) {
			return &s_strong_wall;
		}
		else
		{
			return &s_weak_wall;
		}
	}

	int GetStr() const { return _str; }

private:
	int _str;

	static DisplayChar s_strong_wall;
	static DisplayChar s_weak_wall;
};
