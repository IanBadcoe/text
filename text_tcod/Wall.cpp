#include "Wall.h"

#include "Map.h"

DisplayChar Wall::s_strong_wall(L'#', TCOD_white);
DisplayChar Wall::s_weak_wall(L'#', TCOD_gray);