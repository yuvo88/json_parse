#include "arena.h"
#include "hashmap.h"

#include "parser.h"
#include "readFile.h"
#include <assert.h>
#include <stdint.h>
#include <string.h>
#include <unistd.h>
#define GET_KB(size) ((uint64_t)(size * 1024))
#define GET_MB(size) ((uint64_t)(GET_KB (size) * 1024))
#define GET_GB(size) ((uint64_t)(GET_MB (size) * 1024))

int main (int argc, const char** argv) {
    assert (argc == 2);
    const char* filename  = argv[1];
    Arena* arena          = createArena (GET_GB (2));
    FileBuffer* buffer    = openFile (arena, filename, 4096);
    EntryValue* value     = createEntryValue (arena, NULL, SUPER_PRIMITIVE);
    uint32_t return_value = parseJson (arena, buffer, value);
    if (!return_value) {
        printEntryValueln (value);
    }

    closeFile (buffer);
    destroyArena (arena);
    return 0;
}
