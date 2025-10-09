#include "parser.h"
#include <assert.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define IS_OPEN_BRACKET(variable) ((int)variable == 91)
#define IS_QUOTES(variable) ((int)variable == 34)
#define IS_COMMA(variable) ((int)variable == 44)
#define IS_CLOSED_BRACKET(variable) ((int)variable == 93)
#define IS_WHITESPACE(variable) ((int)variable == 32)
#define IS_NEWLINE(variable) ((int)variable == 10)
#define IS_T(variable) ((int)variable == 116)
#define IS_F(variable) ((int)variable == 102)
#define IS_DIGIT(variable) ((int)variable >= 48 && (int)variable <= 57)
#define STRING_SIZE 500

char* parseString (ParseState* state);
int* parseNumber (ParseState* state);
int* parseBool (ParseState* state);
List* parseList (ParseState* state) { // TODO: This function leaks memory rn
    List* list = createList ();
    assert (IS_OPEN_BRACKET (state->buffer[state->position]));
    state->position++;
    for (; state->position < strlen (state->buffer) &&
    !IS_CLOSED_BRACKET (state->buffer[state->position]);
    state->position++) { // TODO: doesn't fail on two commas or comma at the beggining
        int current = state->buffer[state->position];
        if (IS_QUOTES (current)) {
            char* string = parseString (state);
            addValueToList (list,
            createEntryValue (createSuperPrimitiveString (string, 0), SUPER_PRIMITIVE));
        }
        if (IS_DIGIT (current)) {
            int* number = parseNumber (state);
            addValueToList (list,
            createEntryValue (createSuperPrimitiveInt (*number), SUPER_PRIMITIVE));
        }
        if (IS_T (current) || IS_F (current)) {
            int* bool = parseBool (state);
            addValueToList (list,
            createEntryValue (createSuperPrimitiveBool (*bool), SUPER_PRIMITIVE));
        }
        assert (IS_COMMA (state->buffer[state->position]) ||
        IS_CLOSED_BRACKET (state->buffer[state->position]));
    }
    assert (IS_CLOSED_BRACKET (state->buffer[state->position - 1])); // TODO: Check if there is a less disgusting way of doing this
    return list;
}

char* parseString (ParseState* state) { // TODO: Change the string size to be dynamic
    assert (IS_QUOTES (state->buffer[state->position]));
    char* string = (char*)malloc (STRING_SIZE * sizeof (char));
    state->position++;
    uint32_t i = 0;
    for (; state->position < strlen (state->buffer) &&
    !IS_QUOTES (state->buffer[state->position]);
    state->position++) { // TODO: change strlen to be something more robust
        string[i++] = state->buffer[state->position];
    }
    state->position++;
    return string;
}

int* parseNumber (ParseState* state) {
    assert (IS_DIGIT (state->buffer[state->position]));
    int* number = (int*)malloc (sizeof (int));
    *number     = 0;
    for (; state->position < strlen (state->buffer) &&
    IS_DIGIT (state->buffer[state->position]);
    state->position++) { // TODO: change strlen to be something more robust
        *number *= 10;
        *number += (int)state->buffer[state->position] - 48;
    }

    return number;
}

int* parseBool (ParseState* state) {
    assert (IS_T (state->buffer[state->position]) || IS_F (state->buffer[state->position]));
    int* bool = (int*)malloc (sizeof (int));
    assert (strlen (state->buffer) - state->position >= 4);
    if (state->buffer[state->position] == 't' && state->buffer[state->position + 1] == 'r' &&
    state->buffer[state->position + 2] == 'u' && state->buffer[state->position + 3] == 'e') {
        *bool = 1;
        state->position += 4;
        return bool;
    }
    assert (strlen (state->buffer) - state->position >= 5);
    if (state->buffer[state->position] == 'f' && state->buffer[state->position + 1] == 'a' &&
    state->buffer[state->position + 2] == 'l' && state->buffer[state->position + 3] == 's' &&
    state->buffer[state->position + 4] == 'e') {
        *bool = 0;
        state->position += 5;
        return bool;
    }

    assert (0); // this wasn't a bool
    return bool;
}
