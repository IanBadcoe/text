#pragma once

#include "Entity.h"

#include "ISerialisable.h"

#include <queue>

class StepableQueue;

class Stepable {
public:
    Stepable(std::istringstream& in);
	Stepable(float speed) : _speed(speed) {}
	~Stepable();

	void Step(StepableQueue* queue, float time);
	// returns time of next step or zero for none
	virtual float InnerStep() = 0;

	void SerialiseTo(std::ostringstream& out) const;

	void SetQueue(StepableQueue* queue) const {
		assert(_in_queue == nullptr || queue == nullptr || _in_queue == queue);

		_in_queue = queue;
	}

private:
	float _speed;
	mutable StepableQueue* _in_queue;
};

class StepableQueue {
public:
	StepableQueue() : _last_frame(0) {}
	// returns false at the end of a frame, e.g. when time passes a whole number
	bool Step();

	bool AnythingToStep() { return _queue.size() > 0; }

	void AddFutureStep(Stepable* s, float t) {
        s->SetQueue(this);
		_queue.push(QueueEntry(s, t));
	}

	void SerialiseTo(std::ostringstream& out) const;
	void SerialiseFrom(std::istringstream& in, const World* world);

	void Remove(const Stepable* s) {
		std::priority_queue<QueueEntry> temp;

		while (_queue.size())
		{
			QueueEntry qe = _queue.top();

			if (qe._s != s)
			{
				temp.push(qe);
			}

			_queue = temp;
		}

        s->SetQueue(nullptr);
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
    Actor(std::istringstream& in) : Entity(in), Stepable(in) {}
	Actor(EntityType et, float speed) :
		Entity(et), Stepable(speed) {
	}

	// Inherited via ISerialisable
	virtual void SerialiseTo(std::ostringstream& out) const;
};
