#include "World.h"

#include "Player.h"

World::World(int width, int height) : _width(width), _height(height)
{
	_terrain = new Terrain*[_width * _height];
	_entities = new Entity*[_width * _height];

	for (int i = 0; i < _width; i++) {
		for (int j = 0; j < _width; j++) {
			_terrain[idx(i, j)] = nullptr;
			_entities[idx(i, j)] = nullptr;
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
			ClearTerrain(i, j);
			ClearEntity(i, j);
		}
	}

	delete _terrain;
}

Entity* World::GetCell(int i, int j)
{
	if (GetEntity(i, j))
		return GetEntity(i, j);

	return GetTerrain(i, j);
}

const Entity* World::GetCell(int i, int j) const
{
	if (GetEntity(i, j))
		return GetEntity(i, j);

	return GetTerrain(i, j);
}

void World::SetTerrain(int x, int y, Terrain* e)
{
	ClearTerrain(x, y);

	_terrain[idx(x, y)] = e;

	if (e) {
		e->SetPos(this, x, y);
	}
}

void World::SetEntity(int x, int y, Entity* e)
{
	ClearEntity(x, y);

	_entities[idx(x, y)] = e;

	if (e) {
		e->SetPos(this, x, y);
	}

	if (e->GetType() == EntityType::Player)
	{
		Player* p = static_cast<Player*>(e);

		int num = p->GetNum();

		assert(_players[num] == nullptr || _players[num] == p);

		_players[num] = p;
	}
}

void World::ClearTerrain(int x, int y)
{
	Entity* e = GetTerrain(x, y);

	if (e)
	{
		delete e;
	}

	_terrain[idx(x, y)] = nullptr;
}

void World::ClearEntity(int x, int y)
{
	Entity* e = GetEntity(x, y);

	if (e)
	{
		delete e;
	}

	_entities[idx(x, y)] = nullptr;
}

Player* World::GetPlayer(int i)
{
	assert(i >= 0 && i < 4);

	return _players[i];
}
