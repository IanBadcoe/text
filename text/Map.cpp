#include "stdafx.h"

#include "Map.h"
#include "Entity.h"
#include "World.h"

DisplayChar Map::s_void(L' ', FOREGROUND_WHITE);

void Map::SetChar(int x, int y, const Entity* e) {
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
	EnsureBuffers(w * h);

	for (int i = 0; i < w; i++)
	{
		for (int j = 0; j < h; j++)
		{
			_char_buff[i + j * w] = _map[idx(i + x, j + y)]._dc._char;
			_attr_buff[i + j * w] = _map[idx(i + x, j + y)]._dc._attr;
		}
	}

	DWORD dummy;

	COORD c;
	c.X = 0;
	c.Y = 0;

	WriteConsoleOutputAttribute(console, _attr_buff, w * h, c, &dummy);
	WriteConsoleOutputCharacter(console, _char_buff, w * h, c, &dummy);
}

void Map::ReadWorld(const World& world, int x, int y, int w, int h) {
	for (int i = x; i < x + w; i++)
	{
		for (int j = y; j < y + h; j++)
		{
			SetChar(i, j, world.GetCell(i, j));
		}
	}
}
