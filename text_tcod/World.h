#pragma once

#include "Entity.h"

class World;
class Player;

class World {
public:
	World(int width, int height);

	~World();

	Entity* GetCell(int i, int j);

	const Entity* GetCell(int i, int j) const;

	Terrain* GetTerrain(int x, int y) {
		return _terrain[idx(x, y)];
	}

	const Terrain* GetTerrain(int x, int y) const {
		return _terrain[idx(x, y)];
	}

	Entity* GetEntity(int x, int y) {
		return _entities[idx(x, y)];
	}

	const Entity* GetEntity(int x, int y) const {
		return _entities[idx(x, y)];
	}

	void SetTerrain(int x, int y, Terrain* e);

	void SetEntity(int x, int y, Entity* e);

	int idx(int x, int y) const {
		return x + y * _width;
	}

	void ClearTerrain(int x, int y);

	void ClearEntity(int x, int y);

	int GetWidth() const {
		return _width;
	}

	int GetHeight() const {
		return _height;
	}

	bool IsWalkable(int x, int y) {
		Terrain* t = _terrain[idx(x, y)];

		return t ? t->IsWalkable() : false;
	}

	Player* GetPlayer(int i);

private:
	int _width;
	int _height;

	Terrain **_terrain;
	Entity **_entities;

	Player* _players[4];
};
