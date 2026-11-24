// Shared input helpers for validated integers and alphabetic patient names.
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>

#include "helpers.h"

// Reads one complete line and accepts only a valid integer.
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

// Reads a patient name and repeats until it contains only letters and spaces.
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