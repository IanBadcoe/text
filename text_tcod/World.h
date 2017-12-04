#pragma once

#include "Entity.h"

class World;

class World {
public:
	World(int width, int height) : _width(width), _height(height) {
		_terrain = new Entity*[_width * _height];
		_entities = new Entity*[_width * _height];

		for (int i = 0; i < _width; i++) {
			for (int j = 0; j < _width; j++) {
				_terrain[idx(i, j)] = nullptr;
				_entities[idx(i, j)] = nullptr;
			}
		}
	}

	~World() {
		for (int i = 0; i < _width; i++) {
			for (int j = 0; j < _height; j++) {
				ClearTerrain(i, j);
				ClearEntity(i, j);
			}
		}

		delete _terrain;
	}

	Entity* GetCell(int i, int j) {
		if (GetEntity(i, j))
			return GetEntity(i, j);

		return GetTerrain(i, j);
	}

	const Entity* GetCell(int i, int j) const {
		if (GetEntity(i, j))
			return GetEntity(i, j);

		return GetTerrain(i, j);
	}

	Entity* GetTerrain(int x, int y) {
		return _terrain[idx(x, y)];
	}

	const Entity* GetTerrain(int x, int y) const {
		return _terrain[idx(x, y)];
	}

	Entity* GetEntity(int x, int y) {
		return _entities[idx(x, y)];
	}

	const Entity* GetEntity(int x, int y) const {
		return _entities[idx(x, y)];
	}

	void SetTerrain(int x, int y, Entity* e) {
		ClearTerrain(x, y);

		_terrain[idx(x, y)] = e;

		if (e) {
			e->SetPos(this, x, y);
		}
	}

	void SetEntity(int x, int y, Entity* e) {
		ClearEntity(x, y);

		_entities[idx(x, y)] = e;

		if (e) {
			e->SetPos(this, x, y);
		}
	}

	int idx(int x, int y) const {
		return x + y * _width;
	}

	void ClearTerrain(int x, int y) {
		Entity* e = GetTerrain(x, y);
		
		if (e)
		{
			delete e;
		}

		_terrain[idx(x, y)] = nullptr;
	}

	void ClearEntity(int x, int y) {
		Entity* e = GetEntity(x, y);

		if (e)
		{
			delete e;
		}

		_entities[idx(x, y)] = nullptr;
	}

	int GetWidth() const {
		return _width;
	}

	int GetHeight() const {
		return _height;
	}

private:
	int _width;
	int _height;

	Entity **_terrain;
	Entity **_entities;
};
