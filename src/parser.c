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

char* parseString (ParseState* state);
float* parseNumber (ParseState* state);
uint8_t* parseBool (ParseState* state);

SuperPrimitive* parseSuperPrimitive (ParseState* state) {
    SuperPrimitive* superPrimitive;
    int current = state->buffer[state->position];
    if (IS_QUOTES (current)) {
        char* string = parseString (state);
        superPrimitive = createSuperPrimitiveString (string, strlen (string)); // TODO: remove strlen
    } else if (IS_DIGIT (current)) {
        float* number = parseNumber (state);
        superPrimitive = createSuperPrimitiveFloat (*number); // TODO: remove strlen
    } else if (IS_T (current) || IS_F (current)) {
        uint8_t* boolean = parseBool (state);
        superPrimitive = createSuperPrimitiveBool (*boolean); // TODO: remove strlen
    } else {
        assert (0);
    }
    return superPrimitive;
}
EntryValue* parseEntryValue (ParseState* state) {
    EntryValue* entryValue;
    int current = state->buffer[state->position];
    if (IS_QUOTES (current) || IS_DIGIT (current) || IS_T (current) || IS_F (current)) {
        SuperPrimitive* superPrimitive = parseSuperPrimitive (state);
        entryValue = createEntryValue (superPrimitive, SUPER_PRIMITIVE);
    } else if (IS_OPEN_CURLY (current)) {
        Hashmap* map = parseHashmap (state);
        entryValue   = createEntryValue (map, HASHMAP);
    } else if (IS_OPEN_BRACKET (current)) {
        List* list = parseList (state);
        entryValue = createEntryValue (list, LIST);
    } else {
        assert (0);
    }

    return entryValue;
}
HashmapEntry* parseHashmapEntry (ParseState* state) {
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
            continue;
        }
        state->position++;
    }
    entry->key   = key;
    entry->value = value;
    return entry;
}
Hashmap* parseHashmap (ParseState* state) {
    assert (IS_OPEN_CURLY (state->buffer[state->position]));
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
            setHashmapEntry (hashmap, entry->key, entry->value);
            continue;
        }
        state->position++;
    }
    assert (IS_CLOSED_CURLY (state->buffer[state->position]));
    return hashmap;
}
List* parseList (ParseState* state) { // TODO: This function leaks memory rn
    assert (IS_OPEN_BRACKET (state->buffer[state->position]));
    List* list = createList ();
    SuperPrimitive* superPrimitive;
    state->position++;
    while (state->position < strlen (state->buffer) &&
    !IS_CLOSED_BRACKET (state->buffer[state->position])) { // TODO: doesn't fail on two commas or comma at the beggining
        if (IS_WHITESPACE (state->buffer[state->position]) ||
        IS_NEWLINE (state->buffer[state->position])) {
            state->position++;
            continue;
        } else if (IS_COMMA (state->buffer[state->position])) {
            state->position++;
            continue;
        } else {
            addValueToList (list, parseEntryValue (state));
            continue;
        }
        state->position++;
    }
    assert (IS_CLOSED_BRACKET (state->buffer[state->position]));
    return list;
}

char* parseString (ParseState* state) {
    assert (IS_QUOTES (state->buffer[state->position]));
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

float* parseNumber (ParseState* state) {
    assert (IS_DIGIT (state->buffer[state->position]));
    float* number = (float*)malloc (sizeof (float));
    *number       = 0;
    for (; state->position < strlen (state->buffer) &&
    IS_DIGIT (state->buffer[state->position]);
    state->position++) { // TODO: change strlen to be something more robust
        *number *= 10;
        *number += (float)state->buffer[state->position] - 48;
    }

    if (IS_POINT (state->buffer[state->position])) {
        state->position++;
        float fraction = 0;
        float division = 10;
        for (; state->position < strlen (state->buffer) &&
        IS_DIGIT (state->buffer[state->position]);
        state->position++) { // TODO: change strlen to be something more robust
            fraction = (int)state->buffer[state->position] - 48;
            *number += fraction / division;
        }
    }

    return number;
}

uint8_t* parseBool (ParseState* state) {
    assert (IS_T (state->buffer[state->position]) || IS_F (state->buffer[state->position]));
    uint8_t* boolean = (uint8_t*)malloc (sizeof (uint8_t));
    assert (strlen (state->buffer) - state->position >= 4);
    if (state->buffer[state->position] == 't' && state->buffer[state->position + 1] == 'r' &&
    state->buffer[state->position + 2] == 'u' && state->buffer[state->position + 3] == 'e') {
        *boolean = 1;
        state->position += 4;
        return boolean;
    }
    assert (strlen (state->buffer) - state->position >= 5);
    if (state->buffer[state->position] == 'f' && state->buffer[state->position + 1] == 'a' &&
    state->buffer[state->position + 2] == 'l' && state->buffer[state->position + 3] == 's' &&
    state->buffer[state->position + 4] == 'e') {
        *boolean = 0;
        state->position += 5;
        return boolean;
    }

    assert (0); // this wasn't a boolean
    return boolean;
}
