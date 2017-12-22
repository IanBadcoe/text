#pragma once

#include "Networker.h"
#include "Command.h"
#include "CommandCollator.h"

class Universe;

class Server :
	public INetworkHandler,
	public ICommandReceiver,
	public ICommandSequenceReceiver
{
public:
	Server(Networker* network, Universe* universe);

	// Inherited via INetworkHandler
	virtual void Connected(Networker* networker, const PeerHandle peer) override;
	virtual void Disconnected(Networker* networker, const PeerHandle peer) override;
	virtual void Receive(Networker* networker, const PeerHandle peer, const std::string& data) override;

	// Inherited via ICommandReceiver
	virtual void ReceiveCommand(const Command& c) override;

	// Inherited via ICommandReceiver
	virtual void ReceiveCommandSequence(const CommandSequence& cs) override;

	bool IsEnded() const { return _collator.IsEnded(); }
	void EndFrame() {
		_collator.EndFrame();
	}

	int AddPeer(PeerHandle peer);
	int RemovePeer(PeerHandle peer);

private:
	Networker* _network;
	CommandCollator _collator;

	std::map<PeerHandle, int> _peer_to_player_id_map;

	Universe* _universe;
};
