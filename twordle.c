//Zachary Detton

#include <stdio.h>
#include <string.h>

#define MAX_GUESSES 6
#define WORD_LEN 5

/* Load a random 5-letter word from word.txt */
char* load_mystery_word() {
    FILE *file = fopen("mystery.txt", "r");
    if (!file) {
        static char default_word[] = "apple";
        printf("Error: word.txt not found. Using default word 'apple'.\n");
        return default_word;
    }

    char line[100];
    char words[100][WORD_LEN + 1];
    int count = 0;

    while (fgets(line, sizeof(line), file) && count < 100) {
        line[strcspn(line, "\r\n")] = '\0';   // remove newline
        if (strlen(line) == WORD_LEN) {
            strcpy(words[count], line);
            count++;
        }
    }
    fclose(file);

    if (count == 0) {
        static char default_word[] = "apple";
        printf("No valid words in word.txt. Using default 'apple'.\n");
        return default_word;
    }

    /* Simple random selection without srand (since we can't use time.h) */
    /* Using a fixed seed for reproducibility */
    int idx = count > 1 ? (count / 2) : 0;   // pick middle word if multiple
    static char mystery[WORD_LEN + 1];
    strcpy(mystery, words[idx]);
    return mystery;
}

int is_valid_guess(const char *guess) {
    if (strlen(guess) != WORD_LEN) return 0;
    for (int i = 0; i < WORD_LEN; i++) {
        if ((guess[i] < 'a' || guess[i] > 'z') && 
            (guess[i] < 'A' || guess[i] > 'Z')) {
            return 0;
        }
    }
    return 1;
}

void to_lower(char *str) {
    for (int i = 0; str[i]; i++) {
        if (str[i] >= 'A' && str[i] <= 'Z') {
            str[i] += 32;
        }
    }
}

void display_guesses(char guesses[][WORD_LEN + 1], int num_guesses, const char *mystery) {
    for (int g = 0; g < num_guesses; g++) {
        char display[WORD_LEN + 1] = {0};
        char pointers[WORD_LEN + 1] = "     ";
        char mystery_copy[WORD_LEN + 1];
        strcpy(mystery_copy, mystery);

        /* First pass: correct position → uppercase */
        for (int i = 0; i < WORD_LEN; i++) {
            if (guesses[g][i] == mystery_copy[i]) {
                display[i] = guesses[g][i] - 32;   // to uppercase
                mystery_copy[i] = '\0';            // mark as used
            } else {
                display[i] = guesses[g][i];
            }
        }

        /* Second pass: correct letter, wrong position → ^ */
        for (int i = 0; i < WORD_LEN; i++) {
            if (display[i] >= 'a' && display[i] <= 'z' && 
                strchr(mystery_copy, guesses[g][i]) != NULL) {
                pointers[i] = '^';
                /* Remove one occurrence */
                char *pos = strchr(mystery_copy, guesses[g][i]);
                if (pos) *pos = '\0';
            }
        }

        printf("%s\n", display);
        printf("%s\n", pointers);
    }
    printf("\n");
}

int main() {
    char *mystery = load_mystery_word();
    char guesses[MAX_GUESSES][WORD_LEN + 1];
    int num_guesses = 0;
    char input[100];

    printf("Welcome to Wordle!\n");
    printf("Guess the 5-letter word. You have %d attempts.\n\n", MAX_GUESSES);

    while (num_guesses < MAX_GUESSES) {
        printf("Enter your guess: ");
        if (fgets(input, sizeof(input), stdin) == NULL) {
            break;
        }

        input[strcspn(input, "\r\n")] = '\0';   // remove newline
        
        to_lower(input);

        if (!is_valid_guess(input)) {
            printf("Invalid guess. Must be exactly 5 letters (A-Z only).\n\n");
            continue;
        }

        strcpy(guesses[num_guesses], input);
        num_guesses++;

        display_guesses(guesses, num_guesses, mystery);

        if (strcmp(input, mystery) == 0) {
            printf("You won in %d guesses!\n", num_guesses);
            return 0;
        }
    }

    printf("Game over! The word was: ");
    for (int i = 0; i < WORD_LEN; i++) {
        printf("%c", mystery[i] - 32);   // print uppercase
    }
    printf("\n");

    return 0;
}
