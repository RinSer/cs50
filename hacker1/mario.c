#include <cs50.h>
#include <stdio.h>

/*
 * Program that builds a pyramid with gap
 * not higher than 23 lines.
 * Takes a positive integer less than 23
 * as input.
 */

// Function that checks height input value
int check_height(int height) 
{
    if (height < 0) {
        return 1;
    } else if (height > 23) {
        return 1;
    } else {
        return 0;
    }
}

int main(void)
{
    int height;
    int lines;
    // Loops until receives proper input
    do {
        printf("height: ");
        height = GetInt();
    } while (check_height(height));
    // Loop generating half-pyramid
    lines = height;
    while (lines > 0) {
        for (int i=0; i < lines-1; i++) {
            printf(" ");
        }
        for (int i=0; i < height-lines+1; i++) {
            printf("#");
        }
        printf("  ");
        for (int i=0; i < height-lines+1; i++) {
            printf("#");
        }
        printf("\n");
        lines--;
    }
}

