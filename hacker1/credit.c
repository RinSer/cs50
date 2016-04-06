#include <stdio.h>
#include <cs50.h>

#define INVALID "INVALID\n"
#define AX "AMEX\n"
#define MC "MASTERCARD\n"
#define VS "VISA\n"

/*
 * Receives a credit card number as input 
 * and checks if it has a valid format.
 * Outputs the name of card provider.
 */

int main(void)
{
    long long card_number;
    long long card_number_copy;
    int digit_length;
    int card_digits[16];
    do {
        printf("Number: ");
        card_number = GetLongLong();
    } while (card_number < 1);
    // Find the number length
    card_number_copy = card_number;
    digit_length = 0;
    while (card_number_copy >= 10)
    {
        card_number_copy = card_number_copy/10;
        digit_length++;
    }
    if (digit_length < 13 || digit_length > 16)
    {
        printf(INVALID);
        return 0;
    }
    // Store the number as int array
    int d = digit_length;
    while (d >= 0)
    {
        card_digits[d] = card_number%10;
        card_number = card_number/10;
        d--;
    }
    // Find check sum
    int odd_sum = 0;
    int even_sum = 0;
    int digit;
    int ones;
    int tens;
    int i = digit_length;
    while (i >= 0)
    {
        odd_sum += card_digits[i];
        i--;
        if (i >= 0)
        {
            digit = card_digits[i]*2;
            ones = digit%10;
            tens = digit/10;
            even_sum += (ones+tens);
            i--;   
        }
    }
    int check_sum = odd_sum + even_sum;
    // Find if check sum is valid
    if (check_sum%10 == 0)
    {
        int first_digit = card_digits[0];
        switch (first_digit)
        {
            case 3:
                printf(AX);
                return 0;
            case 4:
                printf(VS);
                return 0;
            case 5:
                printf(MC);
                return 0;
            default:
                printf(INVALID);
                return 0;
        }
    }
    else
    {
        printf(INVALID);
        return 0;
    }
}