#define _XOPEN_SOURCE
#define SALT_LENGTH 2
#define WORDS_PATH "./words"

#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

/*
 * DES Hash Crack
 * Takes one argument as DES hashed string and tries to decrypt it 
 * or takes two arguments: unencrypted string and salt, and encrypts
 * the given string.
 */

int dictionary_attack(char * password);
void bruteforce_attack(char * password, int length);
int recursive_increment(char * string);

int main(int argc, char* argv[])
{
    if (argc == 2) 
    {
        if (dictionary_attack(argv[1]) < 0)
        {
            bruteforce_attack(argv[1], 8);
        }
        else
        {
            return 0;
        }
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

// Bruteforce attack using common English words
int dictionary_attack(char * password)
{
    // Get Salt
    char salt[SALT_LENGTH];
    salt[0] = password[0];
    salt[1] = password[1];
    // Read words from a file and try them
    FILE * words;
    char word[255];
    words = fopen(WORDS_PATH, "r");
    
    if(words == NULL) 
    {
        perror("Error in opening file");
        return -1;
    }
    
    while(fgets(word, 255, words) != NULL)
    {
        word[strlen(word)-1] = 0;
        if(strcmp(password, crypt(word, salt)) == 0)
        {
            printf("%s\n", word);
            fclose(words);
            return 0;
        }
        int i = 1;
        do {
            if (word[0] < 91) 
            {
                word[0] = tolower(word[0]);
            } 
            else 
            {
                for (int j = 0; j < strlen(word); j++)
                {
                    word[j] = toupper(word[j]);
                }
                i = 0;
            }
            if(strcmp(password, crypt(word, salt)) == 0)
            {
                printf("%s\n", word);
                fclose(words);
                return 0;
            }
        } while (i);
        
    }

    if(feof(words))
    {
        return -1;
    }
    else if (ferror(words))
    {
        printf("Error while reading the file!\n");
        return -1;
    }
    
    fclose(words);

    return 0;
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
        for (int j = 0; j < i; j++) string[j] = 32;
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
            string[0] += 31;
            return 1;
        case 126:
            string[0] = 32;
            // Move pointer to the next char and increment it
            return recursive_increment(string+sizeof(char));
        default:
            string[0]++;
            return 1;
    }
}
