#pragma once

#include "Command.h"

#include "World.h"
#include "InputHandler.h"
#include "Actor.h"
#include "Networker.h"
#include "ISerialisable.h"

#include <map>

class Universe :
	public ICommandSequenceReceiver,
	public ICommandSender,
	public ICommandReceiver,
	public ISerialisable
{
public:
    Universe();
    ~Universe();

    const World* GetWorld() const { return _world; }

    // Inherited via ICommandSequenceReceiver
    virtual void ReceiveCommandSequence(const CommandSequence& cs) override;

    // Inherited via ICommandSequenceSender
    virtual void SetCommandReceiver(ICommandReceiver* csr) override;

    // Inherited via ICommandReceiver
    virtual void ReceiveCommand(const Command& c) override;

	// Inherited via ISerialisable
	virtual void SerialiseTo(std::ostringstream& out) const override;
	virtual void SerialiseFrom(std::istringstream& in) override;

	// true -> end of frame
	// false -> more to process
	bool Step() {
		if (!_queue.AnythingToStep())
			return true;

		return _queue.Step();
	}

	Player* GetPlayer(int i);
	const Player* GetPlayer(int i) const;

	Player* GetLocalPlayer() { return GetPlayer(_local_player_id); }
	const Player* GetLocalPlayer() const { return GetPlayer(_local_player_id); }

	void EnsurePlayer(int player_id, bool is_local);

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

