#include "Player.h"

#include "Map.h"

TCODColor Player::s_foreground[4] = {
	TCODColor(128, 32, 32),
	TCODColor(32, 128, 32),
	TCODColor(32, 32, 128),
	TCODColor(32, 128, 128)
};

DisplayChar Player::s_player[4] = {
	DisplayChar('@', s_foreground[0]),
	DisplayChar('@', s_foreground[1]),
	DisplayChar('@', s_foreground[2]),
	DisplayChar('@', s_foreground[3])
};

DisplayChar& Player::Disp() const
{
	return s_player[_num];
}
