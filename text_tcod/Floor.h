#pragma once

#include "Entity.h"

class Floor : public Entity {
public:
	Floor() : Entity(EntityType::Floor) {}

	virtual DisplayChar& Disp() const {
		return s_floor;
	}
private:
	static DisplayChar s_floor;
};
