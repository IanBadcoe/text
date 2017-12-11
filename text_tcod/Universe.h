#pragma once

#include "Command.h"

#include "World.h"
#include "InputHandler.h"
#include "Actor.h"

class Universe : public ICommandSequenceReceiver, public ICommandSender, public ICommandReceiver
{
public:
    Universe(int player_id);
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

	Player* GetPlayer() { return _local_player; }
	const Player* GetPlayer() const { return _local_player; }

private:
	void ProcessCommand(const Command& cmd);
	bool ProcessGlobalCommand(const Command& cmd);

	ICommandReceiver* _pass_commands_to;

    World* _world;

	InputHandler* _input;

	StepableQueue _queue;

	bool _is_ended;

	Player* _local_player;
	int _player_id;
};

