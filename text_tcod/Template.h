#pragma once

#include "Coord.h"

class World;
class Entity;
class Terrain;

class Template {
public:
	void Apply(World* w);

	virtual Terrain* TerrainForCell(Coord pos) = 0;
};

class CircleTemplate : public Template {
public:
	CircleTemplate(int cx, int cy, int rad, int width) :
		_cx(cx),
		_cy(cy),
		_rad(rad),
		_width(width) {}

	virtual Terrain* TerrainForCell(Coord pos);

private:
	int _cx;
	int _cy;
	int _rad;
	int _width;
};
