#pragma once

#include <assert.h>

class World;
class DisplayChar;

enum class EntityType {
	Base,
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

private:
	EntityType _type;
	int _x;
	int _y;
	World* _w;
};
