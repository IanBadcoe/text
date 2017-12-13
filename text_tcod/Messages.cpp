#include "precompiled.h"

#include "Messages.h"

#include "Universe.h"

void PeerJoinedMessage::ToBytes(std::ostringstream& out) const
{
	ToBytesBase(out);

	out <<= _player_id;
	out <<= _frame_number;
}

void PeerJoinedMessage::FromBytes(std::istringstream& in)
{
	FromBytesBase(in);

	in >>= _player_id;
	in >>= _frame_number;
}

void Message::ToBytesBase(std::ostringstream& out) const
{
	out <<= _type;
}

void Message::FromBytesBase(std::istringstream& in)
{
	in >>= _type;
}

void CommandMessage::ToBytes(std::ostringstream& out) const
{
	ToBytesBase(out);

	out <<= _command;
}

void CommandMessage::FromBytes(std::istringstream& in)
{
	FromBytesBase(in);

	in >>= _command;
}

void PeerLeftMessage::ToBytes(std::ostringstream& out) const
{
	ToBytesBase(out);

	out <<= _player_id;
}

void PeerLeftMessage::FromBytes(std::istringstream& in)
{
	FromBytesBase(in);

	in >>= _player_id;
}

void UniverseMessage::ToBytes(std::ostringstream& out) const
{
	ToBytesBase(out);

	_universe->SerialiseTo(out);
}

void UniverseMessage::FromBytes(std::istringstream& in)
{
	FromBytesBase(in);

	_universe->SerialiseFrom(in);
}
