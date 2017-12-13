#include "precompiled.h"

#include "Actor.h"

#include "World.h"
#include "ISerialisable.h"
#include "InputHandler.h"

void Stepable::Step(StepableQueue* queue, float time)
{
	float duration = InnerStep();

	if (_speed != 0.0f && duration != 0)
	{
		queue->AddFutureStep(this, time + duration / _speed);
	}
}

bool StepableQueue::Step()
{
	assert(_queue.size());

	QueueEntry qe = _queue.top();

	float new_time = qe._time;

	int new_frame = (int)new_time;

	// if we have reached the frame the next step should be
	// executed in that step
	if (new_frame - _last_frame < 2)
	{
		_queue.pop();

		qe._s->Step(this, new_time);
	}

	// if we should be on a frame above the current one, step towards it
	if (new_frame > _last_frame) {
		_last_frame++;
		return false;
	}

	return true;
}

void StepableQueue::SerialiseTo(std::ostringstream& out) const
{
	out <<= _last_frame;

	out <<= _queue.size();

	std::priority_queue<QueueEntry> temp(_queue);

	while (temp.size()) {
		QueueEntry qe = temp.top();
		temp.pop();

		out <<= qe._time;

		Entity* e = dynamic_cast<Entity*>(qe._s);
		if (e)
		{
			out <<= (char)1;
			// reference entities by their map position as we already serialised them
			out <<= e->GetPos();
		}
		else
		{
			// the only non-entity we have so far, will need virtual serialise/deserialise if we get more
			InputHandler* ih = dynamic_cast<InputHandler*>(qe._s);
			assert(ih);

			out <<= (char)0;

			// no data :-)
		}
	}
}

void StepableQueue::SerialiseFrom(std::istringstream& in, const World* world)
{
	in >>= _last_frame;

	int num_entries;
	in >>= num_entries;

	for (int i = 0; i < num_entries; i++)
	{
		float time;

		in >>= time;

		Coord pos;
		in >>= pos;

		char b;
		in >>= b;

		Stepable* s = nullptr;

		if (b)
		{
			Entity* e = const_cast<Actor*>(world->GetActor(pos));
			assert(e);

			s = dynamic_cast<Stepable*>(e);
			assert(s);

		}
		else
		{
			// the only non-entity we have at the moment...
			s = new InputHandler();
		}

		QueueEntry qe(s, time);
		_queue.push(qe);
	}
}

Stepable::~Stepable() {
	if (_queue)
		_queue->Remove(this);
}
