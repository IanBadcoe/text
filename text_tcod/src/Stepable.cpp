#include "precompiled.h"

#include "Stepable.h"
#include "Serialization.h"
#include "Entity.h"
#include "InputHandler.h"
#include "Actor.h"
#include "Universe.h"
#include "World.h"


Stepable::Stepable(std::istringstream& in) {
	in >>= _speed;
}

Stepable::~Stepable() {
	if (_in_queue) {
		_in_queue->Remove(this);
	}
}

void Stepable::Step(StepableQueue* queue, float time) {
	float duration = InnerStep();

	if (_speed != 0.0f && duration != 0) {
		queue->AddAbsoluteStep(this, time + duration / _speed);
	}
}

void Stepable::SerialiseTo(std::ostringstream& out) const {
	out <<= _speed;
}

bool StepableQueue::Step() {
	assert(_queue.size());

	QueueEntry qe = _queue.top();
	assert(qe._time >= _last_time);
	_last_time = qe._time;

	float new_time = qe._time;

	int new_frame = (int)new_time;

	// if we have reached the frame the next step should be
	// executed in that step
	if (new_frame - _last_frame < 2) {
		_queue.pop();
		_contains.erase(qe._s);

		qe._s->SetQueue(nullptr);
		qe._s->Step(this, new_time);
	}

	// if we should be on a frame above the current one, step towards it
	if (new_frame > _last_frame) {
		_last_frame++;
		return false;
	}

	return true;
}

void StepableQueue::AddRelativeStep(Stepable* s, float t) {
	AddAbsoluteStep(s, _last_time + t);
}

void StepableQueue::AddAbsoluteStep(Stepable* s, float t) {
	assert(!Contains(s));
	assert(t >= _last_time);

	s->SetQueue(this);
	_queue.push(QueueEntry(s, t));
	_contains.insert(s);

	std::priority_queue<QueueEntry> temp(_queue);

	float tt = temp.top()._time;

	while (temp.size()) {
		assert(temp.top()._time >= tt);
		tt = temp.top()._time;
		temp.pop();
	}
}

void StepableQueue::SerialiseTo(std::ostringstream& out) const {
	out <<= _last_frame;

	out <<= (int)_queue.size();

	std::priority_queue<QueueEntry> temp(_queue);

	while (temp.size()) {
		QueueEntry qe = temp.top();
		temp.pop();

		out <<= qe._time;

		Entity* e = dynamic_cast<Entity*>(qe._s);
		if (e) {
			out <<= (char)1;
			// reference entities by their map position as we already serialised them
			out <<= e->GetPos();
		} else {
			// the only non-entity we have so far, will need virtual serialise/deserialise if we get more
			InputHandler* ih = dynamic_cast<InputHandler*>(qe._s);
			assert(ih);

			out <<= (char)0;

			// no data :-)
		}
	}
}

void StepableQueue::SerialiseFrom(std::istringstream& in, const Universe* u) {
	Clear();

	in >>= _last_frame;

	int num_entries;
	in >>= num_entries;

	for (int i = 0; i < num_entries; i++) {
		float time;

		in >>= time;

		char b;
		in >>= b;

		Stepable* s = nullptr;

		if (b) {
			Coord pos;
			in >>= pos;

			Entity* e = const_cast<Actor*>(u->GetWorld()->GetActor(pos));
			assert(e);

			s = dynamic_cast<Stepable*>(e);
			assert(s);
		} else {
			// the only non-entity we have at the moment...
			s = const_cast<InputHandler*>(u->GetInputHandler());
		}

		AddAbsoluteStep(s, time);
	}
}

inline void StepableQueue::Remove(const Stepable* s) {
	if (!Contains(s))
		return;

	std::priority_queue<QueueEntry> temp;

	while (_queue.size()) {
		QueueEntry qe = _queue.top();

		if (qe._s != s) {
			temp.push(qe);
		}

		_queue = temp;
	}

	s->SetQueue(nullptr);
	_contains.erase(s);
}

void StepableQueue::Clear() {
	_queue = std::priority_queue<QueueEntry>();
	_contains.clear();
	_last_frame = 0;
	_last_time = 0.0f;
}

