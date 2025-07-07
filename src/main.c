#include "hashMap.h"
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main() {
  HashMap hashmap = createHashmap();
  FieldData data = {.data = "something", .type = VARCHAR};
  FieldData data2 = {.data = "goodDay", .type = VARCHAR};
  int statusCode = hashmapSetFieldData(&hashmap, "1", data);
  printf("status: %d\n", statusCode);
  statusCode = hashmapSetFieldData(&hashmap, "2", data2);
  printf("status: %d\n", statusCode);

  FieldData returnData = {.data = "", .type = VARCHAR};
  statusCode = hashmapGetByKey(&hashmap, "1", &returnData);
  printf("1: %s\n", returnData.data);
  hashmapGetByKey(&hashmap, "2", &returnData);
  printf("2: %s\n", returnData.data);
  hashmapGetByKey(&hashmap, "3", &returnData);
  printf("3: %s\n", returnData.data);
  // FieldData data1 = {.data = "something", .type = VARCHAR};
  // HashMapEntry entry1 = {.key = "something", .next = NULL, .value = data1};
  // FieldData data2 = {.data = "something2", .type = VARCHAR};
  // HashMapEntry entry2 = {.key = "something", .next = NULL, .value = data2};
  // HashMapEntry **entries =
  //     (HashMapEntry **)(calloc(HASH_ARRAY_LENGTH, sizeof(HashMapEntry *)));
  // HashMap hashmap = {.count = 0, .entries = entries};
  // int statusCode = hashmapSetFieldData(&hashmap, &entry1);
  // statusCode = hashmapSetFieldData(&hashmap, &entry2);
  // FieldData some = {.data = "", .type = VARCHAR};
  // statusCode = hashmapGetByKey(&hashmap, "sldkfjslkdjf", &some);
  // printf("returned_data: %s, statusCode: %d\n", some.data, statusCode);
  // free(entries);
  //
  return 0;
}
