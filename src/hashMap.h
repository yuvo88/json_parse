#ifndef __hashMap_h
#define __hashMap_h
#include <stddef.h>
#include <stdint.h>
#define KEY_SIZE 256
#define FIELD_DATA_SIZE 1024

typedef enum FieldTypes { VARCHAR } FieldTypes;
typedef enum ReturnCodes { SUCCESS, KEY_NOT_FOUND } ReturnCodes;
typedef struct HashMapEntryData {
    char data[FIELD_DATA_SIZE];
    FieldTypes type;
} FieldData;

typedef struct HashMapEntry {
    char key[KEY_SIZE];
    FieldData value;
    struct HashMapEntry* next;
} HashMapEntry;

typedef struct HashMap {
    HashMapEntry** entries;
    uint32_t count;
} HashMap;


ReturnCodes hashmapGetByKey (HashMap* map, const char* key, FieldData* returnMap);
ReturnCodes hashmapSetFieldData (HashMap* map, HashMapEntry* value);
uint32_t fnv1a (const char* data, size_t len);
#endif
