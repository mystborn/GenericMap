#include <string.h>
#include "map.h"
#include "fnv1a.h"

define_map_c(SSMap, const char*, const char*, fnv32, strcmp, NULL)