#pragma once

#include "Networker.h"
#include "Command.h"

class Client :
	public INetworkHandler,
	public ICommandReceiver,
	public ICommandSequenceReceiver,
	public ICommandSequenceSender
{
public:
	Client(Networker* network, Universe* universe) : _network(network), _is_ended(false), _highest_frame_received(-1), _current_frame(0), _universe(universe) {}
	~Client() { }

	// Inherited via INetworkHandler
	virtual void Connected(Networker* networker, const PeerHandle peer) override;
	virtual void Disconnected(Networker* networker, const PeerHandle peer) override;
	virtual void Receive(Networker* networker, const PeerHandle peer, const std::string& data) override;

	// Inherited via ICommandReceiver
	virtual void ReceiveCommand(const Command& c) override;

	// Inherited via ICommandSequenceReceiver
	virtual void ReceiveCommandSequence(const CommandSequence& cs) override;

	// Inherited via ICommandSequenceSender
	virtual void SetCommandSequenceReceiver(ICommandSequenceReceiver* csr) override;

	bool IsEnded() const { return _is_ended; }
	bool FrameReceived() const
	{
		// when we are exiting, no point waiting to sych another frame...
		return IsEnded() || _highest_frame_received >= _current_frame + 1;
	}
	void EndFrame() { _current_frame++; }

private:
	Networker* _network;
	const PeerHandle* _local_peer;

	bool _is_ended;
	int _highest_frame_received;
	int _current_frame;

	Universe* _universe;
};
