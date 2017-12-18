#pragma once

#include "Coord.h"
#include "ISerialisable.h"
#include "DisplayChar.h"

#include <sstream>

class World;
class DisplayChar;

enum class EntityType {
	Unknown,
	Player,
	Wall,
	Floor,
	ShadedVoid
};

class EntityCreator;

class Entity {
public:
	Entity(std::istringstream& in) : _w(nullptr)
	{
		in >>= _type;
		in >>= _pos;
		in >>= _dc;
	}
	Entity(EntityType t) : _type(t), _pos(0, 0), _w(nullptr) {}

	DisplayChar Disp() const { return _dc; }
	void SetDisplayChar(DisplayChar dc) { _dc = dc; }

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
	DisplayChar _dc;
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
