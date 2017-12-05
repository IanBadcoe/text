#pragma once

#include "Entity.h"

#include <algorithm>
#include <queue>

class World;
class Player;

class World {
public:
	World(int width, int height);

	~World();

	Terrain* GetTerrain(Coord pos) {
		return _terrain[idx(pos)];
	}

	const Terrain* GetTerrain(Coord pos) const {
		return _terrain[idx(pos)];
	}

	Actor* GetActor(Coord pos) {
		return _actors[idx(pos)];
	}

	const Actor* GetActor(Coord pos) const {
		return _actors[idx(pos)];
	}

	void SetTerrain(Coord pos, Terrain* e);

	void AddActor(Coord pos, Actor* e);
	void RemoveActor(Coord pos);

	int idx(Coord pos) const {
		return pos._x + pos._y * _width;
	}

	void ClearTerrain(Coord pos);

	void ClearEntity(Coord pos);

	int GetWidth() const {
		return _width;
	}

	int GetHeight() const {
		return _height;
	}

	bool IsWalkable(Coord pos) {
		Terrain* t = _terrain[idx(pos)];

		return t ? t->IsWalkable() : false;
	}

	Player* GetPlayer(int i);

	Coord ClampCoord(Coord c) {
		return Coord(std::min(std::max(c._x, 0), _width),
			std::min(std::max(c._y, 0), _height));
	}

	bool InRange(Coord pos) {
		return pos._x >= 0 && pos._x < _width
			&& pos._y >= 0 && pos._y < _height;
	}

	void QueueStep(Actor* a, float delay);

private:
	int _width;
	int _height;

	Terrain **_terrain;
	Actor **_actors;

	Player* _players[4];

	struct QueueEntry {
		QueueEntry(Actor* a, float time) : _a(a), _time(time) {}

		Actor* _a;
		float _time;

		bool operator<(const QueueEntry& rhs) const {
			// earliest first
			return _time > rhs._time;
		}
	};

	std::priority_queue<QueueEntry> _action_queue;
};
