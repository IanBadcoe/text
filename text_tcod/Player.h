#pragma once

#include "Actor.h"
#include "Command.h"

#include "color.hpp"

#include "assert.h"

class Player : public Actor, public ICommandSender {
public:
	Player(int num) : Actor(EntityType::Player, 1.0f), _num(num), _commands_dest(nullptr) {
		assert(_num >= 0 && _num < 4);
	}

	virtual float InnerStep();

	virtual DisplayChar& Disp() const;

	int GetNum() const { return _num; }

    // Inherited via ICommandSender
    virtual void SetCommandReceiver(ICommandReceiver * csr) override;

private:
	int _num;

	static DisplayChar s_player[4];
	static TCODColor s_foreground[4];

    ICommandReceiver* _commands_dest;
};
