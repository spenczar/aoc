#include <stdlib.h>
#include <stdio.h>
#include "aoc_strings.h"

struct String* aoc_string_new(char *start, size_t length) {
  struct String *s = malloc(sizeof(struct String));
  s->start = start;
  s->length = length;
  return s;
}

char* aoc_string_raw_copy(struct String *s) {
  char *copy = malloc(s->length + 1);
  for (size_t i = 0; i < s->length; i++) {
    copy[i] = s->start[i];
  }
  copy[s->length] = '\0';
  return copy;
}

struct String* aoc_getline(FILE *f) {
  size_t capacity = 16;
  char *line = malloc(capacity);
  size_t length = 0;
  int c;
  while ((c = fgetc(f)) != EOF) {
    if (length == capacity) {
      capacity *= 2;
      line = realloc(line, capacity);
    }
    line[length++] = c;
    if (c == '\n') {
      break;
    }
  }
  return aoc_string_new(line, length);
}

struct StringsVector* aoc_string_split(struct String *s, char delim) {
  struct StringsVector *v = aoc_strings_vector_new(16);
  size_t start = 0;
  for (size_t i = 0; i < s->length; i++) {
    if (s->start[i] == delim) {
      aoc_strings_vector_push(v, aoc_string_new(s->start + start, i - start));
      start = i + 1;
    }
  }
  aoc_strings_vector_push(v, aoc_string_new(s->start + start, s->length - start));
  return v;
}

struct StringsVector* aoc_string_whitespace_split(struct String *s) {
  // Splits whitespace greedily: "  a  b  " -> ["a", "b"]
  struct StringsVector *v = aoc_strings_vector_new(16);
  size_t start = 0;
  for (size_t i = 0; i < s->length; i++) {
    if (s->start[i] == ' ' || s->start[i] == '\t') {
      if (i > start) {
	aoc_strings_vector_push(v, aoc_string_new(s->start + start, i - start));
      }
      start = i + 1;
    }
  }
  if (s->length > start) {
    aoc_strings_vector_push(v, aoc_string_new(s->start + start, s->length - start));
  }
  return v;
}

void aoc_string_free(struct String *s) {
	free(s);
}

struct StringsVector* aoc_strings_vector_new(size_t capacity) {
  struct StringsVector *v = malloc(sizeof(struct StringsVector));
  v->strings = malloc(capacity * sizeof(struct String*));
  v->length = 0;
  v->capacity = capacity;
  return v;
}

void aoc_strings_vector_push(struct StringsVector *v, struct String *s) {
  if (v->length == v->capacity) {
    v->capacity *= 2;
    v->strings = realloc(v->strings, v->capacity * sizeof(struct String*));
  }
  v->strings[v->length++] = s;
}

struct StringsVector* aoc_readlines(FILE *f) {
  struct StringsVector *v = aoc_strings_vector_new(16);
  struct String *s;
  while ((s = aoc_getline(f))->length > 0) {
    aoc_strings_vector_push(v, s);
  }
  return v;
}

struct String* aoc_sv_get(struct StringsVector *v, size_t i) {
	return v->strings[i];
}

