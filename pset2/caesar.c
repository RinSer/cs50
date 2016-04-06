#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <cs50.h>

/*
 * Program that outputs Caesar cipher 
 * of the received string.
 * Takes the key size as argument.
 */

int main(int argc, char* argv[])
{
    if (argc == 2)
    {
        int key = atoi(argv[1]);
        char* string = GetString();
        char c;
        // Cipher logic
        for (int i = 0; i < strlen(string); i++)
        {
            if (string[i] >= 'A' && string[i] <= 'Z')
            {
                c = ((string[i]-'A')+key)%26+'A';
            }
            else if (string[i] >= 'a' && string[i] <= 'z')
            {
                c = ((string[i]-'a')+key)%26+'a';
            }
            else
            {
                c = string[i];
            }
            printf("%c", c);
        }
        printf("\n");
    }
    else
    {
        printf("Wrong number of arguments!\n");
        return 1;
    }
}