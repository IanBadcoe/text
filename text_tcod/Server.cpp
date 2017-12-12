#include "precompiled.h"

#include "Server.h"

#include "Messages.h"

#include <set>

void Server::Connected(Networker * networker, const PeerHandle peer, bool is_this_peer)
{
	if (is_this_peer)
	{
		assert(!_this_peer || _this_peer);

		_this_peer = peer;
	}

	PeerJoined pj;
	pj._player_num = AddPeer(peer);

	_network->SendToAllPeers(pj);
}

void Server::Disconnected(Networker * networker, const PeerHandle peer, bool is_this_peer)
{
}

void Server::Receive(Networker * networker, const PeerHandle peer, const std::vector<uint8_t>& data)
{
}

void Server::ReceiveCommand(const Command & c)
{
	_collator.ReceiveCommand(c);
}

void Server::SetCommandSequenceReceiver(ICommandSequenceReceiver * csr)
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
