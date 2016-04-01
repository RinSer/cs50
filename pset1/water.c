#include <cs50.h>
#include <stdio.h>

/*
 * Program that returns the number of bottles 
 * you waste taking shower.
 * Takes positive integer as input.
 */

int main(void)
{
    int minutes;
    int bottles;
    // Loops until correct input
    do {
        printf("minutes: ");
        minutes = GetInt();
    } while (minutes < 0);
    bottles = 12*minutes;
    printf("bottles: %i\n", bottles);
}
