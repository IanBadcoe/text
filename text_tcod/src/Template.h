#pragma once

#include "Coord.h"

#include "mersenne.hpp"

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

class SkyslandTemplate : public Template {
public:
	// random numbers used to setup sinusoidal noise function
	SkyslandTemplate(int cx, int cy, int rad, uint32_t seed);

	virtual Terrain* TerrainForCell(Coord pos);

private:
	float Noise(const Coord& pos);

	int _cx;
	int _cy;
	int _rad;

	float _phases[4];
	float _wavelengths[4];
};

class TestTemplate : public Template {
	// Inherited via Template
	virtual Terrain * TerrainForCell(Coord pos) override;
};