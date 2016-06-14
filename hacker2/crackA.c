#define _XOPEN_SOURCE
#define SALT_LENGTH 2

#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

/*
 * DES Hash Crack
 * Takes one argument as DES hashed string and tries to decrypt it 
 * using only brute force approach (all permutations of latin letters) 
 * or takes two arguments: unencrypted string and salt, and encrypts
 * the given string.
 */

void bruteforce_attack(char * password, int length);
int recursive_increment(char * string);

int main(int argc, char* argv[])
{
    if (argc == 2) 
    {
        bruteforce_attack(argv[1], 8);
    } 
    else if (argc == 3) 
    {
        printf("%s\n", crypt(argv[1], argv[2]));
    } 
    else 
    {
        printf("Wrong number of arguments!\n");
        return -1;
    }
}


// Brute force attack using all possible char combinations
void bruteforce_attack(char * password, int length)
{
    // Get Salt
    char salt[SALT_LENGTH];
    salt[0] = password[0];
    salt[1] = password[1];
    // Try all possible combinations of ASCII printable chars
    char string[length+1];
    for (int i = 0; i <= length; i++)
    {
        for (int j = 0; j < i; j++) string[j] = 65;
        string[i] = 0;
        do {
            if(strcmp(password, crypt(string, salt)) == 0)
            {
                printf("%s\n", string);
                return;
            }
        } while (recursive_increment(string));
    }
    // The password has not been found
    printf("Password has not been found\n");
}

// Recursively increment string
int recursive_increment(char * string)
{
    switch (string[0])
    {
        case 0:
            return 0;
        case 1:
            string[0] += 64;
            return 1;
        // Skip non-alphabetic chars
        case 90:
            string[0] = 97;
            return 1;
        case 122:
            string[0] = 65;
            // Move pointer to the next char and increment it
            return recursive_increment(string+sizeof(char));
        default:
            string[0]++;
            return 1;
    }
}
