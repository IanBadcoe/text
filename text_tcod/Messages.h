#pragma once

struct Message {
	enum class Type {
		JoinResponse
	};

	Message(Type t) : _type(t) {}

	Type _type;
};

struct JoinResponseMessage : public Message {
	JoinResponseMessage() : Message(Message::Type::JoinResponse) {}

	int _player_num;
};

template<typename T>
inline void CrackMessage(const uint8_t * data, size_t size, T& out)
{
	assert(size == sizeof(T));

	memcpy(&out, data, size);
}
