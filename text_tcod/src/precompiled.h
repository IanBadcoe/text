#pragma once

#define WINDOWS_LEAN_AND_MEAN

// memory debugging
#if 0
#define _CRTDBG_MAP_ALLOC  
#include <stdlib.h>  
#include <crtdbg.h>  
#endif

#include <winsock2.h>
#include <windows.h>

#include <assert.h>

#include <set>
#include <algorithm>
#include <vector>
#include <queue>

#include <concrt.h>

#undef min
#undef max