#include <stdio.h>
#include <stdint.h>
#include <string.h>

uint32_t fnv1a(const char* data, size_t len) {
    uint32_t hash = 2166136261u;
    for (size_t i = 0; i < len; i++) {
        hash ^= (uint8_t)data[i];
        hash *= 16777619u;
    }
    return hash;
}

int main() {
    uint32_t min = UINT32_MAX;
    char best[8] = {0};

	uint32_t res = fnv1a(t, 3);
	printf("%u", res);

    return 0;
}
