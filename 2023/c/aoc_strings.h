#pragma once

#include <stdio.h>

struct String {
  char *start;
  size_t length;
};

/// Allocates a new String struct with the given start and length.
struct String* aoc_string_new(char *start, size_t length);

/// Copies the contents of the given String into a new null-terminated
/// string. The caller is responsible for freeing the returned string.
char* aoc_string_raw_copy(struct String *s);

/// Reads a line from the given file. The returned String does not
/// include the newline character. The caller is responsible for
/// freeing the returned String.
struct String* aoc_getline(FILE *f);

/// Splits the given String on the given delimiter. The returned
/// StringsVector contains the substrings between the delimiters. 
struct StringsVector* aoc_string_split(struct String *s, char delim);

/// Splits the given String on whitespace. The returned StringsVector
/// contains the substrings between the whitespace. Any consecutive
/// whitespace characters are treated as a single delimiter.
struct StringsVector* aoc_string_whitespace_split(struct String *s);

/// Free the memory associated with a string.
void aoc_string_free(struct String *s);

struct StringsVector {
  struct String **strings;
  size_t length;
  size_t capacity;
};

/// Allocates a new StringsVector with the given capacity.
struct StringsVector* aoc_strings_vector_new(size_t capacity);

/// Appends the given String to the StringsVector. If the StringsVector
/// is full, it will be resized.
void aoc_strings_vector_push(struct StringsVector *v, struct String *s);

/// Free the memory associated with a StringsVector.
struct StringsVector* aoc_readlines(FILE *f);

/// Returns the String at the given index in the StringsVector.
struct String* aoc_sv_get(struct StringsVector *v, size_t i);

