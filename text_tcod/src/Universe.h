#pragma once

#include "InputHandler.h"

class Map;
class Player;

class Universe :
	public ICommandSequenceReceiver,
	public ICommandSender,
	public ICommandReceiver
{
public:
    Universe();
    ~Universe();

	void CreateTestWorld();

    const World* GetWorld() const { return _world; }
	InputHandler* GetInputHandler() const { return _input; }

    // Inherited via ICommandSequenceReceiver
    virtual void ReceiveCommandSequence(const CommandSequence& cs) override;

    // Inherited via ICommandSequenceSender
    virtual void SetCommandReceiver(ICommandReceiver* csr) override;

    // Inherited via ICommandReceiver
    virtual void ReceiveCommand(const Command& c) override;

	// Inherited via ISerialisable
	void SerialiseTo(std::ostringstream& out) const;
	void SerialiseFrom(std::istringstream& in);

	// true -> end of frame
	// false -> more to process
	bool Step() {
		if (!_stepable_queue.AnythingToStep())
			return true;

		return _stepable_queue.Step();
	}

	Player* GetPlayer(int i);
	const Player* GetPlayer(int i) const;

	Player* GetLocalPlayer() { return GetPlayer(_local_player_id); }
	const Player* GetLocalPlayer() const { return GetPlayer(_local_player_id); }

	void EnsurePlayer(int player_id, bool is_local);

	void SetMap(const Map* map) {
		_map = map;
	}

	void QueueNewStepable(Stepable* s) {
		// any new stepable's first step is 1/2 a frame in the future
		_stepable_queue.AddRelativeStep(s, 0.5f);
	}

private:
	void ProcessCommand(const Command& cmd);
	bool ProcessGlobalCommand(const Command& cmd);
	Command TranslateCommand(Command c);

	ICommandReceiver* _pass_commands_to;

    World* _world;

	InputHandler* _input;

	StepableQueue _stepable_queue;

	std::map<int, Player*> _players;
	int _local_player_id;

	// for coordinate translation from commands
	const Map* _map;
};

