#include "precompiled.h"

#include "Networker.h"

#include "enet/enet.h"

#include "assert.h"
#include "concrt.h"

class NetworkData {
public:
	NetworkData(enet_uint16 port, int max_players, bool force_no_client) :
		_port(port),
		_ready(false),
		_failed(false),
		_server(false),
        _enet_host(nullptr),
        _enet_peer(nullptr),
        _terminate(false),
        _terminated(false),
		_max_players(max_players),
		_force_no_client(force_no_client) {}

	void SetReady();
	void SetFailed();
	void SetServer();
	void SetTerminate();
	void SetTerminated();

	bool IsReady() const;
	bool IsFailed() const;
	bool IsServer() const;
	bool IsTerminate() const;
	bool IsTerminated() const;

	void SendEvents(Networker* n, INetworkHandler* nh);
	void StoreEvent(const ENetEvent& event);

	// accessed only from this thread...
	const enet_uint16 _port;

    ENetHost* _enet_host;
    ENetPeer* _enet_peer;

    HANDLE _thread;

	const int _max_players;
	const bool _force_no_client;

private:
	mutable Concurrency::critical_section _cs;

	// the parts that get accessed in and out of our thread and thus need the critical section
	bool _ready;
	bool _failed;
	bool _server;
	bool _terminate;
	bool _terminated;

	std::vector<ENetEvent> _event_queue;
};

Networker::Networker(enet_uint16 port, int max_players, bool force_no_client) :
    _data(new NetworkData(port, max_players, force_no_client))
{
    _data->_thread = CreateThread(nullptr, 0, NetworkThreadFunc, this, 0, nullptr);
}

Networker::~Networker()
{
    TerminateThread();

    delete _data;
}

bool Networker::IsReady()
{
	assert(_data);
	return _data->IsReady();
}

bool Networker::IsFailed()
{
	assert(_data);
	return _data->IsFailed();
}

bool Networker::IsServer()
{
	assert(_data);
	return _data->IsServer();
}

void Networker::SetTerminate()
{
	assert(_data);
	_data->SetTerminate();
}

bool Networker::IsTerminated()
{
	assert(_data);
	return _data->IsTerminated();
}

void Networker::SendToServer(const Message& msg)
{
	assert(_data);
	assert(_data->_enet_peer);

	SendToPeer(reinterpret_cast<PeerHandle>(_data->_enet_peer), msg);
}

void Networker::SendToPeer(PeerHandle peer, const uint8_t* data, size_t size)
{
	ENetPacket* packet = enet_packet_create(data,
		size,
		ENET_PACKET_FLAG_RELIABLE);

	enet_peer_send(reinterpret_cast<ENetPeer*>(peer), 0, packet);
}

void Networker::SendToAllPeers(const uint8_t* data, size_t size)
{
	ENetPacket* packet = enet_packet_create(data,
		size,
		ENET_PACKET_FLAG_RELIABLE);

	enet_host_broadcast(_data->_enet_host, 0, packet);
}


void Networker::StoreEvent(const ENetEvent& event)
{
	assert(_data);

	_data->StoreEvent(event);
}

void Networker::SendEvents(INetworkHandler* nh)
{
	assert(_data);

	_data->SendEvents(this, nh);
}

void NetworkData::SetReady()
{
	Concurrency::critical_section::scoped_lock sl(_cs);

	_ready = true;
}

void NetworkData::SetFailed()
{
	Concurrency::critical_section::scoped_lock sl(_cs);

	_failed = true;
}

void NetworkData::SetServer()
{
	Concurrency::critical_section::scoped_lock sl(_cs);

	_server = true;
}

void NetworkData::SetTerminate()
{
	Concurrency::critical_section::scoped_lock sl(_cs);

	_terminate = true;
}

void NetworkData::SetTerminated()
{
	Concurrency::critical_section::scoped_lock sl(_cs);

	_terminated = true;
}

bool NetworkData::IsReady() const
{
	Concurrency::critical_section::scoped_lock sl(_cs);

	return _ready;
}

bool NetworkData::IsFailed() const
{
	Concurrency::critical_section::scoped_lock sl(_cs);

	return _failed;
}

bool NetworkData::IsServer() const
{
	Concurrency::critical_section::scoped_lock sl(_cs);

	return _server;
}

bool NetworkData::IsTerminate() const
{
	Concurrency::critical_section::scoped_lock sl(_cs);

	return _terminate;
}

bool NetworkData::IsTerminated() const
{
	Concurrency::critical_section::scoped_lock sl(_cs);

	return _terminated;
}

void NetworkData::SendEvents(Networker* n, INetworkHandler* nh)
{
	Concurrency::critical_section::scoped_lock sl(_cs);

	for (int i = 0; i < _event_queue.size(); i++)
	{
		ENetEvent event = _event_queue[i];
		switch (event.type) {
		case ENetEventType::ENET_EVENT_TYPE_CONNECT:
		{
			nh->Connected(n, reinterpret_cast<PeerHandle>(event.peer));
			break;
		}

		case ENetEventType::ENET_EVENT_TYPE_DISCONNECT:
		{
			nh->Disconnected(n, reinterpret_cast<PeerHandle>(event.peer));
			break;
		}

		case ENetEventType::ENET_EVENT_TYPE_RECEIVE:
		{
			std::string data(event.packet->data, event.packet->data + event.packet->dataLength);
			enet_packet_destroy(event.packet);
			nh->Receive(n, reinterpret_cast<PeerHandle>(event.peer), data);
			break;
		}
		}
	}

	_event_queue.clear();
}

void NetworkData::StoreEvent(const ENetEvent& event)
{
	Concurrency::critical_section::scoped_lock sl(_cs);

	_event_queue.push_back(event);

}

bool Networker::TryFindHost()
{
    assert(_data);
	
	assert(!_data->_enet_host);
    assert(!_data->_enet_peer);
	assert(!_data->IsReady());

	_data->_enet_host = enet_host_create(
		NULL	/* create a client host */,
		1		/* only allow 1 outgoing connection */,
		2		/* #channels */,
		0, 0	/* no throttling */);

    if (!_data->_enet_host)
	{
        return false;
    }

	int ret;
    ENetAddress address;
//    ret = enet_address_set_host(&addr, "10.7.9.30");
	address.host = ENET_HOST_BROADCAST;
    address.port = _data->_port;

    /* Initiate the connection, allocating the two channels 0 and 1. */
    _data->_enet_peer = enet_host_connect(_data->_enet_host, &address, 2, 0);

    if (_data->_enet_peer == NULL)
    {
        DestroyHost();

        return false;
    }

    ENetEvent event;

    /* Wait up to 5 seconds for the connection attempt to succeed. */
	ret = enet_host_service(_data->_enet_host, &event, 5000);

    if (ret > 0 && event.type == ENET_EVENT_TYPE_CONNECT)
    {
		StoreEvent(event);
		return true;
    }

	enet_peer_reset(_data->_enet_peer);
	_data->_enet_peer = nullptr;

    DestroyHost();

    return false;
}

bool Networker::TryCreateHost()
{
	assert(_data);

	assert(!_data->_enet_host);
	assert(!_data->_enet_peer);
	assert(!_data->IsReady());

	ENetAddress address;

	/* A specific host address can be specified by   */
	/* enet_address_set_host (& address, "x.x.x.x"); */
	address.host = ENET_HOST_ANY;
//	int ret = enet_address_set_host(&address, "10.7.9.30");
	/* Bind the server to port 1234. */
	address.port = _data->_port;
	_data->_enet_host = enet_host_create(
		&address				/* the address to bind the server host to */,
		32, //_data->_max_players		/* #clients */,
		2						/* #channels */,
		0, 0					/* no throttling */);

	if (_data->_enet_host == NULL)
	{
		return false;
	}

	return true;
}

void Networker::DestroyHost()
{
	if (_data->_enet_host)
	{
		enet_host_destroy(_data->_enet_host);
	}

    _data->_enet_host = nullptr;
}

void Networker::TerminateThread()
{
	assert(_data);

	_data->SetTerminate();

    WaitForSingleObject(_data->_thread, INFINITE);

    _data->SetTerminated();
}

DWORD Networker::NetworkThreadFunc(LPVOID lpParameter)
{
    Networker* me = reinterpret_cast<Networker*>(lpParameter);

    me->InnerThreadFunction();

    return 0;
}

void Networker::LeaveSession() {
	if (!_data->_enet_peer)
		return;

	assert(_data->_enet_host);

	ENetEvent event;
	enet_peer_disconnect(_data->_enet_peer, 0);

	/* Allow up to 3 seconds for the disconnect to succeed
	* and drop any packets received packets.
	*/
	while (enet_host_service(_data->_enet_host, &event, 3000) > 0)
	{
		switch (event.type)
		{
		case ENET_EVENT_TYPE_RECEIVE:
			enet_packet_destroy(event.packet);
			break;
		case ENET_EVENT_TYPE_DISCONNECT:
			_data->_enet_peer = nullptr;

			return;
		}
	}

	/* We've arrived here, so the disconnect attempt didn't */
	/* succeed yet.  Force the connection down.             */
	enet_peer_reset(_data->_enet_peer);
	_data->_enet_peer = nullptr;
}


void Networker::InnerThreadFunction()
{
	if (enet_initialize() != 0)
	{
		_data->SetFailed();
		_data->SetReady();

		return;
	}

	if (!_data->_force_no_client && TryFindHost())
	{
		_data->SetReady();
	}
	else if (TryCreateHost())
	{
		_data->SetServer();
		_data->SetReady();
	}
	else
	{
		_data->SetFailed();
		_data->SetReady();

		return;
	}

	while (true) {
		if (_data->IsTerminate())
		{
			if (_data->_enet_peer)
			{
				LeaveSession();
			}

			DestroyHost();
			enet_deinitialize();

			break;
		}

		ENetEvent event;

		int ret = enet_host_service(_data->_enet_host, &event, 5);

		if (ret < 0)
		{
			_data->SetTerminate();
		}
		else if (ret > 0)
		{
			StoreEvent(event);
		}
	}
}
