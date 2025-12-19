#include "wordle.c"
#include <stdio.h>

#define ATTEMPTS 6

#define GREEN "\x1b[42m\x1b[30m"  // green background
#define YELLOW "\x1b[43m\x1b[30m" // yellow background
#define GRAY "\x1b[47m\x1b[30m"   // gray background
#define RESET "\x1b[0m"

void petty_print() {
  printf("\033[A\033[K"); // Move up one line and clear it
  for (int i = 0; i < WORD_LENGTH - 1; i++) {

    switch (all[i].state) {
    case NUH:
      printf("|" GRAY "  %c  " RESET, all[i].letter);
      break;
    case GOOD:
      printf("|" GREEN "  %c  " RESET, all[i].letter);
      break;
    case EXISTS:
      printf("|" YELLOW "  %c  " RESET, all[i].letter);
      break;
    }
  }
  printf("|\n");
}

int main() {

  const char *attemp_str[] = {"attempts.", "attempt. "};

  if (init_wordle())
    return 1;

  printf("|=============================|\n");
  printf("|     WORDLE GAME :    |\n");
  printf("|=============================|\n");
  printf("|  Guess the 5-letter word.   |\n"
         "|    You have %d attempts.     |\n",
         ATTEMPTS);
  printf("|=============================|\n");

  int a = ATTEMPTS;
  while (a > 0) {
    printf("enter your guess: ");
    scanf("%s", guess);

    switch (check_word()) {
    case -1:
      continue;
    case 1:
      petty_print();
      printf("|=============================|\n");
      return 0;
    case 0:
      petty_print();
      break;
    }

    a--;
    printf("|=============================|\n");
    printf("|    You have %d %s     |\n", a, attemp_str[a == 1]);
    printf("|=============================|\n");
  }
  
  #define RED "\033[31m"
  printf("|           "RED"LOSER"RESET"             |\n");
  printf("| the corrrect word was %s |\n", target);
  printf("|=============================|\n");

  return 0;
}