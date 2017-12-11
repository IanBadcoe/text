#include "precompiled.h"

#include "Floor.h"

#include "Map.h"

TCODColor Floor::s_floor_foreground(64, 64, 64);
TCODColor Floor::s_floor_background(32, 32, 32);
DisplayChar Floor::s_floor(L'.', Floor::s_floor_foreground, Floor::s_floor_background);
