#pragma once

#include "Entity.h"

class Terrain : public Entity {
public:
	Terrain(std::istringstream& in);
	Terrain(EntityType et, SerialiseOrder so, bool is_walkable, bool is_transparent) :
		Entity(et, so),
		_is_walkable(is_walkable),
		_is_transparent(is_transparent) {
	}

	bool IsWalkable() const { return _is_walkable; }
	bool IsTransparent() const { return _is_transparent; }

	// Inherited via ISerialisable
	virtual void SerialiseTo(std::ostringstream& out) const override = 0;

	virtual void CalcDisp(const Terrain* surrounds[8]) = 0;
	virtual bool DrawCompatWith(const Terrain* other) const = 0;

	static uint8_t GetWallCharacter(uint8_t code);
	static uint8_t GetBlockCharacter(uint8_t code);

private:
	static void InitWallData();
	static void InitBlockData();

	static std::map<uint8_t, uint8_t> s_wall_translate;
	static std::map<uint8_t, uint8_t> s_block_translate;

	bool _is_walkable;
	bool _is_transparent;
};

inline bool operator<(const TCODColor& a, const TCODColor& b) {
	// TCODColor constness is wrong, wrong, wrong...
	return const_cast<TCODColor&>(a).getValue() < const_cast<TCODColor&>(b).getValue();
}
