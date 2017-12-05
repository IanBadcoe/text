#include "Entity.h"

#include "World.h"

void Actor::Step(float time)
{
	float duration = InnerStep();

	if (_speed != 0.0)
	{
		GetWorld()->QueueStep(this, time + duration / _speed);
	}
}
