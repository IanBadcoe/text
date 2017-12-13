#pragma once

#include "Actor.h"
#include "Command.h"

#include "color.hpp"

#include <assert.h>
#include <queue>

class Player : public Actor, public ICommandReceiver {
public:
    Player(std::istringstream& in);
	Player(int id) : Actor(EntityType::Player, 1.0f), _id(id) {
		assert(_id >= 0 && _id < s_max_players);
	}

	virtual float InnerStep();

	virtual DisplayChar Disp() const;

	int GetId() const { return _id; }
	void SetId(int id) { _id = id; }

	// ICommandReceiver
	virtual void ReceiveCommand(const Command& c) override {
		_command_queue.push(c);
	}

	float ExecuteCommand(const Command& cmd);

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
};
