#pragma once

#include "enet/enet.h"

#include "Messages.h"

class NetworkData;
class Networker;

class PeerDummy;

typedef PeerDummy* PeerHandle;

class INetworkHandler {
public:
	virtual void Connected(Networker* networker, const PeerHandle peer) = 0;
	virtual void Disconnected(Networker* networker, const PeerHandle peer) = 0;
	virtual void Receive(Networker* networker, const PeerHandle peer, const std::string& data) = 0;
};

class Networker {
public:
	Networker(enet_uint16 port, int max_players, bool force_no_client);
	~Networker();

	bool IsReady();
	bool IsFailed();
    bool IsServer();

	void SetTerminate();
	bool IsTerminated();

	void SendToServer(const Message& msg);
	void SendToServer(const uint8_t* data, size_t size);
	void SendToPeer(PeerHandle peer, const Message& msg);
	void SendToAllPeers(const Message& msg);
	void SendToPeer(PeerHandle peer, const uint8_t* data, size_t size);
	void SendToAllPeers(const uint8_t* data, size_t size);

	void SendEvents(INetworkHandler* nh);

private:
	bool TryFindServer();
	bool TryCreateServer();

	void DestroyServer();
    void TerminateThread();

	void LeaveSession();

    static DWORD WINAPI NetworkThreadFunc(_In_ LPVOID lpParameter);
    void InnerThreadFunction();

    NetworkData* _data;

	// accessed only from this thread...
	const enet_uint16 _port;
	const int _max_players;
	const bool _force_no_client;
};
