#pragma once

#include "Entity.h"

#include <queue>

class StepableQueue;

class Stepable {
public:
	Stepable(float speed) : _speed(speed) {}

	void Step(StepableQueue* queue, float time);
	// returns time of next step or zero for none
	virtual float InnerStep() = 0;

private:
	float _speed;
};

class StepableQueue {
public:
	StepableQueue() : _last_frame(0) {}
	// returns false at the end of a frame, e.g. when time passes a whole number
	bool Step();

	bool AnythingToStep() { return _queue.size() > 0; }

	void AddFutureStep(Stepable* s, float t) {
		_queue.push(QueueEntry(s, t));
	}

private:
	struct QueueEntry {
		QueueEntry(Stepable* a, float time) : _s(a), _time(time) {}

		Stepable* _s;
		float _time;

		bool operator<(const QueueEntry& rhs) const {
			// earliest first
			return _time > rhs._time;
		}
	};

	std::priority_queue<QueueEntry> _queue;

	int _last_frame;
};

class Actor : public Entity, public Stepable {
public:
	Actor(EntityType et, float speed) :
		Entity(et), Stepable(speed) {
	}
};

