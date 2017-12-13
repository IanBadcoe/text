#pragma once

#include "Entity.h"

#include "ISerialisable.h"

#include <queue>

class StepableQueue;

class Stepable {
public:
	Stepable(float speed) : _speed(speed) {}
	~Stepable();

	void Step(StepableQueue* queue, float time);
	// returns time of next step or zero for none
	virtual float InnerStep() = 0;

	void SerialiseStepableTo(std::ostringstream& out) const;
	void SerialiseStepableFrom(std::istringstream& in);

	void SetQueue(StepableQueue* queue) {
		assert(_queue == nullptr || _queue == queue);

		_queue = queue;
	}

private:
	float _speed;
	StepableQueue* _queue;
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

	// Inherited via ISerialisable
	virtual void SerialiseTo(std::ostringstream& out) const = 0;
};
