#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>



typedef struct PartNumber {
  size_t start_x;
  size_t end_x;
  size_t y;
  int value;
} PartNumber;

typedef struct PartsList {
  PartNumber *part_numbers;
  size_t num_parts;
  size_t max_parts;
} PartsList;

PartsList *new_parts_list(size_t size) {
  PartsList *parts_list = malloc(sizeof(PartsList));
  parts_list->num_parts = 0;
  parts_list->max_parts = size;
  parts_list->part_numbers = malloc(parts_list->max_parts * sizeof(PartNumber));
  return parts_list;
}

void print_parts_list(PartsList *parts_list) {
  for (size_t i = 0; i < parts_list->num_parts; i++) {
    PartNumber part_number = parts_list->part_numbers[i];
    printf("Part number %zu: %d at (%zu, %zu) to (%zu, %zu)\n", i, part_number.value, part_number.start_x, part_number.y, part_number.end_x, part_number.y);
  }
}

void free_parts_list(PartsList *parts_list) {
  free(parts_list->part_numbers);
  free(parts_list);
}

void append_part_number(PartsList *parts_list, PartNumber part_number) {
  if (parts_list->num_parts == parts_list->max_parts) {
    parts_list->max_parts *= 2;
    parts_list->part_numbers = realloc(parts_list->part_numbers, parts_list->max_parts * sizeof(PartNumber));
  }
  parts_list->part_numbers[parts_list->num_parts] = part_number;
  parts_list->num_parts++;
}

PartsList* adjacent_parts(PartsList *parts_list, size_t x, size_t y) {
  PartsList *result = new_parts_list(2);
  for (size_t i = 0; i < parts_list->num_parts; i++) {
    PartNumber part_number = parts_list->part_numbers[i];

    // Must be in same row, or adjacent
    if (!(part_number.y == y-1 || part_number.y == y || part_number.y == y+1)) {
      continue;
    }

    // If in the same row, then must either end with x-1 or start with x+1
    if (part_number.y == y) {
      if (!(part_number.end_x == x-1 || part_number.start_x == x+1)) {
	continue;
      }
    }

    // If in the adjacent row, then must overlap with x, or with x-1 or x+1 (for diagonals)
    if (part_number.y == y-1 || part_number.y == y+1) {
      if (!(part_number.start_x <= x+1 && part_number.end_x >= x-1)) {
	continue;
      }
    }
    // Found a match
    append_part_number(result, part_number);
  }
  return result;
}

PartsList* find_parts(FILE *inputf) {
  char *line = NULL;
  size_t linecap = 0;
  ssize_t linelen;
  size_t i = 0;
  size_t num_parts = 0;
  size_t max_parts = 100;

  bool in_number = false;
  PartsList *part_numbers = new_parts_list(100);

  PartNumber current_part_number;
  while ((linelen = getline(&line, &linecap, inputf)) > 0) {
    for (size_t j = 0; j < linelen; j++) {
      if (line[j] >= '0' && line[j] <= '9') {
	// Found a digit. Now find the end of the number.
	if (!in_number) {
	  current_part_number.start_x = j;
	  current_part_number.y = i;
	  in_number = true;
	}
      } else {
	if (in_number) {
	  current_part_number.end_x = j-1;
	  in_number = false;
	  current_part_number.value = atoi(line + current_part_number.start_x);
	  append_part_number(part_numbers, current_part_number);
	}
      }
    }
    i++;
  }
  rewind(inputf);
  return part_numbers;
}

  
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


long total_gear_ratio(FILE *inputf) {
  PartsList *parts_list = find_parts(inputf);
  print_parts_list(parts_list);
  long total_gear_ratio = 0;

  char *line = NULL;
  size_t linecap = 0;
  ssize_t linelen;
  size_t i = 0;
  while ((linelen = getline(&line, &linecap, inputf)) > 0) {
    for (size_t j = 0; j < linelen; j++) {
      // If this is an asterisk, check our parts list
      if (line[j] == '*') {
	// Check if there is an adjacent part number
	PartsList *ap = adjacent_parts(parts_list, j, i);
	printf("found asterisk at (%zu, %zu)\n", j, i);
	printf("adjacent parts:\n");
	print_parts_list(ap);
	printf("\n");
	if (ap->num_parts == 2) {
	  // Found two adjacent part numbers. Calculate the gear ratio.
	  PartNumber part1 = ap->part_numbers[0];
	  PartNumber part2 = ap->part_numbers[1];
	  long gear_ratio = part1.value * part2.value;
	  total_gear_ratio += gear_ratio;
	}
	free_parts_list(ap);
      }
    }
    i++;
  }
  rewind(inputf);
  return total_gear_ratio;
}



int main(int argc, char * argv[]) {
  if (argc != 2) {
    printf("missing input");
    return 1;
  }

  FILE *inputf  = fopen(argv[1], "r");

  long sum = total_gear_ratio(inputf);
  printf("sum is %ld\n", sum);
}
