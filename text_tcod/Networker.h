#pragma once

#include "enet/enet.h"

#include "Messages.h"

#include <vector>

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
	bool TryFindHost();
	bool TryCreateHost();

	void DestroyHost();
    void TerminateThread();

	void LeaveSession();

    static DWORD WINAPI NetworkThreadFunc(_In_ LPVOID lpParameter);
    void InnerThreadFunction();

	void StoreEvent(const ENetEvent& event);

    NetworkData* _data;
};
