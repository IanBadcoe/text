#include "precompiled.h"

#include "Client.h"

#include "Messages.h"
#include "Universe.h"

void Client::Connected(Networker* networker, const PeerHandle peer)
{
	printf("Client: Connection, peer=%p\n", peer);
}

void Client::Disconnected(Networker* networker, const PeerHandle peer)
{
	printf("Client: Disconnection, peer=%p\n", peer);
}

void Client::Receive(Networker* networker, const PeerHandle peer, const std::string& data)
{
	Message::Type t = *reinterpret_cast<const Message::Type*>(data.data());

	std::istringstream str;
	str.str(data);

	switch (t)
	{
	case Message::Type::Command:
	{
		assert(false);

		break;
	}

	case Message::Type::JoinAnnounce:
	{
		PeerJoinedMessage pj;
		pj.FromBytes(str);

		// if this was us, we are already known as the local player and this does nothing
		// if it was somebody else, ensure we have a player object for them...
		_universe->EnsurePlayer(pj._player_id, false);
		_current_frame = pj._frame_number;

		break;
	}

	case Message::Type::LeaveAnnounce:
	{
		PeerLeftMessage pl;
		pl.FromBytes(str);

		// if this was us, we are already known as the local player and this does nothing
		// if it was somebody else, ensure we have a player object for them...
		_universe->EnsurePlayer(pl._player_id, false);

		break;
	}

	case Message::Type::Universe:
	{
		UniverseMessage um(_universe);
		um.FromBytes(str);

		break;
	}

	case Message::Type::CommandSequence:
	{
		CommandSequenceMessage csm;
		csm.FromBytes(str);

		assert(csm._sequence._frame > _highest_frame_received);
		_highest_frame_received = csm._sequence._frame;

		_universe->ReceiveCommandSequence(csm._sequence);

		break;
	}

	default:
	{
		assert(0 == "unrecognised message");
	}
	}
}

void Client::ReceiveCommand(const Command& c)
{
	if (IsEnded())
		return;

	if (c._type == Command::Type::Exit)
	{
		_is_ended = true;
		return;
	}

	CommandMessage cm(c);

	_network->SendToServer(cm);
}

void Client::ReceiveCommandSequence(const CommandSequence& cs)
{
}

void Client::SetCommandSequenceReceiver(ICommandSequenceReceiver* csr)
{
}
