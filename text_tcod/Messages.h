#pragma once

#include "Command.h"

#include <vector>

#include <sstream>

class Universe;

struct Message {
	enum class Type {
		Unknwown,
		JoinResponse,
		Command,
		Universe
	};

	Message(Type t) : _type(t) {}
	Message() : _type(Type::Unknwown) {}

	Type _type;

	virtual void ToBytes(std::ostringstream& out) const = 0;
	virtual void FromBytes(std::istringstream& in) = 0;

	void ToBytesBase(std::ostringstream& out) const;
	void FromBytesBase(std::istringstream& in);
};

struct PeerJoinedMessage : public Message {
	PeerJoinedMessage() {}
	PeerJoinedMessage(int player_id, int frame_number) :
		Message(Message::Type::JoinResponse),
		_player_id(player_id),
		_frame_number(frame_number) {}

	int _player_id;
	int _frame_number;

	// Inherited via Message
	virtual void ToBytes(std::ostringstream& out) const override;
	virtual void FromBytes(std::istringstream& in) override;
};

struct PeerLeftMessage : public Message {
	PeerLeftMessage() {}
	PeerLeftMessage(int player_id) :
		Message(Message::Type::JoinResponse),
		_player_id(player_id) {}

	int _player_id;

	// Inherited via Message
	virtual void ToBytes(std::ostringstream& out) const override;
	virtual void FromBytes(std::istringstream& in) override;
};

struct CommandMessage : public Message {
	CommandMessage() {}
	CommandMessage(const Command& cmd) : Message(Message::Type::Command), _command(cmd) {}

	Command _command;

	// Inherited via Message
	virtual void ToBytes(std::ostringstream& out) const override;
	virtual void FromBytes(std::istringstream& in) override;
};

struct UniverseMessage : public Message {
	UniverseMessage(Universe* u) : Message(Message::Type::Universe), _universe(u) {}

	Universe* _universe;

	// Inherited via Message
	virtual void ToBytes(std::ostringstream& out) const override;
	virtual void FromBytes(std::istringstream& in) override;
};
