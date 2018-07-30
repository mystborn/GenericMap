# GenericMap

This project contains a pair of macros that will generate a strongly typed map type. To add this to your project you can just copy the `generic_map` files from the source folder, or add the whole source folder somewhere in your source tree.

## Defining a Type

In a header file, use the macro `map_define_h` to generate the type and function definitions. It has the following arguments:

| Argument | Description |
| --- | --- |
| typeName | The name of the generated type. This will also prefix all of the function names. |
| keyType | The type of the used to access map elements. |
| valueType | The type of the map elements. |

In the corresponding source file, use the macro `map_define_c` to generate the function implementations.

| Argument | Description |
| --- | --- |
| typeName | The name of the generated type. This will also prefix all of the function names. |
| keyType | The type of the used to access map elements. |
| valueType | The type of the map elements. |
| hashFn | A function that takes a `keyType` and returns a uint32_t that will be used as the hash code for the key. |
| compareFn | A function that takes two keys, compares them, and returns an int determining if they're equal. It should return 0 for equality. |
| defaultValue | The value to return when you try to access a key that doesn't exist. |

The first three arguments of `map_define_c` *MUST* be the same as those used in `map_define_h`.

```c
// map.h
#include "generic_map.h"

// The name SSMap means string to string map.
map_define_h(SSMap, const char*, const char*)



// map.c
#include <string.h>

#include "map.h"

// This is a file inside of the Examples folder.
// It provides an implementation of the fnv-1a algorithm,
// which is an efficient string hash.
// The function name is "fnv32", which you'll see below.
#include "fnv1a.h"

map_define_c(SSMap, const char*, const char*, fnv32, strcmp, NULL)
```

That's all you need to define the maps.

## Functions

The macros define the following functions to use the maps.

| Function | Description | Return |
| --- | --- | --- |
| \<typeName>Init(\<typeName>\* map) | Initializes the data needed for the map. Returns the result of `calloc` used to initialize the data buffer. | void \* |
| \<typeName>Free(\<typeName>\* map) | Frees the data used by the map. Does not free the map itself. | void |
| \<typeName>Add(\<typeName>\* map, \<keyType> key, \<valueType> value) | Adds the key and value to the map. Returns false if the key has already been added. | bool |
| \<typeName>Set(\<typeName>\* map, \<keyType> key, \<valueType> value) | Adds the key and value to the map. Overwrites the previous value if the key has already been added. | void |
| \<typeName>Get(\<typeName>\* map, \<keyType> key) | Gets the value associated with the specified key. Returns `defaultValue` if the key isn't found. | <valueType> |
| \<typeName>Remove(\<typeName>\* map, \<keyType> key) | Removes the key and the associated value from the map. Returns false if the key wasn't found. | bool |

```c
#include <stdio.h>
#include "map.h"

int main() {
    SSMap map;
    SSMapInit(&map);

    SSMapAdd(&map, "hello", "world");
    SSMapSet(&map, "cow", "moo");
    printf("hello: %s\n", SSMapGet(&map, "hello"));
    printf("cow: %s\n", SSMapGet(&map, "cow"));

    SSMapSet(&map, "hello", "player 1");
    printf("hello: %s\n", SSMapGet(&map, "hello"));

    SSMapRemove(&map, "hello");
    if(SSMapGet(&map, "hello") == NULL)
        printf("Removed hello from the map\n");

    SSMapFree(map);
}
```

## Fields

The only useful field of the generated type is the `count` field, which can be used to get the number of elements added to the map.

```c
#include <stdio.h>
#include "map.h"

int main() {
    SSMap map;
    SSMapInit(&map);

    printf("%d\n", map.count);
    SSMapAdd(&map, "hello", "world");
    printf("%d\n", map.count);

    SSMapFree(&map);
}
```