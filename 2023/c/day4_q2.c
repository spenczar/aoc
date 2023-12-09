#include <stdio.h>
#include <stdlib.h>
#include "aoc_strings.h"
#include <time.h>

struct Card {
  int id;
  int winners[10];
  int scratches[25];
  int qty;
};



int main(int argc, char** argv) {

  // Mark start time
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

  struct StringsVector* lines = aoc_readlines(f);
  fclose(f);

  struct Card* cards = calloc(lines->length, sizeof(struct Card));
  
  for (size_t i = 0; i < lines->length; i++) {
    struct String *line = aoc_sv_get(lines, i);
    struct StringsVector *words = aoc_string_whitespace_split(line);
    // First word is "Card"
    // Then the ID, but with a trailing colon.
    char *id_str = aoc_string_raw_copy(aoc_sv_get(words, 1));
    cards[i].id = atoi(id_str);
    free(id_str);
    // Then 10 winners
    for (size_t j = 0; j < 10; j++) {
      cards[i].winners[j] = atoi(aoc_string_raw_copy(aoc_sv_get(words, 2 + j)));
    }
    // Then a pipe character
    // Then 25 scratches
    for (size_t j = 0; j < 25; j++) {
      cards[i].scratches[j] = atoi(aoc_string_raw_copy(aoc_sv_get(words, 13 + j)));
    }
    // Count the card itself
    cards[i].qty++;
    
    // Check each winner to see if its in the scratches
    int n_winners = 0;
    for (size_t j = 0; j < 10; j++) {
      int found = 0;
      for (size_t k = 0; k < 25; k++) {
	if (cards[i].winners[j] == cards[i].scratches[k]) {
	  found = 1;
	  // Increment qty for the winning card
	  n_winners++;
	  break;
	}
      }
    }

    // The n_winners following cards increase in quantity - one per
    // quantity of the present card.
    for (size_t j = 1; j <= n_winners; j++) {
      cards[i + j].qty += cards[i].qty;
    }
  }

  // Sum the quantities
  int sum = 0;
  for (size_t i = 0; i < lines->length; i++) {
    sum += cards[i].qty;
  }

  // Mark end time
  diff = clock() - start;

  printf("Sum: %d\n", sum);
  printf("Time: %ld usec\n", diff);
  
  return 0;
}
