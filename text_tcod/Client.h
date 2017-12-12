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
	Client(Networker* network) : _network(network), _is_ended(false), _highest_frame_received(-1), _next_frame_required(0) {}

	// Inherited via INetworkHandler
	virtual void Connected(Networker * networker, const PeerHandle peer, bool is_this_peer) override;
	virtual void Disconnected(Networker * networker, const PeerHandle peer, bool is_this_peer) override;
	virtual void Receive(Networker * networker, const PeerHandle peer, const std::vector<uint8_t>& data) override;

	// Inherited via ICommandReceiver
	virtual void ReceiveCommand(const Command & c) override;

	// Inherited via ICommandSequenceReceiver
	virtual void ReceiveCommandSequence(const CommandSequence & cs) override;

	// Inherited via ICommandSequenceSender
	virtual void SetCommandSequenceReceiver(ICommandSequenceReceiver * csr) override;

	bool IsEnded() const { return _is_ended; }
	bool FrameReceived() const { return _highest_frame_received >= _next_frame_required; }
	void EndFrame() { _next_frame_required++; }

private:
	Networker* _network;
	const PeerHandle* _local_peer;

	bool _is_ended;
	int _highest_frame_received;
	int _next_frame_required;
};
