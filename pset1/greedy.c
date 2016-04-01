#include <cs50.h>
#include <stdio.h>
#include <math.h>

/*
 * Programs that counts the minimum number 
 * of coins to construct the input value.
 * Input should be a floating point number.
 *
 */

int main(void)
{
    float change;
    int number;
    int count;
    int coin[4] = {25, 10, 5, 1};
    printf("O hai! ");
    // Input check loop
    do {
        printf("How much change is owed?\n");
        change = GetFloat();
    } while (change < 0);
    // Precision adjustment
    number = (int) round(change*100);
    count = 0;
    for (int i=0; i<4; i++) {
        count += number/coin[i];
        number = number%coin[i];
    }
    printf("%d\n", count);
}
