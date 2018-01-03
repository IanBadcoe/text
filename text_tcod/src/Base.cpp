#include "precompiled.h"

#include "Base.h"

#include "Floor.h"
#include "Player.h"
#include "Universe.h"
#include "World.h"

static Entity* CreateBase(std::istringstream& in, World* w) {
    return new Base(in, w);
}

EntityCreator Base::s_creator(EntityType::Base, CreateBase);

Base::Base(std::istringstream& in, World* w) : Terrain(in)
{
	int player_id;
	in >>= player_id;
	in >>= _player_in;
	in >>= _id;

	if (player_id != -1)
	{
		SetPlayer(w->GetUniverse()->GetPlayer(player_id));
	}

	w->RegisterBase(_id, this);
}

void Base::SerialiseTo(std::ostringstream& out) const
{
	Terrain::SerialiseTo(out);

	out <<= _player ? _player->GetId() : -1;
	out <<= _player_in;
	out <<= _id;
}

void Base::CalcDisp(const Terrain* surrounds[8])
{
	FixDisplay();
}

bool Base::DrawCompatWith(const Terrain* other) const {
	return false;
}

inline void Base::FixDisplay() {
	if (_player_in) {
		SetDisplayChar(DisplayChar('@', _player->GetColour(), Floor::s_foreground));
	} else if (_player) {
		SetDisplayChar(DisplayChar(0x0f, _player->GetColour(), Floor::s_foreground));
	} else {
		SetDisplayChar(DisplayChar(0x0f, TCOD_gray, Floor::s_foreground));
	}
}

int Base::GetId() const {
	return _id;
}

static Entity* CreateBaseEdge(std::istringstream& in, World* w) {
	return new BaseEdge(in, w);
}

EntityCreator BaseEdge::s_creator(EntityType::BaseEdge, CreateBaseEdge);

BaseEdge::BaseEdge(std::istringstream& in, World* w) : Terrain(in) {
	int base_id;
	in >>= base_id;

	SetBase(w->GetBase(base_id));
}

BaseEdge::BaseEdge(const Base* base, Coord::Dir dir) : Terrain(EntityType::BaseEdge, SerialiseOrder::TerrainAfterBase, false, true), _base(base) {
	switch (dir) {
	case Coord::Dir::North:
		SetDisplayChar(DisplayChar(0xe8, TCOD_gray, Floor::s_foreground));
		break;
	case Coord::Dir::NorthEast:
		SetDisplayChar(DisplayChar(0xe9, TCOD_gray, Floor::s_foreground));
		break;
	case Coord::Dir::East:
		SetDisplayChar(DisplayChar(0xf9, TCOD_gray, Floor::s_foreground));
		break;
	case Coord::Dir::SouthEast:
		SetDisplayChar(DisplayChar(0xec, TCOD_gray, Floor::s_foreground));
		break;
	case Coord::Dir::South:
		SetDisplayChar(DisplayChar(0xeb, TCOD_gray, Floor::s_foreground));
		break;
	case Coord::Dir::SouthWest:
		SetDisplayChar(DisplayChar(0xea, TCOD_gray, Floor::s_foreground));
		break;
	case Coord::Dir::West:
		SetDisplayChar(DisplayChar(0xf7, TCOD_gray, Floor::s_foreground));
		break;
	case Coord::Dir::NorthWest:
		SetDisplayChar(DisplayChar(0xe7, TCOD_gray, Floor::s_foreground));
		break;
	}
}

void BaseEdge::SerialiseTo(std::ostringstream& out) const {
	Terrain::SerialiseTo(out);

	out <<= _base->GetId();
}

void BaseEdge::CalcDisp(const Terrain* surrounds[8]) {
}

bool BaseEdge::DrawCompatWith(const Terrain* other) const {
	return false;
}

void BaseEdge::SetBase(const Base* base) {
	_base = base;
}
