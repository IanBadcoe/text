#pragma once

#include "Entity.h"
#include "Stepable.h"
#include "Path.h"

class Actor : public Entity, public Stepable {
public:
	Actor(std::istringstream& in);
	Actor(EntityType et, World* w, const Coord& pos);

	enum class State {
		Idle,
		FollowingPath,
		SingleStep
	};

	// Inherited via ISerialisable
	virtual void SerialiseTo(std::ostringstream& out) const;

	// from Stepable (derived classes call this to step our state machine...)
	virtual float InnerStep() = 0;

	// used by NPCs to request a new command
	virtual void BecomeIdle() = 0;

protected:
	State _state;

	Path _current_path;
	Coord _dest;
private:
	bool CanMoveTo(Coord dest);
	void MoveTo(Coord dest);

	bool _is_idle;
};
