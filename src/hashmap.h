#ifndef __hashmap_h
#define __hashmap_h
#include "superPrimitive.h"
#include <stdint.h>
#define ARRAY_INITIAL_LENGTH 256
typedef enum EntryType { SUPER_PRIMITIVE, HASHMAP, LIST } EntryType;
typedef enum HashmapReturnCodes { SUCCESS, KEY_NOT_FOUND } HashmapReturnCodes;
typedef struct EntryValue {
    void* value;
    EntryType type;
} EntryValue;
typedef struct HashmapEntry {
    EntryValue* value;
    SuperPrimitive* key;
    uint32_t originalHash;
    struct HashmapEntry* next;
} HashmapEntry;
typedef struct Hashmap {
    HashmapEntry** entries;
    uint32_t length;
} Hashmap;
typedef struct List {
    struct List* next;
    EntryValue* value;
} List;
Hashmap* createHashmap ();
List* createList ();
void addValueToList (List* list, EntryValue* value);
void printListln (List* list);
void freeEntryValue (EntryValue* entryValue);
void printEntryValue (EntryValue* entryValue, uint32_t spaceAmount);
void printEntryValueln (EntryValue* entryValue);
HashmapReturnCodes deleteEntryByKey (Hashmap* hashmap, SuperPrimitive* key);
void freeHashmap (Hashmap* hashmap);
void setHashmapEntry (Hashmap* hashmap, SuperPrimitive* key, EntryValue* value);
void printHashmapln (Hashmap* hashmap);
void printHashmapEntryln (HashmapEntry* entry);
void printSpaces (uint32_t amount);
HashmapReturnCodes
getValueByKey (Hashmap* hashmap, SuperPrimitive* key, HashmapEntry* returnValue);
void printHashmap (Hashmap* hashmap, uint32_t spaceAmount);
void printHashmapEntry (HashmapEntry* entry, uint32_t spaceAmount);
void printList (List* list, uint32_t spaceAmount);
EntryValue* createEntryValue (void* value, EntryType type);
#endif
