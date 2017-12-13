#include "precompiled.h"

#include "Server.h"

#include "Messages.h"
#include "Universe.h"

#include <set>

void Server::Connected(Networker* networker, const PeerHandle peer)
{
	printf("Server: Connection, peer=%p\n", peer);

	int player_id = AddPeer(peer);

	_universe->EnsurePlayer(player_id, false);

	UniverseMessage em(_universe);

	_network->SendToPeer(peer, em);

	PeerJoinedMessage pj(player_id, _current_frame);

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
}

void Server::ReceiveCommand(const Command& c)
{
	_collator.ReceiveCommand(c);
}

void Server::SetCommandSequenceReceiver(ICommandSequenceReceiver* csr)
{
	_collator.SetCommandSequenceReceiver(csr);
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
