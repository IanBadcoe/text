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

	case Message::Type::JoinResponse:
	{
		PeerJoinedMessage pj;
		pj.FromBytes(str);

		_universe->EnsurePlayer(pj._player_id, true);
		_current_frame = pj._frame_number;
	}

	case Message::Type::Universe:
	{
		UniverseMessage um(_universe);
		um.FromBytes(str);
	}
	}
}

void Client::ReceiveCommand(const Command& c)
{
	Command temp(c);
	temp._from_frame = _current_frame;

	CommandMessage cm(c);

//	_network->
}

void Client::ReceiveCommandSequence(const CommandSequence& cs)
{
}

void Client::SetCommandSequenceReceiver(ICommandSequenceReceiver* csr)
{
}

/*void Universe::Connected(Networker* networker, const PeerHandle peer)
{
	// Pause all clients

	int peer_id = AddPeer(peer);

	JoinResponseMessage jrm;
	jrm._player_num = peer_id;

	networker->SendToPeer(peer, jrm);
}

void Universe::Disconnected(Networker* networker, const PeerHandle peer)
{
}

void Universe::Receive(Networker* networker, const PeerHandle peer, const std::vector<uint8_t>& data)
{
	const Message* temp = reinterpret_cast<const Message*>(data.data());

	switch (temp->_type)
	{
	case Message::Type::JoinResponse:
	{
		JoinResponseMessage jrm;
		CrackMessage(data.data(), data.size(), jrm);

		/// or could not make Player until now?
		SetLocalPlayerId(jrm._player_num);

		break;
	}
	}
}

*/