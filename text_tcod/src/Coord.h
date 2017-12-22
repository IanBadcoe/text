#pragma once

class Coord {
public:
	Coord() : _x(0), _y(0) {}
	Coord(int x, int y) : _x(x), _y(y) {}

	enum class Dir {
		North,
		NorthEast,
		East,
		SouthEast,
		South,
		SouthWest,
		West,
		NorthWest
	};

	static int DX[8];
	static int DY[8];

	static Coord DC[8];

	typedef std::map<TCOD_keycode_t, Dir> KeyMapType;
	static KeyMapType KeyMap;

	Coord Step(Dir d) {
		return *this + DC[(int)d];
	}

	Coord operator+(const Coord& c) const {
		return Coord(_x + c._x, _y + c._y);
	}

	Coord operator-(const Coord& c) const {
		return Coord(_x - c._x, _y - c._y);
	}

    Coord operator/(int v) const {
        return Coord(_x / v, _y / 2);
    }

	bool operator==(const Coord& c) const {
		return _x == c._x && _y == c._y;
	}

	int _x;
	int _y;
};

class IterateNxN {
public:
	IterateNxN(Coord c, int n = 3) : _c(c), _n(n / 2), _ended(false) {
		// ambiguous for even numbers
		assert((n & 1) == 1);

		_current = _c - Coord(_n, _n);
	}

	bool Ended() const { return _ended; }

	Coord Current() { return _current; }

	void Next() {
		if (_current._x < _c._x + _n) {
			_current._x++;
		}
		else if (_current._y < _c._y + _n) {
			_current._x = _c._x - _n;
			_current._y++;
		}
		else {
			_ended = true;
		}
	}

private:
	Coord _c;
	Coord _current;
	int _n;
	bool _ended;
};

class IterateNxNSpiralOut {
public:
	IterateNxNSpiralOut(Coord c, int n = 3) : _current(c), _n(n / 2), _ended(false), _m(0), _o(0), _dir(Coord::Dir::South) {
		// ambiguous for even numbers
		assert((n & 1) == 1);
	}

	bool Ended() const { return _ended; }

	Coord Current() { return _current; }

	void Next() {
		if (_o < _m * 2) {
			_o++;
			_current = _current.Step(_dir);
		} else if (_o > 0 && _dir != Coord::Dir::West) {
			_dir = (Coord::Dir)(((int)_dir + 6) % 8);
			_o = 1;			// in this case we already stepped one...
			_current = _current.Step(_dir);
		} else if (_m <= _n) {
			_dir = Coord::Dir::South;
			_o = 1;
			_m++;
			_current = _current.Step(Coord::Dir::NorthWest);
			_current = _current.Step(_dir);
		} else {
			_ended = true;
		}
	}

private:
	Coord _current;
	int _n;
	bool _ended;
	int _m;
	int _o;
	Coord::Dir _dir;
};
