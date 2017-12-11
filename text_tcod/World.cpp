#include "precompiled.h"

#include "World.h"

#include "Player.h"

World::World(int width, int height) : _width(width), _height(height)
{
	_terrain = new Terrain*[_width * _height];
	_actors = new Actor*[_width * _height];

	for (int i = 0; i < _width; i++) {
		for (int j = 0; j < _width; j++) {
			_terrain[idx(Coord(i, j))] = nullptr;
			_actors[idx(Coord(i, j))] = nullptr;
		}
	}

	_players[0] = nullptr;
	_players[1] = nullptr;
	_players[2] = nullptr;
	_players[3] = nullptr;
}

World::~World()
{
	for (int i = 0; i < _width; i++) {
		for (int j = 0; j < _height; j++) {
			ClearTerrain(Coord(i, j));
			ClearEntity(Coord(i, j));
		}
	}

	delete _terrain;
}

void World::SetTerrain(Coord pos, Terrain* e)
{
	ClearTerrain(pos);

	_terrain[idx(pos)] = e;

	if (e) {
		e->SetPos(pos);
	}
}

void World::AddActor(Coord pos, Actor* e)
{
	assert(!_actors[idx(pos)]);

	_actors[idx(pos)] = e;

	if (e) {
		e->SetPos(pos);
		e->SetWorld(this);
	}

	if (e->GetType() == EntityType::Player)
	{
		Player* p = static_cast<Player*>(e);

		int num = p->GetNum();

		assert(_players[num] == nullptr || _players[num] == p);

		_players[num] = p;
	}
}

void World::RemoveActor(Coord pos)
{
	assert(_actors[idx(pos)]);

	_actors[idx(pos)]->SetWorld(nullptr);
	_actors[idx(pos)] = nullptr;
}

void World::ClearTerrain(Coord pos)
{
	Entity* e = GetTerrain(pos);

	if (e)
	{
		delete e;
	}

	_terrain[idx(pos)] = nullptr;
}

void World::ClearEntity(Coord pos)
{
	Entity* e = GetActor(pos);

	if (e)
	{
		delete e;
	}

	_actors[idx(pos)] = nullptr;
}

const Player* World::GetPlayer(int i) const
{
    return const_cast<World*>(this)->GetPlayer(i);
}

Player* World::GetPlayer(int i) {
    assert(i >= 0 && i < 4);

    return _players[i];
}

