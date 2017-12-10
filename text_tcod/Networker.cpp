#include "Networker.h"

#include "enet/enet.h"

#include "assert.h"


class NetworkData {
public:
    NetworkData(enet_uint16 port) :
        _port(port),
        _ready(false),
        _failed(false),
        _host(nullptr),
        _peer(nullptr),
        _terminate(false),
        _terminated(false) {}

    enet_uint16 _port;

    bool _ready;
    bool _failed;

    ENetHost* _host;
    ENetPeer* _peer;

    bool _terminate;
    bool _terminated;

    Concurrency::critical_section _cs_init;
    Concurrency::critical_section _cs;

    HANDLE _thread;
};

Networker::Networker(enet_uint16 port) :
    _data(new NetworkData(port))
{
    Concurrency::critical_section::scoped_lock sl(_data->_cs);

    _data->_thread = CreateThread(nullptr, 0, NetworkThreadFunc, this, 0, nullptr);
}

Networker::~Networker()
{
    TerminateThread();

    delete _data;
//	DestroyHost();
//	enet_deinitialize();
}

bool Networker::Ready()
{
	// we need _cs_init because we need to return false here if we are still locked
	// but _cs will get locked again later on, and we wouldn't want "ready" 
	Concurrency::critical_section::scoped_lock sl(_data->_cs_init);

	return _data->_ready;
}

bool Networker::Failed()
{
	Concurrency::critical_section::scoped_lock sl(_data->_cs);

	return _data->_failed;
}

bool Networker::TryFindHost()
{
    assert(_data);
    assert(!_data->_host);
    assert(!_data->_peer);

	_data->_host = enet_host_create(
		NULL	/* create a client host */,
		1		/* only allow 1 outgoing connection */,
		1		/* allow up 2 channels to be used, 0 and 1 */,
		0, 0    /* no throttling */);

    if (!_data->_host)
	{
        return false;
    }

    ENetAddress addr;
    addr.host = ENET_HOST_ANY;
    addr.port = _data->_port;

    /* Initiate the connection, allocating the two channels 0 and 1. */
    _data->_peer = enet_host_connect(_data->_host, &addr, 1, 0);

    if (_data->_peer == NULL)
    {
        DestroyHost();

        return false;
    }

    ENetEvent event;

    /* Wait up to 5 seconds for the connection attempt to succeed. */
    if (enet_host_service(_data->_host, &event, 5000) > 0 &&
        event.type == ENET_EVENT_TYPE_CONNECT)
    {
        return true;
    }

    DestroyHost();

    return false;
}

bool Networker::TryCreateHost()
{
	return false;
}

void Networker::DestroyHost()
{
	if (_data->_host)
	{
		enet_host_destroy(_data->_host);
	}

    _data->_host = nullptr;
}

void Networker::TerminateThread()
{
    {
        Concurrency::critical_section::scoped_lock(_data->_cs);

        _data->_terminate = true;
    }

    WaitForSingleObject(_data->_thread, INFINITE);

    _data->_terminated = true;
}

DWORD Networker::NetworkThreadFunc(LPVOID lpParameter)
{
    Networker* me = reinterpret_cast<Networker*>(lpParameter);

    me->InnerThreadFunction();

    return 0;
}

void Networker::InnerThreadFunction()
{
}
