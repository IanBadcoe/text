#pragma once

#include "concrt.h"

#include "enet/enet.h"

#include <vector>

class NetworkData;
class Networker;

class PeerDummy;

typedef PeerDummy* PeerHandle;

class INetworkHandler {
public:
	virtual void Connected(Networker* networker, const PeerHandle peer) = 0;
	virtual void Disconnected(Networker* networker, const PeerHandle peer) = 0;
	virtual void Receive(Networker* networker, const PeerHandle peer, const std::vector<uint8_t>& data) = 0;
};

class Networker {
public:
	Networker(enet_uint16 port);
	~Networker();

	bool IsReady();
	bool IsFailed();
    bool IsServer();

	void SetTerminate();
	bool IsTerminated();

	void SetNetworkHandler(INetworkHandler* nh);

	template <typename T> void SendToPeer(PeerHandle peer, const T& msg);
	template <typename T> void SendToAllPeers(const T& msg);
	void SendToPeer(PeerHandle peer, const uint8_t* data, size_t size);
	void SendToAllPeers(const uint8_t* data, size_t size);

private:
	bool TryFindHost();
	bool TryCreateHost();

	void DestroyHost();
    void TerminateThread();

	void LeaveSession();

    static DWORD WINAPI NetworkThreadFunc(_In_ LPVOID lpParameter);
    void InnerThreadFunction();

    NetworkData* _data;
};

template<typename T>
inline void Networker::SendToPeer(PeerHandle peer, const T& msg)
{
	SendToPeer(peer, reinterpret_cast<const uint8_t*>(&msg), sizeof(T));
}

template<typename T>
inline void Networker::SendToAllPeers(const T& msg)
{
	SendToAllPeers(reinterpret_cast<const uint8_t*>(&msg), sizeof(T));
}
