#include "precompiled.h"

#include "Client.h"


void Client::Connected(Networker * networker, const PeerHandle peer, bool is_this_peer)
{
}

void Client::Disconnected(Networker * networker, const PeerHandle peer, bool is_this_peer)
{
}

void Client::Receive(Networker * networker, const PeerHandle peer, const std::vector<uint8_t>& data)
{
}

void Client::ReceiveCommand(const Command & c)
{
}

void Client::ReceiveCommandSequence(const CommandSequence & cs)
{
}

void Client::SetCommandSequenceReceiver(ICommandSequenceReceiver * csr)
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