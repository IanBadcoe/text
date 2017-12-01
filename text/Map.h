#pragma once

class DisplayChar {
public:
	DisplayChar(wchar_t c, WORD a) : _char(c), _attr(a) {}

	wchar_t _char;
	WORD _attr;
};

class Map {
private:
	class MapChar {
	public:
		MapChar() : _dc(' ', 0), _frame(0) {}
		MapChar(DisplayChar dc, int frame) : _dc(dc), _frame(frame) {}

		DisplayChar _dc;
		uint64_t _frame;
	};

public:
	Map(int width, int height) : _width(width), _height(height), _frame(0) {
		_map = new MapChar[width * height];
	}

	DisplayChar GetChar(int i, int j) const {
		return _map[idx(i, j)]._dc;
	}

	void SetChar(int x, int y, DisplayChar dc) {
		_map[idx(x, y)]._frame = _frame;
		_map[idx(x, y)]._dc = dc;
	}

	int idx(int x, int y) const {
		return x + y * _width;
	}

	void NextFrame() { _frame++; }

private:
	int _width;
	int _height;

	MapChar* _map;

	int _frame;
};