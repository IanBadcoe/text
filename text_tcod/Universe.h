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
	public ICommandReceiver
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

	Player* GetPlayer(int i);
	const Player* GetPlayer(int i) const;

	void SetLocalPlayerId(int id);

private:
	void ProcessCommand(const Command& cmd);
	bool ProcessGlobalCommand(const Command& cmd);

	ICommandReceiver* _pass_commands_to;

    World* _world;

	InputHandler* _input;

	StepableQueue _queue;

	std::map<int, Player*> _players;
	int _local_player_id;
};

