#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_RED 12
#define MAX_GREEN 13
#define MAX_BLUE 14
#define noop

typedef struct Draw {
  long blue;
  long red;
  long green;
} Draw;

int exceeds_max(char *line) {
  char *chunk, *draw, *endptr;
  long qty;
  while ((chunk = strsep(&line, ";")) != NULL) {
    char *chunk_copy = chunk;
    while ((draw = strsep(&chunk_copy, ",")) != NULL) {
      qty = strtol(draw, &endptr, 10);
      switch (endptr[1]) {
      case 'b':
        if (qty > MAX_BLUE) {
          return 1;
        }
        break;
      case 'r':
        if (qty > MAX_RED) {
          return 1;
        }
        break;
      case 'g':
        if (qty > MAX_GREEN) {
          return 1;
        }
        break;
      default:
        printf("ERROR! char is %c", endptr[1]);
        return -1;
        break;
      }
    }
  }
  return 0;
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

    if (exceeds_max(line_copy) == 0) {
      total += game_id;
    }
  }
  printf("\ntotal: %ld\n", total);
}
