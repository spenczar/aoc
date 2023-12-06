#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

size_t num_lines(FILE *inputf) {
  // scan the input file and count the number of lines
  size_t num_lines = 0;
  char *line = NULL;
  size_t linecap = 0;
  ssize_t linelen;
  while ((linelen = getline(&line, &linecap, inputf)) > 0) {
    num_lines++;
  }
  rewind(inputf);
  return num_lines;
}

size_t first_line_length(FILE *inputf) {
  // Find length of the first line
  char *line = NULL;
  size_t linecap = 0;
  ssize_t linelen;
  linelen = getline(&line, &linecap, inputf);
  rewind(inputf);
  return linelen;
}

bool **find_symbols(FILE *inputf) {
  // Scan the input file, looking for symbols which are not digits or
  // periods. Record the positions of such symbols by constructing an
  // MxN array of booleans.
  size_t N = num_lines(inputf);
  size_t M = first_line_length(inputf);
  bool **symbols = malloc(N * sizeof(bool *));
  for (size_t i = 0; i < N; i++) {
    symbols[i] = malloc(M * sizeof(bool));
  }

  char *line = NULL;
  size_t linecap = 0;
  ssize_t linelen;
  size_t i = 0;
  while ((linelen = getline(&line, &linecap, inputf)) > 0) {
    for (size_t j = 0; j < linelen; j++) {
      switch (line[j]) {
      case '.':
      case '\n':
      case '0'...'9':
	symbols[i][j] = false;
	break;
      default:
	symbols[i][j] = true;
	break;
      }
    }
    i++;
  }

  rewind(inputf);
  return symbols;
}

bool adjacent_to_symbol(bool **symbol_map, size_t i, size_t j, size_t N, size_t M) {
  // Is the position (i, j) adjacent to a symbol? Diagonals count
  // too. Take care with the edges.
  // Previous row:
  if (i > 0) {
    if (j > 0) {
      if (symbol_map[i-1][j-1]) {
	return true;
      }
    }
    if (symbol_map[i-1][j]) {
      return true;
    }
    if (j < M-1) {
      if (symbol_map[i-1][j+1]) {
	return true;
      }
    }
  }

  // Current row:
  if (j > 0) {
    if (symbol_map[i][j-1]) {
      return true;
    }
  }

  if (j < M-1) {
    if (symbol_map[i][j+1]) {
      return true;
    }
  }

  // Next row:
  if (i < N-1) {
    if (j > 0) {
      if (symbol_map[i+1][j-1]) {
	return true;
      }
    }
    if (symbol_map[i+1][j]) {
      return true;
    }
    if (j < M-1) {
      if (symbol_map[i+1][j+1]) {
	return true;
      }
    }
  }
  return false;
}

long sum_part_numbers(FILE *inputf) {
  long sum = 0;
  bool **symbols = find_symbols(inputf);
  size_t N = num_lines(inputf);
  size_t M = first_line_length(inputf);

  char *line = NULL;
  size_t linecap = 0;
  ssize_t linelen;

  size_t i = 0;

  char *number_start = NULL;
  bool in_number = false;
  bool is_part = false;
  while ((linelen = getline(&line, &linecap, inputf)) > 0) {
    for (size_t j = 0; j < linelen; j++) {
      if (!in_number) {
	// Is the current character a digit? If so, we're now in a number.
	if (line[j] >= '0' && line[j] <= '9') {
	  in_number = true;
	  number_start = &line[j];
	}
      }

      if (in_number) {
	// Have we stopped being in a number?
	if (line[j] < '0' || line[j] > '9') {
	  if (is_part) {
	    printf("part number is %.*s\n", (int)(line+j-number_start), number_start);
	    sum += strtol(number_start, NULL, 10);
	  } else {
	    printf("not a part number: %.*s\n", (int)(line+j-number_start), number_start);
	  }

	  in_number = false;
	  is_part = false;
	  number_start = NULL;
	} else {
	  // Are we adjacent to a symbol?
	  is_part = is_part || adjacent_to_symbol(symbols, i, j, N, M);
	}
      }
    }
    i++;
  }

  rewind(inputf);
  return sum;
}

int main(int argc, char * argv[]) {
  if (argc != 2) {
    printf("missing input");
    return 1;
  }

  FILE *inputf  = fopen(argv[1], "r");

  long sum = sum_part_numbers(inputf);
  printf("sum is %ld\n", sum);
}
