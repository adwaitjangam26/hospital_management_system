/*
 * helpers.c
 * Shared console input helpers for validated integer values and alphabetic names.
 * Invalid integer lines and names containing characters other than letters or spaces are rejected.
 */
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>

#include "helpers.h"

/*
 * Reads and validates one complete integer input line.
 * Rejects non-integer values and extra trailing characters, then prompts again.
 */
int inputInteger(void)
{
    int value;
    char extra;

    while (1) {
        if (scanf("%d%c", &value, &extra) == 2 && extra == '\n')
            return value;

        while (getchar() != '\n');
        system("cls");
        printf("Invalid input. Enter an integer: ");
    }
}

/*
 * Reads a patient name and recursively repeats validation until it contains only letters and spaces.
 * Rejects digits, punctuation, and other special characters.
 */
void inputString(char string[])
{
    scanf(" %99[^\n]", string);

    for (int i = 0; string[i] != '\0'; i++) {
        if (!isalpha((unsigned char)string[i]) && string[i] != ' ') {
            printf("Invalid name! Enter alphabets only: ");
            inputString(string);
            return;
        }
    }
}