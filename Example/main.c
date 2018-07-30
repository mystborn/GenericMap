#include <stdio.h>
#include <string.h>
#include "map.h"

int main(int argc, const char* argv[]) {
    SSMap map;
    SSMapInit(&map);
    SSMapAdd(&map, "world", "hello");
    SSMapAdd(&map, "hello", "world");
    printf("%s\n", SSMapGet(&map, "hello"));
    SSMapSet(&map, "hello", "myst");
    printf("%s\n", SSMapGet(&map, "hello"));
    SSMapFree(&map);
}