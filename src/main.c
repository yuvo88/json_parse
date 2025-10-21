#include "hashmap.h"

#include "parser.h"
#include "readFile.h"
#include <assert.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

int main (int argc, const char** argv) {
    assert (argc == 2);
    const char* filename = argv[1];
    FileBuffer* buffer = openFile(filename, 4096);
    for (;!isEndOfFile(buffer); addToPosition(buffer, 1)) {
        printf("%c", getValue(buffer, 0));
    }

    closeFile(buffer);
    
    return 0;
}
