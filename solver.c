#include "wordle.c"
#include <stdio.h>
#include <string.h>

char pool[MAX_WORDS][WORD_LENGTH];
int pool_count = 0;

int match(const char *cand, const char *guess) {
    attempt_response sim[5];
    feedback(guess, cand, sim);

    for (int i = 0; i < 5; i++)
        if (sim[i].state != all[i].state)
            return 0;

    return 1;
}

void filter_pool(const char *guess) {
    int w = 0;
    for (int i = 0; i < pool_count; i++) {
        if (match(pool[i], guess)) {
            memcpy(pool[w], pool[i], WORD_LENGTH);
            w++;
        }
    }
    pool_count = w;
}

int pattern_code(attempt_response fb[5]) {
    int c = 0;
    for (int i = 0; i < 5; i++) {
        c = c * 3 + fb[i].state;
    }
    return c;
}

int entropy_score(const char *g) {
    int counts[243] = {0};
    attempt_response sim[5];

    for (int i = 0; i < pool_count; i++) {
        feedback(g, pool[i], sim);
        int code = pattern_code(sim);
        counts[code]++;
    }

    int score = 0;
    for (int i = 0; i < 243; i++) {
        if (counts[i] > 0) {
            score += counts[i] * counts[i];
        }
    }

    return score;
}

void best_entropy_guess(char *out) {
    int best = 2069676967;
    int idx = 0;

    for (int i = 0; i < pool_count; i++) {
        int s = entropy_score(pool[i]);
        if (s < best) {
            best = s;
            idx = i;
        }
    }

    memcpy(out, pool[idx], WORD_LENGTH);
}

int main() {

    if (init_wordle()) return 1;

    for (int i = 0; i < wordCount; i++)
        memcpy(pool[i], words[i], WORD_LENGTH);

    pool_count = wordCount;
    printf("|=============================|\n");
    printf("|     SOLVER BY TEAM FLAN     |\n");

    for (int turn = 1; turn <= 6; turn++) {

        char guess_local[WORD_LENGTH];

        if (turn == 1) {
            memcpy(guess_local, "brain", 6);
        } else {
            best_entropy_guess(guess_local);
        }
        memcpy(guess, guess_local, WORD_LENGTH);
        int r = check_word();

        printf("|=============================|\n");
        printf("|    Turn %d guess: ", turn);

        #define GREEN "\033[32m"
        #define YELLOW "\033[33m"
        #define GRAY "\033[90m"
        #define RED "\033[31m"
        #define RESET "\033[0m"

        for (int i = 0; i < 5; i++) {
            switch (all[i].state) {
            case NUH:
              printf(GRAY "%c" RESET, all[i].letter);
              break;
            case GOOD:
              printf(GREEN"%c"RESET, all[i].letter);
              break;
            case EXISTS:
              printf(YELLOW"%c"RESET, all[i].letter);
              break;
            }
        }
        printf("      |\n");
        

        if (r == 1) {
            printf("|=============================|\n");
            printf("|    SOLVED word was %s    |\n",target);
            printf("|=============================|\n");
            return 0;
        }

        filter_pool(guess_local);
        printf("|   %4.d candidates left      |\n", pool_count);
    }
    printf("|=============================|\n");
    printf("|  "RED"LOSER"RESET" word was was %s   |\n", target);
    printf("|=============================|\n");
    return 0;
}