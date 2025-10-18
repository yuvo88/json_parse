#include "parser.h"
#include "hashmap.h"
#include "superPrimitive.h"
#include <assert.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define IS_OPEN_BRACKET(variable) ((int)variable == 91)
#define IS_CLOSED_BRACKET(variable) ((int)variable == 93)
#define IS_OPEN_CURLY(variable) ((int)variable == 123)
#define IS_CLOSED_CURLY(variable) ((int)variable == 125)
#define IS_QUOTES(variable) ((int)variable == 34)
#define IS_COLON(variable) ((int)variable == 58)
#define IS_COMMA(variable) ((int)variable == 44)
#define IS_WHITESPACE(variable) ((int)variable == 32)
#define IS_NEWLINE(variable) ((int)variable == 10)
#define IS_T(variable) ((int)variable == 116)
#define IS_F(variable) ((int)variable == 102)
#define IS_POINT(variable) ((int)variable == 46)
#define IS_DIGIT(variable) ((int)variable >= 48 && (int)variable <= 57)
#define INITIAL_STRING_SIZE 512

typedef enum IntFloat { INT_NUMBER, FLOAT_NUMBER, NUMBER_ERROR } IntFloat;
typedef struct IntFloatReturn {
    IntFloat type;
    union {
        float floatNumber;
        int intNumber;
    };
} IntFloatReturn;
List* parseList (ParseState* state);
Hashmap* parseHashmap (ParseState* state);
char* parseString (ParseState* state);
IntFloatReturn parseNumber (ParseState* state);
uint8_t* parseBool (ParseState* state);
SuperPrimitive* parseSuperPrimitive (ParseState* state);

uint32_t parseJson (ParseState* state, EntryValue* parsedReturn) {
    assert(state != NULL);
    assert(parsedReturn != NULL);
    EntryValue* value;
    if (IS_OPEN_BRACKET (state->buffer[state->position])) {
        List* list = parseList (state);
        if (list == NULL) {
            return state->position;
        }

        value = createEntryValue (list, LIST);
    } else if (IS_OPEN_CURLY (state->buffer[state->position])) {
        Hashmap* map = parseHashmap (state);
        if (map == NULL) {
            return state->position;
        }

        value = createEntryValue (map, HASHMAP);
    } else {
        SuperPrimitive* superPrimitive = parseSuperPrimitive (state);
        if (superPrimitive == NULL) {
            return state->position;
        }
        value = createEntryValue (superPrimitive, SUPER_PRIMITIVE);
    }
    parsedReturn->value = value->value;
    parsedReturn->type = value->type;

    return 0;
}

SuperPrimitive* parseSuperPrimitive (ParseState* state) {
    assert(state != NULL);
    SuperPrimitive* superPrimitive;
    int current = state->buffer[state->position];
    if (IS_QUOTES (current)) {
        char* string = parseString (state);
        if (string == NULL) {
            return NULL;
        }
        superPrimitive = createSuperPrimitiveString (string, strlen (string)); // TODO: remove strlen
    } else if (IS_DIGIT (current)) {
        IntFloatReturn intFloatReturn = parseNumber (state);
        switch (intFloatReturn.type) {
        case INT_NUMBER:
            superPrimitive = createSuperPrimitiveInt (intFloatReturn.intNumber); // TODO: remove strlen
            break;
        case FLOAT_NUMBER:
            superPrimitive = createSuperPrimitiveFloat (intFloatReturn.floatNumber); // TODO: remove strlen
            break;
        case NUMBER_ERROR: return NULL; break;
        }
    } else if (IS_T (current) || IS_F (current)) {
        uint8_t* boolean = parseBool (state);
        if (boolean == NULL) {
            return NULL;
        }
        superPrimitive = createSuperPrimitiveBool (*boolean); // TODO: remove strlen
    } else {
        return NULL;
    }
    return superPrimitive;
}
EntryValue* parseEntryValue (ParseState* state) {
    assert(state != NULL);
    EntryValue* entryValue;
    int current = state->buffer[state->position];
    if (IS_QUOTES (current) || IS_DIGIT (current) || IS_T (current) || IS_F (current)) {
        SuperPrimitive* superPrimitive = parseSuperPrimitive (state);
        if (superPrimitive == NULL) {
            return NULL;
        }
        entryValue = createEntryValue (superPrimitive, SUPER_PRIMITIVE);
    } else if (IS_OPEN_CURLY (current)) {
        Hashmap* map = parseHashmap (state);
        if (map == NULL) {
            return NULL;
        }
        entryValue = createEntryValue (map, HASHMAP);
        state->position++;
    } else if (IS_OPEN_BRACKET (current)) {
        List* list = parseList (state);
        if (list == NULL) {
            return NULL;
        }
        entryValue = createEntryValue (list, LIST);
        state->position++;
    } else {
        return NULL;
    }

    return entryValue;
}
HashmapEntry* parseHashmapEntry (ParseState* state) {
    assert(state != NULL);
    HashmapEntry* entry = (HashmapEntry*)malloc (sizeof (HashmapEntry));
    SuperPrimitive* key;
    while (state->position < strlen (state->buffer) &&
    !IS_COLON (state->buffer[state->position])) {
        if (IS_WHITESPACE (state->buffer[state->position]) ||
        IS_NEWLINE (state->buffer[state->position])) {
            state->position++;
            continue;
        } else {
            key = parseSuperPrimitive (state);
            if (key == NULL) {
                return NULL;
            }
            continue;
        }
        state->position++;
    }

    state->position++;

    EntryValue* value;
    while (state->position < strlen (state->buffer) &&
    !IS_COMMA (state->buffer[state->position]) &&
    !IS_CLOSED_CURLY (state->buffer[state->position])) {
        if (IS_WHITESPACE (state->buffer[state->position]) ||
        IS_NEWLINE (state->buffer[state->position])) {
            state->position++;
            continue;
        } else {
            value = parseEntryValue (state);
            if (value == NULL) {
                return NULL;
            }
            continue;
        }
        state->position++;
    }
    entry->key   = key;
    entry->value = value;
    return entry;
}
Hashmap* parseHashmap (ParseState* state) {
    assert(state != NULL);
    if (!IS_OPEN_CURLY (state->buffer[state->position])) {
        return NULL;
    }
    Hashmap* hashmap = createHashmap ();
    HashmapEntry* entry;
    state->position++;
    while (state->position < strlen (state->buffer) &&
    !IS_CLOSED_CURLY (state->buffer[state->position])) {
        if (IS_WHITESPACE (state->buffer[state->position]) ||
        IS_NEWLINE (state->buffer[state->position])) {
            state->position++;
            continue;
        } else {
            entry = parseHashmapEntry (state);
            if (entry == NULL) {
                return NULL;
            }
            setHashmapEntry (hashmap, entry->key, entry->value);
            if (IS_COMMA (state->buffer[state->position])) {
                state->position++;
            }
            continue;
        }
        state->position++;
    }
    if (IS_COMMA(state->buffer[state->position - 1])) {
        return NULL;
    }
    if (!IS_CLOSED_CURLY (state->buffer[state->position])) {
        return NULL;
    }
    return hashmap;
}
List* parseList (ParseState* state) { // TODO: This function leaks memory rn
    if (!IS_OPEN_BRACKET (state->buffer[state->position])) {
        return NULL;
    }
    List* list = createList ();
    SuperPrimitive* superPrimitive;
    state->position++;
    while (state->position < strlen (state->buffer) &&
    !IS_CLOSED_BRACKET (state->buffer[state->position])) { // TODO: doesn't fail on two commas or comma at the beggining
        if (IS_WHITESPACE (state->buffer[state->position]) ||
        IS_NEWLINE (state->buffer[state->position])) {
            state->position++;
            continue;
        } else {
            EntryValue* value = parseEntryValue (state);
            if (value == NULL) {
                return NULL;
            }
            addValueToList (list, value);
            if (IS_COMMA (state->buffer[state->position])) {
                state->position++;
            }
            continue;
        }
        state->position++;
    }
    if (IS_COMMA(state->buffer[state->position - 1])) {
        return NULL;
    }
    if (!IS_CLOSED_BRACKET (state->buffer[state->position])) {
        return NULL;
    }
    return list;
}

char* parseString (ParseState* state) {
    assert(state != NULL);
    if (!IS_QUOTES (state->buffer[state->position])) {
        return NULL;
    }
    char* string        = (char*)malloc (INITIAL_STRING_SIZE * sizeof (char));
    uint32_t stringSize = INITIAL_STRING_SIZE;
    state->position++;
    uint32_t i = 0;
    for (; state->position < strlen (state->buffer) &&
    !IS_QUOTES (state->buffer[state->position]);
    state->position++) {
        if (i > stringSize) {
            stringSize *= 2;
            string = realloc (string, stringSize * sizeof (char));
        }
        string[i++] = state->buffer[state->position];
    }
    string[i] = '\0';
    state->position++;
    return string;
}

IntFloatReturn parseNumber (ParseState* state) {
    assert(state != NULL);
    if (!IS_DIGIT (state->buffer[state->position])) {
        IntFloatReturn ret = { .type = NUMBER_ERROR };
        return ret;
    }
    IntFloatReturn intFloatReturn = { .type = INT_NUMBER };
    float number                  = 0;
    for (; state->position < strlen (state->buffer) &&
    IS_DIGIT (state->buffer[state->position]);
    state->position++) { // TODO: change strlen to be something more robust
        number *= 10;
        number += (float)state->buffer[state->position] - 48;
    }

    if (IS_POINT (state->buffer[state->position])) {
        state->position++;
        float fraction = 0;
        float division = 10;
        for (; state->position < strlen (state->buffer) &&
        IS_DIGIT (state->buffer[state->position]);
        state->position++) { // TODO: change strlen to be something more robust
            fraction = (int)state->buffer[state->position] - 48;
            number += fraction / division;
        }
        intFloatReturn.type        = FLOAT_NUMBER;
        intFloatReturn.floatNumber = (float)number;
    } else {
        intFloatReturn.intNumber = (int)number;
    }

    return intFloatReturn;
}

uint8_t* parseBool (ParseState* state) {
    assert(state != NULL);
    if (!IS_T (state->buffer[state->position]) && !IS_F (state->buffer[state->position])) {
        return NULL;
    }
    uint8_t* boolean = (uint8_t*)malloc (sizeof (uint8_t));
    if (strlen (state->buffer) - state->position < 4) {
        return NULL;
    }
    if (state->buffer[state->position] == 't' && state->buffer[state->position + 1] == 'r' &&
    state->buffer[state->position + 2] == 'u' && state->buffer[state->position + 3] == 'e') {
        *boolean = 1;
        state->position += 4;
        return boolean;
    }
    if (strlen (state->buffer) - state->position < 5) {
        return NULL;
    }
    if (state->buffer[state->position] == 'f' && state->buffer[state->position + 1] == 'a' &&
    state->buffer[state->position + 2] == 'l' && state->buffer[state->position + 3] == 's' &&
    state->buffer[state->position + 4] == 'e') {
        *boolean = 0;
        state->position += 5;
        return boolean;
    }

    return NULL;
}
