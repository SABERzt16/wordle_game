#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define MAX_WORDS 6969
#define WORD_LENGTH 6

int wordCount = 0;
char words[MAX_WORDS][WORD_LENGTH] = {0};
const char *target = NULL;
char guess[WORD_LENGTH] = {0};

#define MAX_WORDS 6969
#define WORD_LENGTH 6

enum { NUH, GOOD, EXISTS };

typedef struct {
  char letter;
  int state;
} attempt_response;

attempt_response all[WORD_LENGTH - 1] = {0};

int Dictionary(char words[MAX_WORDS][WORD_LENGTH], const char *filename) {
  FILE *file = fopen(filename, "r");
  if (!file) {
    printf("[ERROR]: could not open %s\n", filename);
    return 0;
  }

  int count = 0;
  while (count < MAX_WORDS && fscanf(file, "%5s", words[count]) == 1)
    count++;

  fclose(file);
  return count;
}

bool is_valid_word(const char *guess) {
  for (int i = 0; i < wordCount; i++) {
    if (memcmp(guess, &words[i][0], 5) == 0) {
      return true;
    }
  }
  return false;
}

int feedback(const char *guess, const char *target, attempt_response out[5]) {
  if (!is_valid_word(guess)) return 1;
  char guess_copy[WORD_LENGTH] = {0};
  memcpy(guess_copy, guess, WORD_LENGTH);

  for (int i = 0; i < 5; i++) {
    int in_target = 0, in_guess = 0;

    for (int j = 0; j < 5; j++)
      if (target[j] == guess[i]) in_target++;

    for (int j = 0; j < 5; j++) 
      if (guess_copy[j] == guess[i]) 
        in_guess++;

    while (in_guess > in_target) {
      for (int j = 4; j >= 0; j--) {
        if (guess_copy[j] == guess[i]) {
          guess_copy[j] = ' ';
          in_guess--;
          break;
        }
      }
    }
  }
    
  for (int i = 0; i < 5; i++) {
    if (guess_copy[i] == target[i]) {
      out[i].state = GOOD;
    } else if (strchr(target, guess_copy[i])) {
      out[i].state = EXISTS;
    } else {
      out[i].state = NUH;
    }
    out[i].letter = guess[i];
  }

  return 0;
}

#define TO_LOWER(c) ((c) >= 'A' && (c) <= 'Z' ? (c) + 'a' - 'A' : (c))

int init_wordle() {
  srand(time(0));
  wordCount = Dictionary(words, "words.txt");
  if (wordCount == 0)
    return 1;

  target = words[rand() % wordCount];

  return 0;
}

int check_word() {

  if (strlen(guess) != 5) {
    printf("\033[A\033[K"); // Move up one line and clear it
    printf("|   must be 5 letter please   |\n");
    printf("|=============================|\n");
    return -1;
  }

  for (int g = 0; g < sizeof(guess) - 1; g++) {
    guess[g] = TO_LOWER(guess[g]);
  }

  if (feedback(guess, target, all)) {
    printf("\033[A\033[K"); // Move up one line and clear it
    printf("|     must be a valid word    |\n");
    printf("|=============================|\n");
    return -1;

  }

  if (strcmp(guess, target) == 0) {
    return 1;
  }

  return 0;
}
