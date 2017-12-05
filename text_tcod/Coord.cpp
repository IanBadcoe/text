#include "Coord.h"

int Coord::DX[8] = { 0, 1, 1, 1, 0, -1, -1, -1 };
int Coord::DY[8] = { 1, 1, 0, -1, -1, -1, 0, 1 };
Coord Coord::DC[8] = {
	Coord(DX[0], DY[0]),
	Coord(DX[1], DY[1]),
	Coord(DX[2], DY[2]),
	Coord(DX[3], DY[3]),
	Coord(DX[4], DY[4]),
	Coord(DX[5], DY[5]),
	Coord(DX[6], DY[6]),
	Coord(DX[7], DY[7])
};
