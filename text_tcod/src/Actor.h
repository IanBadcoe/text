#pragma once

#include "Entity.h"
#include "Stepable.h"
#include "Path.h"

class Actor : public Entity, public Stepable {
public:
	Actor(std::istringstream& in);
	Actor(EntityType et, float speed) :
		Entity(et), Stepable(speed) {
	}

	enum class State {
		Idle,
		FollowingPath,
		SingleStep
	};

	// Inherited via ISerialisable
	virtual void SerialiseTo(std::ostringstream& out) const;

	// from Stepable (derived classes call this to step our state machine...)
	virtual float InnerStep() = 0;

protected:
	State _state;

	Path _current_path;
	Coord _dest;
private:
	bool CanMoveTo(Coord dest);
	void MoveTo(Coord dest);
};
