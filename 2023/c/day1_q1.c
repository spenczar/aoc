#include <errno.h>
#include <stdio.h>
#include <stdlib.h>

int scan_line(char *line) {
  int digits[2], i;
  digits[0] = -1;
  for (i = 0; i < 1000; i++)  {
    switch (line[i]) {
    case 0:
      return digits[0] * 10 + digits[1];
    case '0' ... '9':
      if (digits[0] == -1) {
          digits[0] = line[i] - '0';
        }
        digits[1] = line[i] - '0';
      }
  }
  return -1;
}

int main(int argc, char *argv[0]) {
  FILE *inputf;
  char line[100];
  int number, total;

  if (argc != 2) {
    printf("missing input file");
    return 1;
  }
  inputf = fopen(argv[1], "r");

  if (inputf == NULL) {
    printf("open file error!");
    return errno;
  }

  total = 0;
  while (fgets(line, 100, inputf)) {
    number = scan_line(line);
    total += number;

  }

  printf("%d\n", total);
}
