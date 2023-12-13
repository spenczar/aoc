#include "aoc_strings.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct Cards {
  // A simple wrapper around 5 card values
  long cards[5];
};

struct Cards parse_cards(char *s) {
  struct Cards cards;
  int i;
  for (i = 0; i < 5; i++) {
    switch (s[i]) {
    case 'A':
      cards.cards[i] = 14;
      break;
    case 'K':
      cards.cards[i] = 13;
      break;
    case 'Q':
      cards.cards[i] = 12;
      break;
    case 'J':
      cards.cards[i] = 11;
      break;
    case 'T':
      cards.cards[i] = 10;
      break;
    default:
      cards.cards[i] = s[i] - '0';
    }
  }
  return cards;
}

char* cards_str(struct Cards cards) {
  char* s = malloc(6 * sizeof(char));
  int i;
  for (i = 0; i < 5; i++) {
    switch (cards.cards[i]) {
    case 14:
      s[i] = 'A';
      break;
    case 13:
      s[i] = 'K';
      break;
    case 12:
      s[i] = 'Q';
      break;
    case 11:
      s[i] = 'J';
      break;
    case 10:
      s[i] = 'T';
      break;
    default:
      s[i] = cards.cards[i] + '0';
    }
  }
  s[5] = '\0';
  return s;
}


enum HandType {
  HIGH_CARD,
  ONE_PAIR,
  TWO_PAIR,
  THREE_OF_A_KIND,
  FULL_HOUSE,
  FOUR_OF_A_KIND,
  FIVE_OF_A_KIND,
};

int compare_cards(const void *a, const void *b) {
  return *(long*)a - *(long*)b;
}

int compare_handtypes(const void *a, const void *b) {
  return *(enum HandType*)a - *(enum HandType*)b;
}

enum HandType hand_type_from_cards(struct Cards cards) {
  // Cards might be unsorted, so first sort a copy.
  long sorted_cards[5];
  memcpy(sorted_cards, cards.cards, 5 * sizeof(long));
  qsort(sorted_cards, 5, sizeof(long), compare_cards);

  // Now walk through and count the number of each unique card value.
  int counts[5];
  int i;
  for (i = 0; i < 5; i++) {
    counts[i] = 0;
  }

  int current_count = 0;
  int current_card = sorted_cards[0];
  for (i = 0; i < 5; i++) {
    if (sorted_cards[i] == current_card) {
      current_count++;
    } else {
      counts[current_count]++;
      current_count = 1;
      current_card = sorted_cards[i];
    }
  }
  counts[current_count]++;

  // Now we have a histogram of the number of each unique card value.
  // We can use this to determine the hand type.

  // Five of a kind
  if (counts[5] == 1) {
    return FIVE_OF_A_KIND;
  }
  if (counts[4] == 1) {
    return FOUR_OF_A_KIND;
  }
  if (counts[3] == 1 && counts[2] == 1) {
    return FULL_HOUSE;
  }
  if (counts[3] == 1) {
    return THREE_OF_A_KIND;
  }
  if (counts[2] == 2) {
    return TWO_PAIR;
  }
  if (counts[2] == 1) {
    return ONE_PAIR;
  }
  return HIGH_CARD;
};

struct Hand {
  enum HandType type;
  long bid;
  long rank;
  struct Cards cards;
};

struct Hand parse_line(struct String *s) {
  struct Hand h;
  struct StringsVector *parts = aoc_string_whitespace_split(s);
  char *tmp = aoc_string_raw_copy(aoc_sv_get(parts, 0));

  // Five characters that determine the cards.
  h.cards = parse_cards(tmp);
  free(tmp);

  // The second part is the bid.
  tmp = aoc_string_raw_copy(aoc_sv_get(parts, 1));
  h.bid = atol(tmp);
  free(tmp);

  // Learn the card type.
  h.type = hand_type_from_cards(h.cards);

  // Unknown until all hands are sorted by relative strength
  h.rank = -1;

  return h;
}

void print_hand(struct Hand h) {
  // Print cards, then, rank, then bid, then product of rank and bid
  char *cards = cards_str(h.cards);
  printf("%s %ld %ld %ld\n", cards, h.rank, h.bid, h.rank * h.bid);
  free(cards);
}

int compare_hands(const void *a, const void *b) {
  struct Hand *ha = (struct Hand*)a;
  struct Hand *hb = (struct Hand*)b;
  if (ha->type != hb->type) {
     return compare_handtypes(&ha->type, &hb->type);
  }

  // Compare ranks of each card, one by one
  for (size_t i = 0; i < 5; i++) {
    if (ha->cards.cards[i] != hb->cards.cards[i]) {
      return ha->cards.cards[i] - hb->cards.cards[i];
    }
  }

  // If we get here, the hands are equal.
  return 0;
}

int main(int argc, char** argv) {
  FILE *inputf;
  if (argc != 2) {
    printf("missing input file");
    return 1;
  }

  inputf = fopen(argv[1], "r");
  struct StringsVector *lines = aoc_readlines(inputf);
  fclose(inputf);

  struct Hand* hands = malloc(lines->length * sizeof(struct Hand));

  int i;
  for (i = 0; i < lines->length; i++) {
    hands[i] = parse_line(aoc_sv_get(lines, i));
  }

  qsort(hands, lines->length, sizeof(struct Hand), compare_hands);

  long total_winnings = 0;
  for (i = 0; i < lines->length; i++) {
    hands[i].rank = i + 1;
    print_hand(hands[i]);
    total_winnings += hands[i].rank * hands[i].bid;
  }

  printf("Total winnings: %ld\n", total_winnings);
  
  return 0;
}

