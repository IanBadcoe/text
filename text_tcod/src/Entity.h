#pragma once

#include <assert.h>

#include "Coord.h"
#include "ISerialisable.h"

#include <sstream>

class World;
class DisplayChar;

enum class EntityType {
	Unknown,
	Player,
	Wall,
	Floor
};

class EntityCreator;

class Entity {
public:
	Entity(std::istringstream& in) : _w(nullptr)
	{
		in >>= _type;
		in >>= _pos;
	}
	Entity(EntityType t) : _type(t), _pos(0, 0), _w(nullptr) {}

	virtual DisplayChar Disp() const = 0;

	void SetPos(Coord pos) {
		_pos = pos;
	}
	EntityType GetType() const { return _type; }

	Coord GetPos() const { return _pos; }

    const World* GetWorld() const { return _w; }
    World* GetWorld() { return _w;  }

	void SetWorld(World* w) {
		assert(!w || !_w || w == _w);

		_w = w;
	}

	virtual void SerialiseTo(std::ostringstream& out) const = 0;

private:
	EntityType _type;
	Coord _pos;
	World* _w;
};

class EntityCreator {
public:
	typedef Entity* (*CreateFunc)(std::istringstream& in);

	EntityCreator(EntityType type, CreateFunc func) : _type(type), _func(func)
	{
		RegisterCreator(this);
	}

	EntityType _type;
	CreateFunc _func;

	static std::map<EntityType, EntityCreator::CreateFunc> s_creation_map;

	static Entity* VirtualSerialiseFrom(std::istringstream& in);
	static void RegisterCreator(const EntityCreator* ac);
};

class Terrain : public Entity {
public:
	Terrain(std::istringstream& in);
	Terrain(EntityType et, bool is_walkable, bool is_transparent) :
		Entity(et),
		_is_walkable(is_walkable),
		_is_transparent(is_transparent) {}

	bool IsWalkable() const { return _is_walkable; }
	bool IsTransparent() const { return _is_transparent; }

	// Inherited via ISerialisable
	virtual void SerialiseTo(std::ostringstream& out) const override = 0;

private:
	bool _is_walkable;
	bool _is_transparent;
};
