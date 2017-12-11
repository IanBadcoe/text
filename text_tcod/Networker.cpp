#include "precompiled.h"

#include "Networker.h"

#include "enet/enet.h"

#include "assert.h"


class NetworkData {
public:
	NetworkData(enet_uint16 port) :
		_port(port),
		_ready(false),
		_failed(false),
		_server(false),
        _enet_host(nullptr),
        _enet_peer(nullptr),
        _terminate(false),
        _terminated(false),
		_network_handler(nullptr) {}

    enet_uint16 _port;

    ENetHost* _enet_host;
    ENetPeer* _enet_peer;

    HANDLE _thread;

	void SetReady();
	void SetFailed();
	void SetServer();
	void SetTerminate();
	void SetTerminated();

	void SetNetworkHandler(INetworkHandler* nh);

	bool IsReady() const;
	bool IsFailed() const;
	bool IsServer() const;
	bool IsTerminate() const;
	bool IsTerminated() const;

	INetworkHandler* GetNetworkHandler() const;
private:
	mutable Concurrency::critical_section _cs;

	// the parts that get accessed in and out of our thread and thus need the critical section
	bool _ready;
	bool _failed;
	bool _server;
	bool _terminate;
	bool _terminated;

	INetworkHandler* _network_handler;
};

Networker::Networker(enet_uint16 port) :
    _data(new NetworkData(port))
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

void Networker::SetNetworkHandler(INetworkHandler * nh)
{
	assert(_data);

	_data->SetNetworkHandler(nh);
}

void Networker::SendToPeer(PeerHandle peer, const uint8_t * data, size_t size)
{
}

void Networker::SendToAllPeers(const uint8_t * data, size_t size)
{
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

void NetworkData::SetNetworkHandler(INetworkHandler * nh)
{
	Concurrency::critical_section::scoped_lock sl(_cs);

	_network_handler = nh;
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

INetworkHandler * NetworkData::GetNetworkHandler() const
{
	Concurrency::critical_section::scoped_lock sl(_cs);

	return _network_handler;
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
		1		/* #channels */,
		0, 0    /* no throttling */);

    if (!_data->_enet_host)
	{
        return false;
    }

    ENetAddress addr;
    addr.host = ENET_HOST_ANY;
    addr.port = _data->_port;

    /* Initiate the connection, allocating the two channels 0 and 1. */
    _data->_enet_peer = enet_host_connect(_data->_enet_host, &addr, 1, 0);

    if (_data->_enet_peer == NULL)
    {
        DestroyHost();

        return false;
    }

    ENetEvent event;

    /* Wait up to 5 seconds for the connection attempt to succeed. */
    if (enet_host_service(_data->_enet_host, &event, 5000) > 0 &&
        event.type == ENET_EVENT_TYPE_CONNECT)
    {
        return true;
    }

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
	/* Bind the server to port 1234. */
	address.port = 1234;
	_data->_enet_host = enet_host_create(&address /* the address to bind the server host to */,
		4		/* #clients */,
		2		/* #channels */,
		0, 0	/* no throttling */);

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
	if (TryFindHost())
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

		int ret = enet_host_service(_data->_enet_host, &event, 50);

		if (ret < 0)
		{
			_data->SetTerminate();
		}
		else if (ret > 0)
		{
			INetworkHandler* nh = _data->GetNetworkHandler();

			switch (event.type) {
			case ENetEventType::ENET_EVENT_TYPE_CONNECT:
			{
				nh->Connected(this, reinterpret_cast<PeerHandle>(event.peer));
				break;
			}
			case ENetEventType::ENET_EVENT_TYPE_DISCONNECT:
			{
				nh->Disconnected(this, reinterpret_cast<PeerHandle>(event.peer));
				break;
			}
			case ENetEventType::ENET_EVENT_TYPE_RECEIVE:
			{
				std::vector<uint8_t> data(event.packet->data, event.packet->data + event.packet->dataLength);
				enet_packet_destroy(event.packet);
				nh->Receive(this, reinterpret_cast<PeerHandle>(event.peer), data);
				break;
			}
			}
		}
	}
}
