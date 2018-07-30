#ifndef GENERIC_MAP_GENERIC_MAP_H
#define GENERIC_MAP_GENERIC_MAP_H

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>

uint32_t getPrime(uint32_t min);

#define define_map_h(typeName, keyType, valueType) \
    typedef struct { \
        keyType key; \
        valueType value; \
        uint32_t hash; \
    } typeName ## _Cell_; \
    \
    typedef struct { \
        typeName ## _Cell_** cells; \
        uint32_t count; \
        uint32_t capacity; \
        uint32_t loadFactor; \
    } typeName; \
    \
    void* typeName ## Init(typeName* map); \
    void typeName ## Free(typeName* map); \
    bool typeName ## Add(typeName* map, keyType key, valueType value); \
    void typeName ## Set(typeName* map, keyType key, valueType value); \
    valueType typeName ## Get(typeName* map, keyType key); \
    bool typeName ## Remove(typeName* map, keyType key);

#define define_map_c(typeName, keyType, valueType, hashFn, compareFn, defaultValue) \
    void* typeName ## Init(typeName* map) { \
        map->capacity = 11; \
        map->count = 0; \
        map->loadFactor = 6; \
        return (map->cells = calloc(11, sizeof(*(map->cells)))); \
    } \
 \
    void typeName ## Free(typeName* map) { \
        for(uint32_t i = 0; i < map->capacity; i++) \
            if(map->cells[i] != NULL) \
                free(map->cells[i]); \
        free(map->cells); \
    } \
 \
    static void typeName ## Resize(typeName* map) { \
        int capacity = map->capacity; \
        map->capacity = getPrime(map->capacity * 2); \
        map->loadFactor = map->capacity / 2; \
        typeName ## _Cell_** old = map->cells; \
        typeName ## _Cell_** new = calloc(map->capacity, sizeof(*(map->cells))); \
 \
        for(int i = 0; i < capacity; i++) { \
            if(old[i] != NULL) { \
                uint32_t cell; \
                old[i]->hash = cell = hashFn(old[i]->key) % map->capacity; \
                while(new[cell] != NULL) { \
                    if(++cell > map->capacity) \
                        cell = 0; \
                } \
                new[cell] = old[i]; \
            } \
        } \
        free(old); \
        map->cells = new; \
    } \
 \
    bool typeName ## Add(typeName* map, keyType key, valueType value) { \
        uint32_t hash, cell; \
 \
        if(map->count == map->loadFactor) \
            typeName ## Resize(map); \
 \
        hash = cell = hashFn(key) % map->capacity; \
 \
        while(true) { \
            if(map->cells[cell] == NULL) { \
                typeName ## _Cell_* newCell = malloc(sizeof(typeName ## _Cell_)); \
                newCell->key = key; \
                newCell->value = value; \
                newCell->hash = hash; \
                map->cells[cell] = newCell; \
                map->count++; \
                return true; \
            } else if(map->cells[cell]->hash == hash && compareFn(map->cells[cell]->key, key) == 0) \
                return false; \
            if(++cell == map->capacity) \
                cell = 0; \
        } \
 \
        return false; \
    } \
 \
    void typeName ## Set(typeName* map, keyType key, valueType value) { \
        uint32_t hash, cell; \
 \
        if(map->count == map->loadFactor) \
            typeName ## Resize(map); \
 \
        cell = hash = hashFn(key) % map->capacity; \
 \
        while(true) { \
            if(map->cells[cell] == NULL) { \
                typeName ## _Cell_* newCell = malloc(sizeof(typeName ## _Cell_)); \
                newCell->key = key; \
                newCell->value = value; \
                newCell->hash = hash; \
                map->cells[cell] = newCell; \
                map->count++; \
                break; \
            } else if(map->cells[cell]->hash == hash && compareFn(map->cells[cell]->key, key) == 0) { \
                map->cells[cell]->value = value; \
                break; \
            } \
            if(++cell == map->capacity) \
                cell = 0; \
        } \
    } \
 \
    valueType typeName ## Get(typeName* map, keyType key) { \
        uint32_t cell, hash; \
        cell = hash = hashFn(key) % map->capacity; \
 \
        while(true) { \
            if(map->cells[cell] == NULL) \
                break; \
 \
            if(map->cells[cell]->hash == hash && compareFn(map->cells[cell]->key, key) == 0) \
                return map->cells[cell]->value; \
 \
            if(++cell == map->capacity) \
                cell = 0; \
        } \
 \
        return defaultValue; \
    } \
 \
    bool typeName ## Remove(typeName* map, keyType key) { \
        int last = -1; \
        uint32_t start, cell, hash; \
 \
        cell = hash = hashFn(key) % map->capacity; \
 \
        while(true) { \
            if(map->cells[cell] == NULL) \
                return false; \
 \
            if(map->cells[cell]->hash == hash && compareFn(map->cells[cell]->key, key) == 0) \
                break; \
 \
            if(++cell == map->capacity) \
                cell = 0; \
        } \
 \
        start = cell++; \
        while(map->cells[cell] != NULL) { \
            if(map->cells[cell]->hash <= hash) \
                last = cell; \
 \
            if(++cell == map->capacity) \
                cell = 0; \
        } \
 \
        if(last != -1) { \
            map->cells[start] = map->cells[last]; \
            map->cells[last] = NULL; \
        } else \
            map->cells[start] = NULL; \
        map->count--; \
        return true; \
    }

#endif //GENERIC_MAP_GENERIC_MAP_H