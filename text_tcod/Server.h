#pragma once

#include "Networker.h"
#include "Command.h"
#include "CommandCollator.h"

class Server :
	public INetworkHandler,
	public ICommandReceiver,
	public ICommandSequenceSender
{
public:
	Server(Networker* network) : _network(network), _this_peer(nullptr) {}

	// Inherited via INetworkHandler
	virtual void Connected(Networker * networker, const PeerHandle peer, bool is_this_peer) override;
	virtual void Disconnected(Networker * networker, const PeerHandle peer, bool is_this_peer) override;
	virtual void Receive(Networker * networker, const PeerHandle peer, const std::vector<uint8_t>& data) override;

	// Inherited via ICommandReceiver
	virtual void ReceiveCommand(const Command & c) override;

	// Inherited via ICommandSequenceSender
	virtual void SetCommandSequenceReceiver(ICommandSequenceReceiver * csr) override;

	bool IsEnded() const { return _collator.IsEnded(); }
	void EndFrame() { _collator.EndFrame(); }

	int AddPeer(PeerHandle peer);

private:
	Networker* _network;
	CommandCollator _collator;

	PeerHandle _this_peer;

	std::map<PeerHandle, int> _peer_to_player_id_map;
};
