#include "precompiled.h"

#include "Server.h"

#include "Messages.h"
#include "Universe.h"

Server::Server(Networker * network, Universe * universe) :
	_network(network),
	_collator(true),
	_universe(universe) {
	_collator.SetCommandSequenceReceiver(this);
	_universe->CreateTestWorld();
}

void Server::Connected(Networker* networker, const PeerHandle peer)
{
	printf("Server: Connection, peer=%p\n", peer);

	int player_id = AddPeer(peer);

	// add a player object for the new player
	_universe->EnsurePlayer(player_id, false);

	// sync their peer with the current game state
	UniverseMessage um(_universe, player_id);
	_network->SendToPeer(peer, um);

	// tell everybody else they joined
	PeerJoinedMessage pj(player_id, _collator.GetFrame());
	_network->SendToAllPeers(pj);
}

void Server::Disconnected(Networker* networker, const PeerHandle peer)
{
	printf("Server: Disconnection, peer=%p\n", peer);

	int player_id = RemovePeer(peer);

	// if we wanted to do anything to the player object to adjust for it not having a human player, we could here...
	// for the moment just let it freeze

	PeerLeftMessage pl(player_id);

	_network->SendToAllPeers(pl);
}

void Server::Receive(Networker* networker, const PeerHandle peer, const std::string& data)
{
	Message::Type t = *reinterpret_cast<const Message::Type*>(data.data());

	std::istringstream str;
	str.str(data);

	switch (t)
	{
	case Message::Type::Command:
	{
		CommandMessage cm;
		cm.FromBytes(str);

		ReceiveCommand(cm._command);

		break;
	}

	case Message::Type::JoinResponse:
	{
		assert(false);

		break;
	}

	case Message::Type::Universe:
	{
		assert(false);

		break;
	}
	}
}

void Server::ReceiveCommand(const Command& c)
{
	_collator.ReceiveCommand(c);
}

int Server::AddPeer(PeerHandle peer)
{
	assert(_peer_to_player_id_map.find(peer) == _peer_to_player_id_map.end());

	std::set<int> in_use;

	for (auto it = _peer_to_player_id_map.begin(); it != _peer_to_player_id_map.end(); it++)
	{
		in_use.insert(it->second);
	}

	int found = -1;
	int i = 0;

	while (true)
	{
		if (in_use.find(i) == in_use.end())
		{
			found = i;
			break;
		}

		i++;
	}

	assert(found != -1);

	_peer_to_player_id_map[peer] = found;

	return found;
}

int Server::RemovePeer(PeerHandle peer)
{
	auto it = _peer_to_player_id_map.find(peer);

	assert(it != _peer_to_player_id_map.end());

	int ret = it->second;

	_peer_to_player_id_map.erase(it);

	return ret;
}

void Server::ReceiveCommandSequence(const CommandSequence& cs)
{
	_universe->ReceiveCommandSequence(cs);
	
	CommandSequenceMessage csm(cs);

	_network->SendToAllPeers(csm);
}
