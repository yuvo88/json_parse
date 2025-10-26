#include "parser.h"
#include "arena.h"
#include "hashmap.h"
#include "readFile.h"
#include "superPrimitive.h"
#include <assert.h>
#include <math.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define IS_OPEN_BRACKET(variable) ((int)variable == 91)
#define IS_BACKSLASH(variable) ((int)variable == 92)
#define IS_CLOSED_BRACKET(variable) ((int)variable == 93)
#define IS_OPEN_CURLY(variable) ((int)variable == 123)
#define IS_CLOSED_CURLY(variable) ((int)variable == 125)
#define IS_QUOTES(variable) ((int)variable == 34)
#define IS_COLON(variable) ((int)variable == 58)
#define IS_COMMA(variable) ((int)variable == 44)
#define IS_WHITESPACE(variable) ((int)variable == 32)
#define IS_NEWLINE(variable) ((int)variable == 10)
#define IS_T(variable) ((int)variable == 116)
#define IS_U(variable) ((int)variable == 117)
#define IS_F(variable) ((int)variable == 102)
#define IS_N(variable) ((int)variable == 110)
#define IS_POINT(variable) ((int)variable == 46)
#define IS_MINUS(variable) ((int)variable == 45)
#define IS_DIGIT(variable) ((int)variable >= 48 && (int)variable <= 57)
#define IS_HEX_UPPERCASE(variable) ((int)variable >= 65 && (int)variable <= 71)
#define IS_HEX_LOWERCASE(variable) ((int)variable >= 97 && (int)variable <= 102)
#define IS_NUMBER(variable) (IS_MINUS (variable) || IS_DIGIT (variable))
#define INITIAL_STRING_SIZE 512

typedef enum HexParseResultEnum {
    PARSE_SUCCESS,
    PARSE_ERROR
} HexParseResultEnum;
typedef struct {
    HexParseResultEnum result;
    uint16_t value;
} HexParseResult;
uint8_t parseNull (FileBuffer* buffer);
List* parseList (Arena* arena, FileBuffer* buffer);
Hashmap* parseHashmap (Arena* arena, FileBuffer* buffer);
SuperPrimitive* parseString (Arena* arena, FileBuffer* buffer);
SuperPrimitive* parseNumber (Arena* arena, FileBuffer* buffer);
SuperPrimitive* parseBool (Arena* arena, FileBuffer* buffer);
SuperPrimitive* parseSuperPrimitive (Arena* arena, FileBuffer* buffer);

uint32_t parseJson (Arena* arena, FileBuffer* buffer, EntryValue* parsedReturn) {
    assert (buffer != NULL);
    assert (parsedReturn != NULL);
    EntryValue* value;
    if (IS_OPEN_BRACKET (getValue (buffer, 0))) {
        List* list = parseList (arena, buffer);
        if (list == NULL) {
            return getPosition (buffer);
        }

        value = createEntryValue (arena, list, LIST);
    } else if (IS_OPEN_CURLY (getValue (buffer, 0))) {
        Hashmap* map = parseHashmap (arena, buffer);
        if (map == NULL) {
            return getPosition (buffer);
        }

        value = createEntryValue (arena, map, HASHMAP);
    } else {
        SuperPrimitive* superPrimitive = parseSuperPrimitive (arena, buffer);
        if (superPrimitive == NULL) {
            return getPosition (buffer);
        }
        value = createEntryValue (arena, superPrimitive, SUPER_PRIMITIVE);
    }
    parsedReturn->value = value->value;
    parsedReturn->type  = value->type;

    return 0;
}

SuperPrimitive* parseSuperPrimitive (Arena* arena, FileBuffer* buffer) {
    assert (buffer != NULL);
    SuperPrimitive* superPrimitive;
    int current = getValue (buffer, 0);
    if (IS_QUOTES (current)) {
        superPrimitive = parseString (arena, buffer);
    } else if (IS_NUMBER (current)) {
        superPrimitive = parseNumber (arena, buffer);
    } else if (IS_T (current) || IS_F (current)) {
        superPrimitive = parseBool (arena, buffer);
    } else {
        return NULL;
    }
    if (superPrimitive == NULL) {
        return NULL;
    }
    return superPrimitive;
}
EntryValue* parseEntryValue (Arena* arena, FileBuffer* buffer) {
    assert (buffer != NULL);
    EntryValue* entryValue;
    int current = getValue (buffer, 0);
    if (IS_QUOTES (current) || IS_NUMBER (current) || IS_T (current) || IS_F (current)) {
        SuperPrimitive* superPrimitive = parseSuperPrimitive (arena, buffer);
        if (superPrimitive == NULL) {
            return NULL;
        }
        entryValue = createEntryValue (arena, superPrimitive, SUPER_PRIMITIVE);
    } else if (IS_OPEN_CURLY (current)) {
        Hashmap* map = parseHashmap (arena, buffer);
        if (map == NULL) {
            return NULL;
        }
        entryValue = createEntryValue (arena, map, HASHMAP);
        addToPosition (buffer, 1);
    } else if (IS_OPEN_BRACKET (current)) {
        List* list = parseList (arena, buffer);
        if (list == NULL) {
            return NULL;
        }
        entryValue = createEntryValue (arena, list, LIST);
        addToPosition (buffer, 1);
    } else if (IS_N (current)) {
        uint32_t retValue = parseNull (buffer);
        if (retValue == 0) {
            return NULL;
        }
        entryValue = createEntryValue (arena, NULL, NULL_VALUE);
    } else {
        return NULL;
    }

    return entryValue;
}
HashmapEntry* parseHashmapEntry (Arena* arena, FileBuffer* buffer) {
    assert (buffer != NULL);
    HashmapEntry* entry = (HashmapEntry*)arenaMalloc (arena, sizeof (HashmapEntry));
    SuperPrimitive* key;
    while (!isEndOfFile (buffer) && !IS_COLON (getValue (buffer, 0))) {
        if (IS_WHITESPACE (getValue (buffer, 0)) || IS_NEWLINE (getValue (buffer, 0))) {
            addToPosition (buffer, 1);
            continue;
        } else {
            key = parseSuperPrimitive (arena, buffer);
            if (key == NULL) {
                return NULL;
            }
            continue;
        }
        addToPosition (buffer, 1);
    }

    addToPosition (buffer, 1);

    EntryValue* value;
    while (!isEndOfFile (buffer) && !IS_COMMA (getValue (buffer, 0)) &&
    !IS_CLOSED_CURLY (getValue (buffer, 0))) {
        if (IS_WHITESPACE (getValue (buffer, 0)) || IS_NEWLINE (getValue (buffer, 0))) {
            addToPosition (buffer, 1);
            continue;
        } else {
            value = parseEntryValue (arena, buffer);
            if (value == NULL) {
                return NULL;
            }
            continue;
        }
        addToPosition (buffer, 1);
    }
    entry->key   = key;
    entry->value = value;
    return entry;
}
Hashmap* parseHashmap (Arena* arena, FileBuffer* buffer) {
    assert (buffer != NULL);
    if (!IS_OPEN_CURLY (getValue (buffer, 0))) {
        return NULL;
    }
    Hashmap* hashmap = createHashmap (arena);
    HashmapEntry* entry;
    addToPosition (buffer, 1);
    while (!isEndOfFile (buffer) && !IS_CLOSED_CURLY (getValue (buffer, 0))) {
        if (IS_WHITESPACE (getValue (buffer, 0)) || IS_NEWLINE (getValue (buffer, 0))) {
            addToPosition (buffer, 1);
            continue;
        } else {
            entry = parseHashmapEntry (arena, buffer);
            if (entry == NULL) {
                return NULL;
            }
            setHashmapEntry (arena, hashmap, entry->key, entry->value);
            if (IS_COMMA (getValue (buffer, 0))) {
                addToPosition (buffer, 1);
            }
            continue;
        }
        addToPosition (buffer, 1);
    }
    if (IS_COMMA (getValue (buffer, -1))) {
        return NULL;
    }
    if (!IS_CLOSED_CURLY (getValue (buffer, 0))) {
        return NULL;
    }
    return hashmap;
}
List* parseList (Arena* arena, FileBuffer* buffer) { // TODO: This function leaks memory rn
    if (!IS_OPEN_BRACKET (getValue (buffer, 0))) {
        return NULL;
    }
    List* list = createList (arena);
    SuperPrimitive* superPrimitive;
    addToPosition (buffer, 1);
    while (!isEndOfFile (buffer) && !IS_CLOSED_BRACKET (getValue (buffer, 0))) {
        if (IS_WHITESPACE (getValue (buffer, 0)) || IS_NEWLINE (getValue (buffer, 0))) {
            addToPosition (buffer, 1);
            continue;
        } else {
            EntryValue* value = parseEntryValue (arena, buffer);
            if (value == NULL) {
                return NULL;
            }
            addValueToList (arena, list, value);
            if (IS_COMMA (getValue (buffer, 0))) {
                addToPosition (buffer, 1);
            }
            continue;
        }
        addToPosition (buffer, 1);
    }
    if (IS_COMMA (getValue (buffer, -1))) {
        return NULL;
    }
    if (!IS_CLOSED_BRACKET (getValue (buffer, 0))) {
        return NULL;
    }
    return list;
}
uint8_t parseStringEscape (FileBuffer* buffer) {
    assert (buffer != NULL);
    if (!IS_BACKSLASH (getValue (buffer, 0))) {
        return 0;
    }
    char retChar = ' ';
    switch (getValue (buffer, 1)) {
    case '"': retChar = (uint8_t)'"'; break;
    case '/': retChar = (uint8_t)'/'; break;
    case 'b': retChar = 8; break;
    case 'f': retChar = 12; break;
    case 'n': retChar = 10; break;
    case 'r': retChar = 13; break;
    case 't': retChar = 9; break;
    case '\\': retChar = (uint8_t)'\\'; break;
    default: return 0; break;
    }
    addToPosition (buffer, 2);
    return retChar;
}
HexParseResult parseStringHexEscape (FileBuffer* buffer) {
    assert (buffer != NULL);
    if (!IS_U (getValue (buffer, 0))) {
        HexParseResult result = { .value = 0, .result = PARSE_ERROR };
        return result;
    }
    uint16_t escaped = 0;
    for (int i = 1; i <= 4; i++) {
        if (IS_HEX_LOWERCASE (getValue (buffer, i))) {
            escaped *= 16;
            escaped += getValue (buffer, i) - 87;
        } else if (IS_HEX_UPPERCASE (getValue (buffer, i))) {
            escaped *= 16;
            escaped += getValue (buffer, i) - 55;
        } else if (IS_NUMBER (getValue (buffer, i))) {
            escaped *= 16;
            escaped += getValue (buffer, i) - 48;
        } else {
            HexParseResult result = { .value = 0, .result = PARSE_ERROR };
            return result;
        }
    }
    addToPosition (buffer, 5);
    HexParseResult result = { .value = escaped, .result = PARSE_SUCCESS };
    return result;
}

SuperPrimitive* parseString (Arena* arena, FileBuffer* buffer) {
    assert (buffer != NULL);
    if (!IS_QUOTES (getValue (buffer, 0))) {
        return NULL;
    }
    char* string = (char*)arenaMalloc (arena, INITIAL_STRING_SIZE * sizeof (char));
    uint32_t stringSize = INITIAL_STRING_SIZE;
    addToPosition (buffer, 1);
    uint32_t i = 0;
    while (!isEndOfFile (buffer) && !IS_QUOTES (getValue (buffer, 0))) {
        if (i > stringSize) {
            string = arenaRealloc (arena, string, stringSize, stringSize * 2);
            stringSize *= 2;
        }
        if (IS_BACKSLASH (getValue (buffer, 0))) {

            if (getValue (buffer, 1) == 'u') {
                addToPosition (buffer, 1);
                HexParseResult parsedResult = parseStringHexEscape (buffer);
                if (parsedResult.result == PARSE_ERROR) {
                    return NULL;
                }
                string[i++] = parsedResult.value & 0xff;
                string[i++] = (parsedResult.value >> 8);
                continue;
            }
            uint8_t escaped = parseStringEscape (buffer);
            if (escaped == 0) {
                return NULL;
            }
            string[i++] = (char)escaped;
            continue;
        }
        string[i++] = getValue (buffer, 0);
        addToPosition (buffer, 1);
    }
    addToPosition (buffer, 1);
    SuperPrimitive* superPrimitive = createSuperPrimitiveString (arena, string, i);
    return superPrimitive;
}

SuperPrimitive* parseNormalNumber (Arena* arena, FileBuffer* buffer) {
    assert (buffer != NULL);
    int minus_factor = 1;
    if (IS_MINUS (getValue (buffer, 0))) {
        minus_factor = -1;
        addToPosition (buffer, 1);
    }
    if (!IS_DIGIT (getValue (buffer, 0))) {
        return NULL;
    }
    SuperPrimitive* superPrimitive;
    float number = 0;
    for (; !isEndOfFile (buffer) && IS_DIGIT (getValue (buffer, 0));
    addToPosition (buffer, 1)) {
        number *= 10;
        number += (float)getValue (buffer, 0) - 48;
    }

    if (IS_POINT (getValue (buffer, 0))) {
        addToPosition (buffer, 1);
        float fraction = 0;
        float division = 10;
        for (; !isEndOfFile (buffer) && IS_DIGIT (getValue (buffer, 0));
        addToPosition (buffer, 1)) {
            fraction = (int)getValue (buffer, 0) - 48;
            number += fraction / division;
        }
        superPrimitive = createSuperPrimitiveFloat (arena, number * minus_factor);
    } else {
        superPrimitive = createSuperPrimitiveInt (arena, number * minus_factor);
    }

    return superPrimitive;
}

SuperPrimitive* parseNumber (Arena* arena, FileBuffer* buffer) {
    assert (buffer != NULL);
    SuperPrimitive* firstNumber = parseNormalNumber (arena, buffer);
    assert (firstNumber->type == INTEGER || firstNumber->type == FLOAT);
    if (firstNumber == NULL) {
        return NULL;
    }

    if (getValue (buffer, 0) != 'e' && getValue (buffer, 0) != 'E') {
        return firstNumber;
    }

    addToPosition (buffer, 1);

    SuperPrimitive* secondNumber = parseNormalNumber (arena, buffer);
    float first                  = *(float*)firstNumber->value;
    float second                 = *(float*)secondNumber->value;
    if (firstNumber->type == INTEGER) {
        first = (float)*(int*)firstNumber->value;
    }
    if (secondNumber->type == INTEGER) {
        second = (float)*(int*)secondNumber->value;
    }
    float result = first * powf (10, second);
    return createSuperPrimitiveFloat (arena, result);
}

SuperPrimitive* parseBool (Arena* arena, FileBuffer* buffer) {
    assert (buffer != NULL);
    if (!IS_T (getValue (buffer, 0)) && !IS_F (getValue (buffer, 0))) {
        return NULL;
    }
    SuperPrimitive* superPrimitive;
    if (isEndOfFileAmount (buffer, 4)) {
        return NULL;
    }
    if (getValue (buffer, 0) == 't' && getValue (buffer, 1) == 'r' &&
    getValue (buffer, 2) == 'u' && getValue (buffer, 3) == 'e') {
        addToPosition (buffer, 4);
        return createSuperPrimitiveBool (arena, 1);
    }
    if (isEndOfFileAmount (buffer, 5)) {
        return NULL;
    }
    if (getValue (buffer, 0) == 'f' && getValue (buffer, 1) == 'a' &&
    getValue (buffer, 2) == 'l' && getValue (buffer, 3) == 's' &&
    getValue (buffer, 4) == 'e') {
        addToPosition (buffer, 5);
        return createSuperPrimitiveBool (arena, 0);
    }

    return NULL;
}

uint8_t parseNull (FileBuffer* buffer) {
    assert (buffer != NULL);
    if (!IS_N (getValue (buffer, 0))) {
        return 0;
    }
    if (isEndOfFileAmount (buffer, 4)) {
        return 0;
    }
    if (getValue (buffer, 0) == 'n' && getValue (buffer, 1) == 'u' &&
    getValue (buffer, 2) == 'l' && getValue (buffer, 3) == 'l') {
        addToPosition (buffer, 4);
        return 1;
    }
    return 0;
}
