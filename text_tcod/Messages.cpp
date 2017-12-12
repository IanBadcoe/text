#include "precompiled.h"

#include "Messages.h"

// utilities

template <typename T>
std::ostringstream& operator<<=(std::ostringstream& out, const T& in) {
	out.write(reinterpret_cast<const char*>(&in), sizeof(T));

	return out;
}

template <typename T>
std::istringstream& operator>>=(std::istringstream& in, T& out) {
	in.read(reinterpret_cast<char*>(&out), sizeof(T));

	return in;
}

void PeerJoined::ToBytes(std::ostringstream& out)
{
	ToBytesBase(out);

	out <<= _player_num;
}

void PeerJoined::FromBytes(std::istringstream& in)
{
	FromBytesBase(in);

	in >>= _player_num;
}

void Message::ToBytesBase(std::ostringstream& out)
{
	out <<= _type;
}

void Message::FromBytesBase(std::istringstream& in)
{
}
