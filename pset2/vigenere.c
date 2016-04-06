#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <cs50.h>

/*
 * Program that outputs Vigener cipher 
 * of the received string.
 * Takes the alphabetic string key as argument.
 */
 
int case_check(char c);

int main(int argc, char* argv[])
{
    if (argc == 2)
    {
        char* key = argv[1];
        for (int i = 0; i < strlen(argv[1]); i++)
        {
            if (case_check(key[i]) == -1)
            {
                printf("Wrong cipher key!\n");
                return 1;
            }
        }
        char* string = GetString();
        char c;
        // Cipher logic
        int key_index;
        int j = 0;
        for (int i = 0; i < strlen(string); i++)
        {
            // Find key char alphabetic index
            if (case_check(key[j]) == 1)
            {
                key_index = key[j] - 'A';
            }
            else
            {
                key_index = key[j] - 'a';
            }
            // Encrypt a char
            if (case_check(string[i]) == 1)
            {
                c = ((string[i]-'A')+key_index)%26+'A';
            }
            else if (case_check(string[i]) == 0)
            {
                c = ((string[i]-'a')+key_index)%26+'a';
            }
            else
            {
                c = string[i];
                j--;
            }
            printf("%c", c);
            // Increment j
            if (j < strlen(key)-1)
            {
                j++;
            }
            else
            {
                j = 0;
            }
        }
        printf("\n");
    }
    else
    {
        printf("Wrong number of arguments!\n");
        return 1;
    }
}

// Function to check if char is uppercase or lowercase
int case_check(char c)
{
    if (c >= 'A' && c <= 'Z')
    {
        // Uppercase
        return 1;
    }
    else if (c >= 'a' && c <= 'z')
    {
        // Lowercase
        return 0;
    }
    else
    {
        // Not a letter
        return -1;
    }
}