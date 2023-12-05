#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_RED 12
#define MAX_GREEN 13
#define MAX_BLUE 14
#define noop

typedef struct Triple {
  long blue_max;
  long red_max;
  long green_max;
} Triple;

long triple_power(Triple t) {
  return (t.blue_max * t.red_max * t.green_max);
}

Triple maxes(char *line) {
  char *chunk, *draw, *endptr;
  long qty;
  Triple max_counts = {0, 0, 0};
  while ((chunk = strsep(&line, ";")) != NULL) {
    char *chunk_copy = chunk;
    while ((draw = strsep(&chunk_copy, ",")) != NULL) {
      qty = strtol(draw, &endptr, 10);
      switch (endptr[1]) {
      case 'b':
        if (qty > max_counts.blue_max) {
          max_counts.blue_max = qty;
        }
        break;
      case 'r':
        if (qty > max_counts.red_max) {
          max_counts.red_max = qty;
        }
        break;
      case 'g':
        if (qty > max_counts.green_max) {
          max_counts.green_max = qty;
        }
        break;
      default:
        printf("ERROR! char is %c", endptr[1]);
        break;
      }
    }
  }
  return max_counts;
}

int main(int argc, char * argv[]) {
  if (argc != 2) {
    printf("missing input");
    return 1;
  }
  FILE *inputf  = fopen(argv[1], "r");

  long total = 0, game_id;
  char *line = NULL;
  size_t linecap = 0;
  ssize_t linelen;
  while ((linelen = getline(&line, &linecap, inputf)) > 0) {
    game_id = strtol(&line[5], NULL, 10);

    char *line_copy = line;
    // Advance past the colon
    line_copy = strstr(line_copy, ":");
    line_copy = line_copy + 2;

    Triple max_quantities = maxes(line_copy);
    total += triple_power(max_quantities);
  }
  printf("\ntotal: %ld\n", total);
}
