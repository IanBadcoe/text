#pragma once

#include <assert.h>

class World;
class DisplayChar;

enum class EntityType {
	Player,
	Wall,
	Floor
};

class Entity {
public:
	Entity(EntityType t) : _type(t), _x(0), _y(0), _w(nullptr) {}

	virtual DisplayChar& Disp() const = 0;

	void SetPos(World* w, int x, int y) {
		assert(_w == nullptr);

		_x = x;
		_y = y;
		_w = w;
	}

	EntityType GetType() const { return _type; }

	int GetX() const { return _x; }
	int GetY() const { return _y; }

private:
	EntityType _type;
	int _x;
	int _y;
	World* _w;
};

class Terrain : public Entity {
public:
	Terrain(EntityType et, bool is_walkable, bool is_transparent) :
		Entity(et),
		_is_walkable(is_walkable),
		_is_transparent(is_transparent) {}

	bool IsWalkable() const { return _is_walkable; }
	bool IsTransparent() const { return _is_transparent; }

private:
	bool _is_walkable;
	bool _is_transparent;
};
