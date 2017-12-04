#pragma once

#include "Entity.h"

#include "color.hpp"

#include "assert.h"

class Player : public Entity {
public:
	Player(int num) : Entity(EntityType::Player), _num(num) {
		assert(_num >= 0 && _num < 4);
	}

	virtual DisplayChar& Disp() const;

	int GetNum() const { return _num; }

private:
	int _num;

	static DisplayChar s_player[4];
	static TCODColor s_foreground[4];
};
