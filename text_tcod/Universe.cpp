#include "precompiled.h"

#include "Universe.h"

#include "World.h"
#include "Template.h"
#include "Player.h"
#include "Messages.h"

Universe::Universe() : _is_ended(false), _player_id(0)
{
    _world = new World(200, 200);

    CircleTemplate ct(100, 100, 75, 3);

    ct.Apply(_world);

	_players[_player_id] = new Player(_player_id);

    for (int i = 0; i < 200; i++) {
        if (_world->IsWalkable(Coord(100, i)))
        {
            _world->AddActor(Coord(100, i), _players[_player_id]);
            break;
        }
    }

	_input = new InputHandler();
	_input->SetCommandReceiver(this);

	_queue.AddFutureStep(_input, 0.0f);
	_queue.AddFutureStep(_players[_player_id], 0.5f);
}


Universe::~Universe()
{
}

void Universe::ReceiveCommandSequence(const CommandSequence & cs)
{
	for (int i = 0; i < cs._commands.size(); i++)
	{
		ProcessCommand(cs._commands[i]);
	}
}

void Universe::SetCommandReceiver(ICommandReceiver * csr)
{
    _pass_commands_to = csr;
}

void Universe::ReceiveCommand(const Command & c)
{
	Command temp(c);
	temp._player_id = _player_id;

	_pass_commands_to->ReceiveCommand(temp);
}

void Universe::ProcessCommand(const Command& cmd)
{
	if (!ProcessGlobalCommand(cmd))
	{
		GetPlayer(cmd._player_id)->ReceiveCommand(cmd);
	}
}

bool Universe::ProcessGlobalCommand(const Command & cmd)
{
	if (cmd._type == Command::Type::Exit)
	{
		_is_ended = true;

		return true;
	}

	return false;
}

int Universe::AddPeer(PeerHandle peer)
{
	assert(_peer_to_player_id_map.size() < 4);
	assert(_peer_to_player_id_map.find(peer) == _peer_to_player_id_map.end());

	bool in_use[4] = { false, false, false, false };

	for (auto it = _peer_to_player_id_map.begin(); it != _peer_to_player_id_map.end(); it++)
	{
		in_use[it->second] = true;
	}

	int found = -1;

	for (int i = 0; i < 4; i++)
	{
		if (!in_use[i])
		{
			found = i;
			break;
		}
	}

	assert(found != -1);

	_peer_to_player_id_map[peer] = found;

	return found;
}

void Universe::Connected(Networker* networker, const PeerHandle peer)
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

		SetPlayerId(jrm._player_num);

		break;
	}
	}
}

void Universe::SetPlayerId(int id)
{
	Player* p = _players[_player_id];

	_players[_player_id] = nullptr;

	p->SetId(id);

	_player_id = id;

	_players[_player_id] = p;
}
