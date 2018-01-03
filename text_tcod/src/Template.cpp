#include "precompiled.h"

#include "Template.h"

#include "World.h"
#include "Floor.h"
#include "Wall.h"
#include "Rock.h"
#include "ShadedVoid.h"

void Template::Apply(World* w) {
	for (int i = 0; i < w->GetWidth(); i++) {
		for (int j = 0; j < w->GetHeight(); j++) {
			w->SetTerrain(Coord(i, j), TerrainForCell(Coord(i, j)));
		}
	}

	PostProcess(w);

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

void CircleTemplate::PostProcess(World* w) {
}


// random numbers used to setup sinusoidal noise function
SkyslandTemplate::SkyslandTemplate(int cx, int cy, int rad, uint32_t seed) :
	_cx(cx),
	_cy(cy),
	_rad(rad),
	_random(seed),
	_noise(2, 0.5f, 2.0f, &_random, TCOD_noise_type_t::TCOD_NOISE_SIMPLEX) {
}

Terrain* SkyslandTemplate::TerrainForCell(Coord pos) {
	float dx = (float)(pos._x - _cx);
	float dy = (float)(pos._y - _cy);

	float r2 = ((dx * dx) + (dy * dy)) / (_rad * _rad);
//	float r4 = r2 * r2;  <-- for "tighter" island
//	float hyperb = (0.5f - r4 / (r4 + 1)) * 2;

	float hyperb = (0.5f - r2 / (r2 + 1)) * 2;

	float height = hyperb + Noise(pos, 0.1f) * 0.3f;

//  	if (height > 0.5) {
//  		return new Wall(1000);
//  	} else if (height > 0.25) {
//  		return new Wall(500);
	if (height > 0.35) {
		const float p_mineral = 0.75f;
		const float p_vein = 0.55f;

		float r = Noise(pos, 0.2f, 2000, true);
		float g = Noise(pos, 0.2f, 3000, true);
		float b = Noise(pos, 0.2f, 4000, true);

		if (r > p_mineral) {
			return new Rock(Rock::Type::RedOre);
		} if (r > p_vein) {
			return new Rock(Rock::Type::Earth);
		} else if (g > p_mineral) {
			return new Rock(Rock::Type::GreenOre);
		} else if (b > p_mineral) {
			return new Rock(Rock::Type::BlueOre);
		} else if (g > p_vein || b > p_vein) {
			return new Rock(Rock::Type::Soft);
		}

		// rock
		return new Rock(Rock::Type::Hard);
	} else if (height > 0.0f) {
		return new Floor();
	} else if (height > -0.5f) {
		int depth = (int)(-height / 0.5f * 4);

		return new ShadedVoid(depth);
	}

	return nullptr;
}

float SkyslandTemplate::Noise(Coord pos, float scale /* = 1.0f */, int offset /* = 0 */, bool turbulent /* = false */) {
	float f[] = { (pos._x + offset) * scale, pos._y * scale };

	if (turbulent)
	{
		return _noise.get(f);
	} else {
		return _noise.getTurbulence(f, 3);
	}
}

void SkyslandTemplate::PostProcess(World* w) {
	for (int i = 0; i < 4; i++) {
		w->EnsureBase(i);
	}
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

void TestTemplate::PostProcess(World* w) {
}
