#pragma once

#include "Command.h"

#include "World.h"
#include "InputHandler.h"
#include "Actor.h"
#include "Networker.h"

#include <map>

class Universe :
	public ICommandSequenceReceiver,
	public ICommandSender,
	public ICommandReceiver,
	public INetworkHandler
{
public:
    Universe();
    ~Universe();

    const World* GetWorld() const { return _world; }

    // Inherited via ICommandSequenceReceiver
    virtual void ReceiveCommandSequence(const CommandSequence & cs) override;

    // Inherited via ICommandSequenceSender
    virtual void SetCommandReceiver(ICommandReceiver * csr) override;

    // Inherited via ICommandReceiver
    virtual void ReceiveCommand(const Command & c) override;

	// true -> end of frame
	// false -> more to process
	bool Step() {
		if (!_queue.AnythingToStep())
			return true;

		return _queue.Step();
	}

	bool IsEnded() const {
		return _is_ended;
	}

	Player* GetPlayer(int i) { assert(i >= 0 && i < 4);  return _players[i]; }
	const Player* GetPlayer(int i) const { assert(i >= 0 && i < 4);  return _players[i]; }

	// Inherited via INetworkHandler
	virtual void Connected(Networker* networker, const PeerHandle peer) override;
	virtual void Disconnected(Networker* networker, const PeerHandle peer) override;
	virtual void Receive(Networker* networker, const PeerHandle peer, const std::vector<uint8_t>& data) override;

	void SetPlayerId(int id);

private:
	void ProcessCommand(const Command& cmd);
	bool ProcessGlobalCommand(const Command& cmd);

	int AddPeer(PeerHandle peer);

	ICommandReceiver* _pass_commands_to;

    World* _world;

	InputHandler* _input;

	StepableQueue _queue;

	bool _is_ended;

	Player* _players[4];
	int _player_id;

	std::map<PeerHandle, int> _peer_to_player_id_map;
};

