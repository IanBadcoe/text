#include "precompiled.h"

#include "World.h"

#include "Actor.h"
#include "Base.h"
#include "Floor.h"

World::World(Universe* u) :
	_width(0),
	_height(0),
	_dirty_terrain(false),
	_tcod_map(nullptr),
	_universe(u)
{
}

World::~World()
{
	for (int i = 0; i < _width; i++) {
		for (int j = 0; j < _height; j++) {
			ClearTerrain(Coord(i, j));
			ClearEntity(Coord(i, j));
		}
	}

	delete[] _terrain;
	delete[] _actors;
}

void World::SetSize(int width, int height) {
	Clear();

	_width = width;
	_height = height;

	_terrain = new Terrain*[_width * _height];
	_actors = new Actor*[_width * _height];

	_tcod_map = new TCODMap(_width, _height);

	for (int i = 0; i < _width; i++) {
		for (int j = 0; j < _width; j++) {
			_terrain[idx(Coord(i, j))] = nullptr;
			_actors[idx(Coord(i, j))] = nullptr;
			_tcod_map->setProperties(i, j, false, false);
		}
	}
}

void World::SetTerrain(Coord pos, Terrain* t)
{
	ClearTerrain(pos);

	_terrain[idx(pos)] = t;

	if (t) {
		t->SetPos(pos);
		_tcod_map->setProperties(pos._x, pos._y, false, t->IsWalkable());
	} else {
		_tcod_map->setProperties(pos._x, pos._y, false, false);
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
	_tcod_map->setProperties(pos._x, pos._y, false, false);

	_dirty_terrain = true;
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

	delete[] _terrain;
	delete[] _actors;
	delete _tcod_map;

	_width = 0;
	_height = 0;
}

bool World::ComputePath(Player* player, Coord from, Coord to, Path& output) const {
	output.Clear();

	TCODPath path(_tcod_map);

	if (!path.compute(from._x, from._y, to._x, to._y))
		return false;

	for (int i = 0; i < path.size(); i++) {
		Coord pos;

		path.get(i, &pos._x, &pos._y);

		output._path.push_back(pos);
	}

	return true;
}

bool World::ProcessWorldCommand(const Command& c) {
// 	static int i = 0;
// 	if (c._type == Command::Type::WorldCellClick) {
// 		AddActor(c._world_cell, new Label(i));
// 
// 		i = (i + 1) % 10;
// 
// 		return true;
// 	}

	return false;
}

void World::EnsureBase(int base_id) {
	int end = base_id + 4;

	for (int j = base_id; j < end; j++) {
		Coord c(_height / 2, _width / 2);
		Coord::Dir d;
		int size;

		switch (j % 4) {
		case 0:
			d = Coord::Dir::West;
			size = _width / 2 - 2;
			break;

		case 1:
			d = Coord::Dir::North;
			size = _height / 2 - 2;
			break;

		case 2:
			d = Coord::Dir::East;
			size = _width / 2 - 2;
			break;

		case 3:
			d = Coord::Dir::South;
			size = _height / 2 - 2;
			break;
		}

		for (int k = 0; k < size; k++) {
			if (TryBase(base_id, c))
				return;

			c = c.Step(d);
		}
	}

	assert(false);
}

Base* World::GetBase(int base_id) {
	auto it = _base_map.find(base_id);

	if (it == _base_map.end())
		return nullptr;

	return it->second;
}

void World::RegisterBase(int base_id, Base* base) {
	assert(_base_map.find(base_id) == _base_map.end());

	_base_map[base_id] = base;
}

bool World::TryBase(int base_id, Coord c) {
	const Terrain* t = GetTerrain(c);

	if (!t || t->GetType() != EntityType::Floor)
		return false;

	// clear off an area around the base
	for (IterateNxN it(c, 5); !it.Ended(); it.Next()) {
		SetTerrain(it.Current(), new Floor());
	}

	Base* b = new Base(base_id);
	SetTerrain(c, b);

	for (int j = 0; j < 8; j++) {
		Coord d = c.Step((Coord::Dir)j);

		SetTerrain(d, new BaseEdge(b, (Coord::Dir)j));
	}

	RegisterBase(base_id, b);

	return true;
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

			const Terrain* surrounds[8] = { nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr };

			for (int k = 0; k < 8; k++) {
				Coord t = pos.Step((Coord::Dir)k);

				if (InRange(t)) {
					surrounds[k] = GetTerrain(t);
				}
			}

			ter->CalcDisp(surrounds);
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

	// bases first as BaseEdges need them to already exist
	for (int i = 0; i < _width * _height; i++) {
		if (_terrain[i] && _terrain[i]->GetType() == EntityType::Base)
		{
			_terrain[i]->SerialiseTo(out);
		}
	}

	for (int i = 0; i < _width * _height; i++) {
		if (_terrain[i] && _terrain[i]->GetType() != EntityType::Base) {
			_terrain[i]->SerialiseTo(out);
		}
	}
}

void World::SerialiseFrom(std::istringstream& in)
{
	Clear();

	int w, h;

	in >>= w;
	in >>= h;

	SetSize(w, h);

	int num_actors;
	int num_terrains;

	in >>= num_actors;
	in >>= num_terrains;

	CreatorArg ca(_universe, this);

	for (int i = 0; i < num_actors; i++) {
		Entity* entity = EntityCreator::VirtualSerialiseFrom(in, ca);

		assert(dynamic_cast<Actor*>(entity));

		AddActor(entity->GetPos(), static_cast<Actor*>(entity));
	}

	_tcod_map = new TCODMap(_width, _height);
	static Entity* last = nullptr;

	for (int i = 0; i < num_terrains; i++) {
		Entity* entity = EntityCreator::VirtualSerialiseFrom(in, ca);
		last = entity;

		assert(dynamic_cast<Terrain*>(entity));

		SetTerrain(entity->GetPos(), static_cast<Terrain*>(entity));
	}

	RecalcTerrainDisps();
}

