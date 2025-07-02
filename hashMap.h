#include <stdint.h>
#define KEY_SIZE 256
#define FIELD_DATA_SIZE 1024

typedef enum FieldTypes {
	VARCHAR
} FieldTypes;

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
	HashMapEntry* entries;
	uint32_t count;
} HashMap;


HashMapEntryData* get(HashMap* map, char key[KEY_SIZE]);
