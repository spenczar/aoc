#include <stdio.h>
#include <stdlib.h>
#include "aoc_strings.h"
#include <time.h>
#include <limits.h>

struct MappingRange {
  long destination;
  long source;
  long length;
};

long apply_map(long id, struct MappingRange mapping) {
  // Returns -1 if the id is not in the mapping range.
  //printf("Mapping %ld into {%ld, %ld, %ld}\n", id, mapping.destination, mapping.source, mapping.length);
  
  if (id < mapping.source || id >= mapping.source + mapping.length) {
    return -1;
  }
  return mapping.destination + (id - mapping.source);
}

struct MappingRange parse_line(struct String* s) {
  struct StringsVector* parts = aoc_string_whitespace_split(s);
  struct MappingRange mr;
  char* tmp = aoc_string_raw_copy(aoc_sv_get(parts, 0));
  mr.destination = atol(tmp);
  free(tmp);
  tmp = aoc_string_raw_copy(aoc_sv_get(parts, 1));
  mr.source = atol(tmp);
  free(tmp);
  tmp = aoc_string_raw_copy(aoc_sv_get(parts, 2));
  mr.length = atol(tmp);
  free(tmp);

  return mr;
}

struct MappingSet {
  struct MappingRange* ranges;
  size_t length;
  size_t capacity;
};

struct MappingSet* new_mapping_set(size_t capacity) {
  struct MappingSet* ms = malloc(sizeof(struct MappingSet));
  ms->ranges = malloc(capacity * sizeof(struct MappingRange));
  ms->length = 0;
  ms->capacity = capacity;
  return ms;
};

void mapping_set_push(struct MappingSet* ms, struct MappingRange mr) {
  if (ms->length == ms->capacity) {
    ms->capacity *= 2;
    ms->ranges = realloc(ms->ranges, ms->capacity * sizeof(struct MappingRange));
  }
  ms->ranges[ms->length++] = mr;
}

long apply_map_set(long id, struct MappingSet* mapping_set) {
  for (size_t i = 0; i < mapping_set->length; i++) {
    long mapped = apply_map(id, mapping_set->ranges[i]);
    if (mapped != -1) {
      return mapped;
    }
  }
  return id;
}

struct InputData {
  long* seeds;
  size_t n_seeds;

  struct MappingSet** mapping_sets;
  size_t n_mapping_sets;
};


struct InputData* parse_input(FILE* f) {
  struct InputData* data = malloc(sizeof(struct InputData));
  data->mapping_sets = malloc(8*sizeof(struct MappingSet*));
  data->n_mapping_sets = 0;

  struct StringsVector* lines = aoc_readlines(f);

  // First line is the seeds
  struct String* line = aoc_sv_get(lines, 0);
  // skip the first word in the line, but the rest are long IDs
  struct StringsVector* seed_words = aoc_string_whitespace_split(line);

  data->n_seeds = seed_words->length - 1;
  data->seeds = malloc(data->n_seeds * sizeof(long));
  for (size_t i = 0; i < data->n_seeds; i++) {
    char* tmp = aoc_string_raw_copy(aoc_sv_get(seed_words, i + 1));
    data->seeds[i] = atol(tmp);
    free(tmp);
  }

  printf("Seeds loaded\n");

  // The rest of the lines are either blank, "headings" which contain
  // a colon and delimit the mapping sets, or mapping ranges.
  for (size_t i = 2; i < lines->length; i++) {
    struct String* line = aoc_sv_get(lines, i);
    if (line->length <= 1) {
      // Blank line
      continue;
    } else if (line->start[line->length - 2] == ':') {
      // Start of a new  mapping
      data->mapping_sets[data->n_mapping_sets] = new_mapping_set(16);
      data->n_mapping_sets++;
    } else {
      // Mapping range

      struct MappingRange mr = parse_line(line);

      mapping_set_push(data->mapping_sets[data->n_mapping_sets-1], mr);
    }
  }

  return data;
}

long min_location2(struct InputData* data) {
  long id, min = LONG_MAX;

  for (size_t i = 0; i < data->n_seeds; i += 2) {
    printf("Seed %ld\n", data->seeds[i]);
    printf("Seed range %ld\n", data->seeds[i+1]);
    for (long j = 0; j < data->seeds[i+1]; j++) {
      id = data->seeds[i] + j;
      for (size_t k = 0; k < data->n_mapping_sets; k++) {
	id = apply_map_set(id, data->mapping_sets[k]);
      }
      if (id < min) {
	printf("New min: %ld\n", id);
	min = id;
      }
    }
  }
  return min;
}


int main(int argc, char** argv) {
  clock_t start = clock(), diff;

  if (argc != 2) {
    printf("Usage: %s <input file>\n", argv[0]);
    return 1;
  }

  FILE *f = fopen(argv[1], "r");
  if (!f) {
    printf("Could not open %s\n", argv[1]);
    return 1;
  }

  struct InputData* data = parse_input(f);

  long min = min_location2(data);
  // Mark end time
  diff = clock() - start;
  printf("Time: %ld usec\n", diff);

  printf("Min location: %ld\n", min);
  return 0;
}
