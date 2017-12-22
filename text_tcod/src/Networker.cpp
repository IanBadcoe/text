#include "precompiled.h"

#include "Networker.h"

#include "enet/enet.h"

class NetworkData {
public:
	NetworkData() :
		_ready(false),
		_failed(false),
		_server(false),
        _enet_host(nullptr),
        _enet_peer(nullptr),
        _terminate(false),
        _terminated(false) {}

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

	void SendToServer(const uint8_t* data, size_t size);
	void SendToPeer(ENetPeer* peer, const uint8_t* data, size_t size);
	void SendToAllPeers(const uint8_t* data, size_t size);

	void Service();

    ENetHost* _enet_host;
    ENetPeer* _enet_peer;

    HANDLE _thread;

private:
	void StoreEvent(const ENetEvent& event);

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
	_data(new NetworkData()),
	_port(port),
	_max_players(max_players),
	_force_no_client(force_no_client)
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

void Networker::SendEvents(INetworkHandler* nh)
{
	assert(_data);

	_data->SendEvents(this, nh);
}

bool Networker::TryFindServer()
{
	// no critical section here, but main program should be waiting for us to be ready...
	assert(_data);

	assert(!_data->_enet_host);
	assert(!_data->_enet_peer);
	assert(!_data->IsReady());

	_data->_enet_host = enet_host_create(
		NULL	/* create a client host */,
		1		/* only allow 1 outgoing connection */,
		1		/* #channels */,
		0, 0	/* no throttling */);

	if (!_data->_enet_host)
	{
		return false;
	}

	int ret;
	ENetAddress address;
	//    ret = enet_address_set_host(&addr, "10.7.9.30");
	address.host = ENET_HOST_BROADCAST;
	address.port = _port;

	_data->_enet_peer = enet_host_connect(_data->_enet_host, &address, 1, 0);

	if (_data->_enet_peer == NULL)
	{
		DestroyServer();

		return false;
	}

	ENetEvent event;

	/* Wait up to 5 seconds for the connection attempt to succeed. */
	ret = enet_host_service(_data->_enet_host, &event, 5000);

	if (ret > 0 && event.type == ENET_EVENT_TYPE_CONNECT)
	{
		// don't actually use this on an client...
//		_data->StoreEvent(event);
		return true;
	}

	enet_peer_reset(_data->_enet_peer);
	_data->_enet_peer = nullptr;

	DestroyServer();

	return false;
}

bool Networker::TryCreateServer()
{
	// no critical section here, but main program should be waiting for us to be ready...
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
	address.port = _port;
	_data->_enet_host = enet_host_create(
		&address				/* the address to bind the server host to */,
		_max_players - 1		/* #clients (we are one of the players :-)) */,
		1						/* #channels */,
		0, 0					/* no throttling */);

	if (_data->_enet_host == NULL)
	{
		return false;
	}

	return true;
}

void Networker::DestroyServer()
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

	// _force_no_client makes us go straight to server creation
	if (!_force_no_client && TryFindServer())
	{
		_data->SetReady();
	}
	else if (TryCreateServer())
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

			DestroyServer();
			enet_deinitialize();

			break;
		}


		_data->Service();
	}
}

void Networker::SendToServer(const Message & msg)
{
	std::ostringstream str;
	std::string buffer;
	buffer.reserve(1000);
	str.str(buffer);

	msg.ToBytes(str);

	std::string s = str.str();

	SendToServer(reinterpret_cast<const uint8_t*>(s.data()), s.size());
}

void Networker::SendToServer(const uint8_t * data, size_t size)
{
	assert(_data);

	_data->SendToServer(data, size);
}

void Networker::SendToPeer(PeerHandle peer, const Message& msg)
{
	std::ostringstream str;
	std::string buffer;
	buffer.reserve(1000);
	str.str(buffer);

	msg.ToBytes(str);

	std::string s = str.str();

	SendToPeer(peer, reinterpret_cast<const uint8_t*>(s.data()), s.size());
}

void Networker::SendToAllPeers(const Message& msg)
{
	std::ostringstream str;

	msg.ToBytes(str);

	std::string s = str.str();

	SendToAllPeers(reinterpret_cast<const uint8_t*>(s.data()), s.size());
}

void Networker::SendToPeer(PeerHandle peer, const uint8_t * data, size_t size)
{
	assert(_data);
	
	_data->SendToPeer(reinterpret_cast<ENetPeer*>(peer), data, size);
}

void Networker::SendToAllPeers(const uint8_t * data, size_t size)
{
	assert(_data);

	_data->SendToAllPeers(data, size);
}

// -- NetworkData

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
	PeerHandle from_whom;
	std::string packet_data;
	ENetEventType type;

	{
		// do all the Enet non-thread-safe stuff in here
		// INCLUDING THE PACKET DELETE
		Concurrency::critical_section::scoped_lock sl(_cs);

		if (!_event_queue.size())
			return;

		const ENetEvent& ev = _event_queue.front();

		type = ev.type;
		from_whom = reinterpret_cast<PeerHandle>(ev.peer);

		if (type == ENetEventType::ENET_EVENT_TYPE_RECEIVE) 			{
			packet_data.insert(packet_data.begin(), ev.packet->data, ev.packet->data + ev.packet->dataLength);
			enet_packet_destroy(ev.packet);
		}

		_event_queue.erase(_event_queue.begin());
	}

	switch (type) {
	case ENetEventType::ENET_EVENT_TYPE_CONNECT:
	{
		nh->Connected(n, from_whom);
		break;
	}

	case ENetEventType::ENET_EVENT_TYPE_DISCONNECT:
	{
		nh->Disconnected(n, from_whom);
		break;
	}

	case ENetEventType::ENET_EVENT_TYPE_RECEIVE:
	{
		nh->Receive(n, from_whom, packet_data);
		break;
	}
	}
}

void NetworkData::StoreEvent(const ENetEvent& event)
{
	_event_queue.push_back(event);
}

void NetworkData::SendToServer(const uint8_t* data, size_t size)
{
	Concurrency::critical_section::scoped_lock sl(_cs);

	assert(_enet_peer);

	ENetPacket* packet = enet_packet_create(data,
		size,
		ENET_PACKET_FLAG_RELIABLE);

	enet_peer_send(_enet_peer, 0, packet);
}

void NetworkData::SendToPeer(ENetPeer* peer, const uint8_t* data, size_t size)
{
//	Concurrency::critical_section::scoped_lock sl(_cs);

	ENetPacket* packet = enet_packet_create(data,
		size,
		ENET_PACKET_FLAG_RELIABLE);

	enet_peer_send(peer, 0, packet);
}

void NetworkData::SendToAllPeers(const uint8_t* data, size_t size)
{
//	Concurrency::critical_section::scoped_lock sl(_cs);

	assert(_enet_host);

	ENetPacket* packet = enet_packet_create(data,
		size,
		ENET_PACKET_FLAG_RELIABLE);

	enet_host_broadcast(_enet_host, 0, packet);
}

void NetworkData::Service()
{
	ENetEvent event;

	int ret = 0;

	{
		Concurrency::critical_section::scoped_lock sl(_cs);
		ret = enet_host_service(_enet_host, &event, 5);
	}

	if (ret < 0)
	{
		SetTerminate();
	}
	else if (ret > 0)
	{
		StoreEvent(event);
	}
}
