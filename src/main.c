#include "hashMap.h"
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main() {
  FieldData data1 = {.data = "something", .type = VARCHAR};
  HashMapEntry entry1 = {.key = "something", .next = NULL, .value = data1};
  FieldData data2 = {.data = "something2", .type = VARCHAR};
  HashMapEntry entry2 = {.key = "something", .next = NULL, .value = data2};
  HashMapEntry **entries = (HashMapEntry **)(malloc(sizeof(int) * UINT32_MAX));
  HashMap hashmap = {.count = 0, .entries = entries};
  int statusCode = hashmapSetFieldData(&hashmap, &entry1);
  statusCode = hashmapSetFieldData(&hashmap, &entry2);
  FieldData *some = (FieldData *)(malloc(sizeof(FieldData)));
  statusCode = hashmapGetByKey(&hashmap, "something", some);
  printf("returned_data: %s, statusCode: %d\n", some->data, statusCode);
  free(entries);
  free(some);

  return 0;
}
