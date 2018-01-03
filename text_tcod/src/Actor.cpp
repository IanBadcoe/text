#include "precompiled.h"

#include "Actor.h"

#include "Terrain.h"
#include "World.h"

Actor::Actor(std::istringstream & in) :
	Entity(in),
	Stepable(in),
	_current_path(in),
	_state(State::Idle)
{
	in >>= _state;
	in >>= _dest;
}

Actor::Actor(EntityType et, SerialiseOrder so, World * w, const Coord & pos) :
	Entity(et, so),
	Stepable(w->GetUniverse()),
	_is_idle(false)
{
	w->AddActor(pos, this);
}

void Actor::SerialiseTo(std::ostringstream& out) const
{
    Entity::SerialiseTo(out);
    Stepable::SerialiseTo(out);
	_current_path.SerialiseTo(out);

	out <<= _state;
	out <<= _dest;
}

float Actor::InnerStep() {
	if (_state == State::Idle && !_is_idle)
	{
		// when we get a first idle tick
		BecomeIdle();
	}

	_is_idle = _state == State::Idle;

	switch (_state) {
	case State::Idle:
	{
		return 1.0f;
	}

	case State::FollowingPath:
	{
		if (_current_path._step == _current_path._path.size()) {
			_state = State::Idle;
			return 1.0f;
		}

		_dest = _current_path._path[_current_path._step];
		_current_path._step++;

		if (CanMoveTo(_dest)) {
			MoveTo(_dest);
			return 3.0f;
		} else {
			// path now blocked, bail out
			// for AI, eventually, could attempt to adjust here
			_state = State::Idle;
			return 1.0f;
		}
	}

	case State::SingleStep:
	{
		if (CanMoveTo(_dest)) {
			MoveTo(_dest);
		}

		_state = State::Idle;
		return 3.0f;
	}
	}

	assert(false);

	return 1.0f;
}

bool Actor::CanMoveTo(Coord dest) {
	World* w = GetWorld();
	const Terrain* t = w->GetTerrain(dest);

	return w->InRange(dest) && !w->GetActor(dest) && t && t->IsWalkable();
}

void Actor::MoveTo(Coord dest) {
	World* w = GetWorld();

	w->RemoveActor(GetPos());
	w->AddActor(dest, this);
}
