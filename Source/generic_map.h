#ifndef GENERIC_MAP_GENERIC_MAP_H
#define GENERIC_MAP_GENERIC_MAP_H

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>

uint32_t fibHash(uint32_t hash, uint32_t shift);

#define define_map_h(typeName, keyType, valueType) \
    typedef struct { \
        bool active; \
        keyType key; \
        valueType value; \
        uint32_t hash; \
    } typeName ## _Cell_; \
    \
    typedef struct { \
        typeName ## _Cell_* cells; \
        uint32_t count; \
        uint32_t capacity; \
        uint32_t loadFactor; \
        uint32_t shift; \
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
        map->shift = 29; \
        map->capacity = 8; \
        map->count = 0; \
        map->loadFactor = 4; \
        return (map->cells = calloc(8, sizeof(typeName ## _Cell_))); \
    } \
 \
    void typeName ## Free(typeName* map) { \
        free(map->cells); \
    } \
 \
    static void typeName ## Resize(typeName* map) { \
        int capacity = map->loadFactor = map->capacity; \
        map->capacity = 1 << (32 - (--map->shift)); \
        typeName ## _Cell_* old = map->cells; \
        typeName ## _Cell_* new = calloc(map->capacity, sizeof(typeName ## _Cell_)); \
 \
        for(int i = 0; i < capacity; i++) { \
            if(old[i].active) { \
                uint32_t cell; \
                old[i].hash = cell = fibHash(hashFn(old[i].key), map->shift); \
                while(new[cell].active) { \
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
        hash = cell = fibHash(hashFn(key), map->shift); \
 \
        while(true) { \
            if(!map->cells[cell].active) { \
                map->cells[cell].active = true; \
                map->cells[cell].key = key; \
                map->cells[cell].value = value; \
                map->cells[cell].hash = hash; \
                map->count++; \
                return true; \
            } else if(map->cells[cell].hash == hash && compareFn(map->cells[cell].key, key) == 0) \
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
        hash = cell = fibHash(hashFn(key), map->shift); \
 \
        while(true) { \
            if(!map->cells[cell].active) { \
                map->cells[cell].active = true; \
                map->cells[cell].key = key; \
                map->cells[cell].value = value; \
                map->cells[cell].hash = hash; \
                map->count++; \
                break; \
            } else if(map->cells[cell].hash == hash && compareFn(map->cells[cell].key, key) == 0) { \
                map->cells[cell].value = value; \
                break; \
            } \
            if(++cell == map->capacity) \
                cell = 0; \
        } \
    } \
 \
    valueType typeName ## Get(typeName* map, keyType key) { \
        uint32_t cell, hash; \
        hash = cell = fibHash(hashFn(key), map->shift); \
 \
        while(true) { \
            if(!map->cells[cell].active) \
                break; \
 \
            if(map->cells[cell].hash == hash && compareFn(map->cells[cell].key, key) == 0) \
                return map->cells[cell].value; \
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
        hash = cell = fibHash(hashFn(key), map->shift); \
 \
        while(true) { \
            if(!map->cells[cell].active) \
                return false; \
 \
            if(map->cells[cell].hash == hash && compareFn(map->cells[cell].key, key) == 0) \
                break; \
 \
            if(++cell == map->capacity) \
                cell = 0; \
        } \
 \
        start = cell++; \
        while(map->cells[cell].active) { \
            if(map->cells[cell].hash <= hash) \
                last = cell; \
 \
            if(++cell == map->capacity) \
                cell = 0; \
        } \
 \
        if(last != -1) { \
            map->cells[start] = map->cells[last]; \
            map->cells[last].active = false; \
        } else \
            map->cells[start].active = false; \
        map->count--; \
        return true; \
    }

#endif //GENERIC_MAP_GENERIC_MAP_H