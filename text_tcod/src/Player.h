#pragma once

#include "Actor.h"
#include "Command.h"

class Player : public Actor, public ICommandReceiver {
public:
    Player(std::istringstream& in);
	Player(int id, World* w, const Coord& pos) : Actor(EntityType::Player, w, pos) {
		assert(_id >= 0 && _id < s_max_players);

		SetId(id);
	}

	virtual float InnerStep();

	int GetId() const { return _id; }
	void SetId(int id) {
		_id = id;
		SetDisplayChar(DisplayChar('@', s_foreground[_id]));
	}

	// ICommandReceiver
	virtual void ReceiveCommand(const Command& c) override {
		_command_queue.push(c);
	}

	void ExecuteCommand();

	static void SetMaxPlayers(int num, const TCODColor* colours) {
		s_max_players = num;

		s_foreground.clear();

		for (int i = 0; i < s_max_players; i++)
		{
			s_foreground.push_back(colours[i]);
		}
	}

    // Inherited via Actor
    virtual void SerialiseTo(std::ostringstream& out) const override;

private:
	int _id;

	std::queue<Command> _command_queue;

	static int s_max_players;
	static std::vector<TCODColor> s_foreground;

	static EntityCreator s_creator;
public:
	TCODColor GetColour() const;

	// Inherited via Actor
	virtual void BecomeIdle() override;
};
