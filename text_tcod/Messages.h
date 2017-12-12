#pragma once

#include <vector>

#include <sstream>

struct Message {
	enum class Type {
		JoinResponse
	};

	Message(Type t) : _type(t) {}

	Type _type;

	virtual void ToBytes(std::ostringstream& out) = 0;
	virtual void FromBytes(std::istringstream& in) = 0;

	void ToBytesBase(std::ostringstream& out);
	void FromBytesBase(std::istringstream& in);
};

struct PeerJoined : public Message {
	PeerJoined() : Message(Message::Type::JoinResponse) {}

	int _player_num;

	// Inherited via Message
	virtual void ToBytes(std::ostringstream & out) override;
	virtual void FromBytes(std::istringstream & in) override;
};
