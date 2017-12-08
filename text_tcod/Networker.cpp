#include "Networker.h"

#include "enet/enet.h"

class try_locker {
public:
	try_locker(Concurrency::critical_section& cs) : _cs(cs) {
		_locked = _cs.try_lock();
	}
	~try_locker() {
		if (_locked)
		{
			_cs.unlock();
		}
	}

	bool IsLocked() const { return _locked; }

private:
	bool _locked;
	Concurrency::critical_section& _cs;
};

Networker::Networker() :
	_ready (false),
	_failed (false)
{
	Concurrency::critical_section::scoped_lock sl(_cs_init);

	if (enet_initialize() != 0)
	{
		_ready = true;
		_failed = true;

		return;
	}
}

Networker::~Networker()
{

}

bool Networker::Ready()
{
	// we need _cs_init because we need to return false here if we are still locked
	// but _cs will get locked again later on, and we wouldn't want "ready" 
	try_locker tl(_cs_init);

	if (tl.IsLocked())
	{
		return _ready;
	}

	return false;
}

bool Networker::Failed()
{
	Concurrency::critical_section::scoped_lock sl(_cs);

	return _ready;
}
