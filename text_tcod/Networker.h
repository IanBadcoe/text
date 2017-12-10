#pragma once

#include "concrt.h"

#include "enet/enet.h"

#include <vector>

class NetworkData;

class DataReceiver {
public:
    virtual void ReceiveData(const std::vector<uint8_t>& data) = 0;
};

class Networker {
public:
	Networker(enet_uint16 port);
	~Networker();

	bool Ready();
	bool Failed();
    bool IsServer();

    typedef void (*ReceiveHandler)(const std::vector<uint8_t>& data);

    void SetDataReceiver(DataReceiver* dr);

private:
	bool TryFindHost();
	bool TryCreateHost();

	void DestroyHost();
    void TerminateThread();

    static DWORD WINAPI NetworkThreadFunc(_In_ LPVOID lpParameter);
    void InnerThreadFunction();

    NetworkData* _data;
};
