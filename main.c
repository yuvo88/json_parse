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

	char t[3] = {204, 36, 49};

	uint32_t res = fnv1a(t, 3);
	printf("%u", res);

    char test[4];
    for (uint16_t a = 0; a < 256; a++) {
        for (uint16_t b = 0; b < 256; b++) {
            for (uint16_t c = 0; c < 256; c++) {
                test[0] = a;
                test[1] = b;
                test[2] = c;
                test[3] = '\0';

                uint32_t h = fnv1a(test, 3);
                if (h < min) {
                    min = h;
                    memcpy(best, test, 3);
                }
            }
        }
    }

    printf("Lowest hash: %u\n", min);
    printf("From string: [%u,%u,%u]\n", (uint8_t)best[0], (uint8_t)best[1], (uint8_t)best[2]);

    return 0;
}
