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

int main(int argc, char * argv[]) {
  FILE *inputf;

  if (argc != 2) {
    printf("missing input");
    return 1;
  }

  inputf = fopen(argv[1], "r");

  long total = 0;
  int game_id;
  char *line = NULL;
  size_t linecap = 0;
  ssize_t linelen;
  char *chunk;
  while ((linelen = getline(&line, &linecap, inputf)) > 0) {
    game_id = atoi(&line[5]);

    char *line_copy = line;
    // Advance past the colon
    line_copy = strstr(line_copy, ":");
    line_copy = line_copy + 2;

    while ((chunk = strsep(&line_copy, ";")) != NULL) {
      char *draw;
      char *endptr;
      long qty;
      while ((draw = strsep(&chunk, ",")) != NULL) {
        qty = strtol(draw, &endptr, 10);
        switch (endptr[1]) {
        case 'b':
          if (qty > MAX_BLUE) {
            goto next_line;
          }
          break;
        case 'r':
          if (qty > MAX_RED) {
            goto next_line;
          }
          break;
        case 'g':
          if (qty > MAX_GREEN) {
            goto next_line;
          }
          break;
        default:
          printf("ERROR! char is %c", endptr[1]);
          return 1;
          break;
        }
      }
    }
    total += game_id;
 next_line:
    // ??? noop to get the goto to work OK
    printf("");
  }
  printf("\ntotal: %ld\n", total);
}
