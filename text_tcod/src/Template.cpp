#include "precompiled.h"

#include "Template.h"

#include "Floor.h"
#include "Wall.h"
#include "World.h"
#include "ShadedVoid.h"

#include "Math.h"

void Template::Apply(World* w) {
	for (int i = 0; i < w->GetWidth(); i++) {
		for (int j = 0; j < w->GetHeight(); j++) {
			w->SetTerrain(Coord(i, j), TerrainForCell(Coord(i, j)));
		}
	}

	w->RecalcTerrainDisps();
}

Terrain* CircleTemplate::TerrainForCell(Coord pos) {
	int dx = pos._x - _cx;
	int dy = pos._y - _cy;

	int r2 = (dx * dx) + (dy * dy);

	double r = sqrt(r2);

	double d = r - _rad;

	if (d < _width && d > -_width) {
		return new Floor();
	}

	return new Wall(1000);
}


// random numbers used to setup sinusoidal noise function
SkyslandTemplate::SkyslandTemplate(int cx, int cy, int rad, uint32_t seed) :
	_cx(cx),
	_cy(cy),
	_rad(rad) {
	TCODRandom r(seed);

	for (int i = 0; i < 4; i++) {
		_phases[i] = r.getFloat(0, (float)_rad);
		if (i < 2) {
			_wavelengths[i] = r.getFloat(_rad / 16.0f, _rad / 8.0f);
		} else {
			_wavelengths[i] = r.getFloat(_rad / 8.0f, (float)_rad / 4);
		}
	}
}

Terrain* SkyslandTemplate::TerrainForCell(Coord pos) {
	float dx = (float)(pos._x - _cx);
	float dy = (float)(pos._y - _cy);

	float r2 = ((dx * dx) + (dy * dy)) / (_rad * _rad);
	float r4 = r2 * r2;

	float hyperb = (0.5f - r4 / (r4 + 1)) * 2;

	float height = hyperb + Noise(pos) / 2;

	if (height > 0.5) {
		return new Wall(1000);
	} else if (height > 0.25) {
		return new Wall(500);
	} else if (height > 0.0f) {
		return new Floor();
	} else if (height > -0.5f) {
		int depth = (int)(-height / 0.5f * 4);

		return new ShadedVoid(depth);
	}

	return nullptr;
}

float SkyslandTemplate::Noise(const Coord& pos) {
	float x0 = (pos._x + _phases[0]) / _wavelengths[0];
	float x1 = (pos._x + _phases[1]) / _wavelengths[1];
	float y0 = (pos._y + _phases[2]) / _wavelengths[2];
	float y1 = (pos._y + _phases[3]) / _wavelengths[3];

	return (sin(x0) + sin(x1) + sin(y0) + sin(y1)) / 4;
}

Terrain * TestTemplate::TerrainForCell(Coord pos) {
	pos = pos - Coord(100, 10);
	int i = pos._x / 4;
	int j = pos._y / 4;

	if (i >= 0 && i < 16 && j < 16 && j >= 0) {
		uint8_t code = i + 16 * j;

		int di = pos._x - i * 4;
		int dj = pos._y - j * 4;

		if (di != 3 && dj != 3) {
			if (di == 1 && dj == 1)
				return new Wall(1000);

			Coord offset(di - 1, dj - 1);

			for (int k = 0; k < 8; k++) {
				Coord c = Coord(0, 0).Step((Coord::Dir)k);

				if (c == offset) {
					if (code & 1 << k)
					{
						return new Wall(1000);
					}

					break;
				}

			}
		}
	}

	return new Floor();
}
