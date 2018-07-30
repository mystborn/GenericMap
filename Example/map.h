#ifndef GENERIC_MAP_EXAMPLE_MAP_H
#define GENERIC_MAP_EXAMPLE_MAP_H

#include "../Source/generic_map.h"

define_map_h(SSMap, const char*, const char*)

#define SNAKE_CASE

define_map_h(si_map, const char*, int)

#undef SNAKE_CASE

#endif