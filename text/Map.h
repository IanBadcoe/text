#pragma once

class Entity;
class World;

#define	FOREGROUND_BLACK 0
#define FOREGROUND_YELLOW FOREGROUND_GREEN | FOREGROUND_RED
#define FOREGROUND_CYAN FOREGROUND_BLUE | FOREGROUND_GREEN
#define FOREGROUND_MAGENTA FOREGROUND_BLUE | FOREGROUND_RED
#define FOREGROUND_WHITE FOREGROUND_BLUE | FOREGROUND_GREEN | FOREGROUND_RED

#define	INTENSE_FOREGROUND_BLACK | FOREGROUND_INTENSITY
#define INTENSE_FOREGROUND_YELLOW FOREGROUND_GREEN | FOREGROUND_RED | FOREGROUND_INTENSITY
#define INTENSE_FOREGROUND_CYAN FOREGROUND_BLUE | FOREGROUND_GREEN | FOREGROUND_INTENSITY
#define INTENSE_FOREGROUND_MAGENTA FOREGROUND_BLUE | FOREGROUND_RED | FOREGROUND_INTENSITY
#define INTENSE_FOREGROUND_WHITE FOREGROUND_BLUE | FOREGROUND_GREEN | FOREGROUND_RED | FOREGROUND_INTENSITY

#define	BACKGROUND_BLACK 0
#define BACKGROUND_YELLOW BACKGROUND_GREEN | BACKGROUND_RED
#define BACKGROUND_CYAN BACKGROUND_BLUE | BACKGROUND_GREEN
#define BACKGROUND_MAGENTA BACKGROUND_BLUE | BACKGROUND_RED
#define BACKGROUND_WHITE BACKGROUND_BLUE | BACKGROUND_GREEN | BACKGROUND_RED

#define	INTENSE_BACKGROUND_BLACK | BACKGROUND_INTENSITY
#define INTENSE_BACKGROUND_YELLOW BACKGROUND_GREEN | BACKGROUND_RED | BACKGROUND_INTENSITY
#define INTENSE_BACKGROUND_CYAN BACKGROUND_BLUE | BACKGROUND_GREEN | BACKGROUND_INTENSITY
#define INTENSE_BACKGROUND_MAGENTA BACKGROUND_BLUE | BACKGROUND_RED | BACKGROUND_INTENSITY
#define INTENSE_BACKGROUND_WHITE BACKGROUND_BLUE | BACKGROUND_GREEN | BACKGROUND_RED | BACKGROUND_INTENSITY

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
	Map(int width, int height)
		: _width(width),
		  _height(height),
		  _frame(0),
		  _char_buff(nullptr),
		  _attr_buff(nullptr),
		  _buff_size(0)
	{
		_map = new MapChar[width * height];
	}
	~Map() {
		delete[] _map;
		delete[] _char_buff;
		delete[] _attr_buff;
	}

	DisplayChar GetChar(int i, int j) const {
		return _map[idx(i, j)]._dc;
	}

	void SetChar(int x, int y, const Entity* e);

	int idx(int x, int y) const {
		return x + y * _width;
	}

	void NextFrame() { _frame++; }

	void Draw(HANDLE console, int x, int y, int w, int h);

	void ReadWorld(const World& world, int x, int y, int w, int h);

private:
	void EnsureBuffers(size_t s) {
		if (s > _buff_size)
		{
			delete[] _char_buff;
			delete[] _attr_buff;

			_char_buff = new wchar_t[s];
			_attr_buff = new WORD[s];
			_buff_size = s;
		}
	}

	int _width;
	int _height;

	MapChar* _map;

	int _frame;

	static DisplayChar s_void;

	wchar_t* _char_buff;
	WORD* _attr_buff;
	size_t _buff_size;
};