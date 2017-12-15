#include "precompiled.h"

#include "World.h"

#include "Player.h"

World::World(int width, int height) : _width(width), _height(height), _dirty_terrain(false)
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

	_dirty_terrain = true;
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

void World::Clear()
{
	for (int i = 0; i < _width; i++)
	{
		for (int j = 0; j < _height; j++)
		{
			Coord p(i, j);

			ClearEntity(p);
			ClearTerrain(p);
		}
	}
}

void World::RecalcTerrainDisps() {
	if (!_dirty_terrain)
		return;

	_dirty_terrain = false;

	for (int i = 0; i < _width; i++) {
		for (int j = 0; j < _width; j++) {
			Coord pos(i, j);

			Terrain* ter = GetTerrain(pos);

			if (!ter)
				continue;

			uint8_t code = 0;

			for (int k = 0; k < 8; k++) {
				Coord t = pos.Step((Coord::Dir)k);

				if (InRange(t)) {
					Terrain* oth = GetTerrain(t);
					
					if (oth && ter->DrawCompatWith(oth)) {
						code |= 1 << k;
					}
				}
			}

			ter->CalcDisp(code);
		}
	}
}

void World::SerialiseTo(std::ostringstream& out) const
{
	out <<= _width;
	out <<= _height;

	int num_actors = 0;
	int num_terrains = 0;

	for (int i = 0; i < _width * _height; i++) {
		if (_actors[i]) num_actors++;
		if (_terrain[i]) num_terrains++;
	}

	out <<= num_actors;
	out <<= num_terrains;

	for (int i = 0; i < _width * _height; i++) {
		if (_actors[i])
		{
			_actors[i]->SerialiseTo(out);
		}
	}

	for (int i = 0; i < _width * _height; i++) {
		if (_terrain[i])
		{
			_terrain[i]->SerialiseTo(out);
		}
	}
}

void World::SerialiseFrom(std::istringstream& in)
{
	Clear();

	in >>= _width;
	in >>= _height;

	int num_actors;
	int num_terrains;

	in >>= num_actors;
	in >>= num_terrains;

	for (int i = 0; i < num_actors; i++) {
		Entity* entity = EntityCreator::VirtualSerialiseFrom(in);

		assert(dynamic_cast<Actor*>(entity));

		AddActor(entity->GetPos(), static_cast<Actor*>(entity));
	}

	for (int i = 0; i < _width * _height; i++) {
		Entity* entity = EntityCreator::VirtualSerialiseFrom(in);

		assert(dynamic_cast<Terrain*>(entity));

		SetTerrain(entity->GetPos(), static_cast<Terrain*>(entity));
	}

	RecalcTerrainDisps();
}

