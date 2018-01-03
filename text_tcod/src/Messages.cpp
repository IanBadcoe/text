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

	out <<= _your_player_id;
	_u->SerialiseTo(out);
}

void UniverseMessage::FromBytes(std::istringstream& in)
{
	FromBytesBase(in);

	in >>= _your_player_id;
	_u->SerialiseFrom(in);

	// set the local player id
	_u->EnsurePlayer(_your_player_id, true);
}

void CommandSequenceMessage::ToBytes(std::ostringstream& out) const
{
	ToBytesBase(out);

	out <<= _sequence._frame;

	out <<= (int)_sequence._commands.size();

	for (int i = 0; i < _sequence._commands.size(); i++)
	{
		out <<= _sequence._commands[i];
	}
}

void CommandSequenceMessage::FromBytes(std::istringstream& in)
{
	FromBytesBase(in);

	in >>= _sequence._frame;

	int num_commands;

	in >>= num_commands;

	for (int i = 0; i < num_commands; i++)
	{
		Command c;

		in >>= c;

		_sequence._commands.push_back(c);
	}
}
