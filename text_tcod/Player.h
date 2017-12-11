#pragma once

#include "Actor.h"
#include "Command.h"

#include "color.hpp"

#include <assert.h>
#include <queue>

class Player : public Actor, public ICommandReceiver {
public:
	Player(int num) : Actor(EntityType::Player, 1.0f), _num(num) {
		assert(_num >= 0 && _num < 4);
	}

	virtual float InnerStep();

	virtual DisplayChar& Disp() const;

	int GetNum() const { return _num; }

	// ICommandReceiver
	virtual void ReceiveCommand(const Command& c) override {
		_command_queue.push(c);
	}

	float ExecuteCommand(const Command& cmd);

private:
	int _num;

	std::queue<Command> _command_queue;

	static DisplayChar s_player[4];
	static TCODColor s_foreground[4];
};
