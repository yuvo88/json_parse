#ifndef __hashmap_h
#define __hashmap_h
#include "superPrimitive.h"
#include <stdint.h>
#define ARRAY_INITIAL_LENGTH 256
typedef enum HashmapEntryType {
    SUPER_PRIMITIVE,
    HASHMAP,
    LIST
} HashmapEntryType;
typedef enum HashmapReturnCodes { SUCCESS, KEY_NOT_FOUND } HashmapReturnCodes;
typedef struct HashmapEntry {
    void* value;
    SuperPrimitive* key;
    HashmapEntryType type;
    uint32_t originalHash;
    struct HashmapEntry* next;
} HashmapEntry;
typedef struct Hashmap {
    HashmapEntry** entries;
    uint32_t length;
} Hashmap;
Hashmap* createHashmap ();
HashmapReturnCodes deleteEntryByKey (Hashmap* hashmap, SuperPrimitive* key);
void freeHashmap (Hashmap* hashmap);
void setHashmapEntry (Hashmap* hashmap, SuperPrimitive* key, void* value, HashmapEntryType type);
void printHashmapln (Hashmap* hashmap);
void printHashmapEntryln (HashmapEntry* entry);
HashmapReturnCodes
getValueByKey (Hashmap* hashmap, SuperPrimitive* key, HashmapEntry* returnValue);
void printHashmapEntryValueln (HashmapEntry* entry);
void printHashmap (Hashmap* hashmap, uint32_t spaceAmount);
void printHashmapEntry (HashmapEntry* entry, uint32_t spaceAmount);
void printHashmapEntryValue (HashmapEntry* entry, uint32_t spaceAmount);
#endif
