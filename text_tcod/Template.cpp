#include "Template.h"

#include "Floor.h"
#include "Wall.h"
#include "World.h"

#include "Math.h"

void Template::Apply(World& w)
{
	for (int i = 0; i < w.GetWidth(); i++)
	{
		for (int j = 0; j < w.GetHeight(); j++)
		{
			w.SetTerrain(i, j, TerrainForCell(i, j));
		}
	}
}

Terrain* CircleTemplate::TerrainForCell(int x, int y)
{
	int dx = x - _cx;
	int dy = y - _cy;

	int r2 = (dx * dx) + (dy * dy);

	double r = sqrt(r2);

	double d = r - _rad;

	if (d < _width && d > -_width)
	{
		return new Floor();
	}

	return new Wall(1000);
}
