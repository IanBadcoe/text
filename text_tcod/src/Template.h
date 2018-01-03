#pragma once

class World;
class Entity;
class Terrain;
class Universe;
class Coord;

class Template {
public:
	void Apply(World* w);

	virtual Terrain* TerrainForCell(Coord pos) = 0;
	virtual void PostProcess(World* w) = 0;
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

	// Inherited via Template
	virtual void PostProcess(World* w) override;
};

class SkyslandTemplate : public Template {
public:
	// random numbers used to setup sinusoidal noise function
	SkyslandTemplate(int cx, int cy, int rad, uint32_t seed);

	virtual Terrain* TerrainForCell(Coord pos);

private:
	int _cx;
	int _cy;
	int _rad;

	float Noise(Coord pos, float scale = 1.0f, int offset = 0, bool turbulent = false);

	TCODRandom _random;
	TCODNoise _noise;

	// Inherited via Template
	virtual void PostProcess(World* w) override;
};

class TestTemplate : public Template {
	// Inherited via Template
	virtual Terrain* TerrainForCell(Coord pos) override;

	// Inherited via Template
	virtual void PostProcess(World* w) override;
};