#pragma once

#include "Actor.h"

class Label : public Actor
{
public:
	Label(int i) : Actor(EntityType::Label, 0.0f) {
		SetDisplayChar(DisplayChar(i + 48, TCODColor(192, 192, 0)));
	}

	// Inherited via Actor
	virtual float InnerStep() override;
};

