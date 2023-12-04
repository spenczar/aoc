#include <assert.h>
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

bool strmatch(char *have, char *want, size_t have_length, size_t want_length) {
  if (have_length < want_length) {
    return false;
  }
  for (size_t i = 0; i < want_length; i++) {
    if (have[i] != want[i]) {
      return false;
    }
  }
  return true;
}

void update_digits(int digits[2], int val) {
  if (digits[0] == -1) {
    digits[0] = val;
  }
  digits[1] = val;
}

int peek(char * target, size_t target_length, int val, char text_buffer[4], FILE *inputf, int digits[2]) {
  int read_length;
  read_length = fread(text_buffer, 1, target_length, inputf);
  assert(read_length > 0);

  if (strmatch(text_buffer, target, read_length, target_length)) {
    update_digits(digits, val);
  }

  return fseek(inputf, -read_length, SEEK_CUR);
}

int main(int argc, char* argv[]) {
  FILE *inputf;
  assert(argc == 2);

  inputf = fopen(argv[1], "r");

  if (inputf == NULL) {
    printf("open file error!");
    return errno;
  }

  char next_char;
  char text_buffer[4];
  size_t read_length;
  int digits[2];
  int total;
  digits[0] = -1;
  total = 0;
  while ((next_char = getc(inputf)) != EOF) {
    switch (next_char) {
    case '0'...'9':
      // Literal digit
      update_digits(digits, next_char - '0');
      break;

    case 'o':
      assert(peek("ne", 1, 2, text_buffer, inputf, digits) == 0);
      break;

    case 't':
      read_length = fread(text_buffer, 1, 4, inputf);
      if (strmatch("wo", text_buffer, read_length, 2)) {
        update_digits(digits, 2);
      } else if (strmatch("hree", text_buffer, read_length, 4)) {
        update_digits(digits, 3);
      }
      fseek(inputf, -read_length, SEEK_CUR);
      break;


    case 'f':
      read_length = fread(text_buffer, 1, 3, inputf);
      if (strmatch("our", text_buffer, read_length, 3)) {
        update_digits(digits, 4);
      } else if (strmatch("ive", text_buffer, read_length, 3)) {
        update_digits(digits, 5);
      }
      fseek(inputf, -read_length, SEEK_CUR);
      break;

    case 's':
      read_length = fread(text_buffer, 1, 4, inputf);
      if (strmatch("ix", text_buffer, read_length, 2)) {
        update_digits(digits, 6);
      } else if (strmatch("even", text_buffer, read_length, 4)) {
        update_digits(digits, 7);
      }
      fseek(inputf, -read_length, SEEK_CUR);
      break;

    case 'e':
      read_length = fread(text_buffer, 1, 4, inputf);
      if (strmatch("ight", text_buffer, read_length, 4)) {
        update_digits(digits, 8);
      }
      fseek(inputf, -read_length, SEEK_CUR);
      break;

    case 'n':
      read_length = fread(text_buffer, 1, 3, inputf);
      if (strmatch("ine", text_buffer, read_length, 3)) {
        update_digits(digits, 9);
      }
      fseek(inputf, -read_length, SEEK_CUR);
      break;

    case '\n':
      // Emit a result
      total += digits[0] * 10 + digits[1];
      digits[0] = -1;
      digits[1] = -1;
      break;
    }
  }

  printf("%d\n", total);
}
