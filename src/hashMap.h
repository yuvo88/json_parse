#ifndef __hashMap_h
#define __hashMap_h
#include <stdint.h>
#include <stddef.h>
#define KEY_SIZE 256
#define FIELD_DATA_SIZE 1024

typedef enum FieldTypes {
	VARCHAR
} FieldTypes;
typedef enum ReturnCodes {
	SUCCESS,
	MISSING_KEY_ERROR,
	EMPTY_ENTRIES_ARRAY_ERROR,
	ENTRY_NOT_FOUND
}ReturnCodes;
typedef struct HashMapEntryData {
	char data[FIELD_DATA_SIZE];
	FieldTypes type;
} HashMapEntryData; 

typedef struct HashMapEntry {
	char key[KEY_SIZE];
	HashMapEntryData value;
	struct HashMapEntry* next;
} HashMapEntry;

typedef struct HashMap {
	HashMapEntry** entries;
	uint32_t count;
} HashMap;


int getFromHashmap(HashMap* map, const char* key, HashMapEntryData* returnMap);
uint32_t fnv1a(const char* data, size_t len);
#endif
