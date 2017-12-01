#include "stdafx.h"

#include "Map.h"
#include "Entity.h"

DisplayChar Map::s_void(L' ', FOREGROUND_WHITE);

void Map::SetChar(int x, int y, Entity* e) {
	_map[idx(x, y)]._frame = _frame;

	if (e)
	{
		_map[idx(x, y)]._dc = e->Disp();
	}
	else
	{
		_map[idx(x, y)]._dc = s_void;
	}
}

void Map::Draw(HANDLE console, int x, int y, int w, int h) {
	for (int i = x; i < x + w; i++)
	{
		for (int j = y; j < y + h; j++)
		{
			COORD c;
			c.X = i;
			c.Y = j;

			DWORD dummy;

			WriteConsoleOutputAttribute(console, &_map[idx(i, j)]._dc._attr, 1, c, &dummy);
			WriteConsoleOutputCharacter(console, &_map[idx(i, j)]._dc._char, 1, c, &dummy);
		}
	}
}

