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

