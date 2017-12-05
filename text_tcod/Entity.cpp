#include "Entity.h"

#include "World.h"

void Actor::Step()
{
	float time = InnerStep();

	if (_speed != 0.0)
	{
		GetWorld()->QueueStep(this, time / _speed);
	}
}
