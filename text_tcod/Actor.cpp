#include "Actor.h"

#include "World.h"

void Stepable::Step(StepableQueue* queue, float time)
{
	float duration = InnerStep();

	if (_speed != 0.0f && duration != 0.0f)
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

	// if we have reached the frame the next step should be in
	// execute that step
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
