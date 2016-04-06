#include <cs50.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>

/*
 * Program that receives person's full name
 * and outputs his/hers initials.
 * Takes a string as input.
 *
 */
 
int main(void)
{
    char * full_name;
    full_name = GetString();
    if (full_name[0] != ' ') {
        printf("%c", toupper(full_name[0]));
    }
    for (int i = 1, length = strlen(full_name); i < length; i++) {
        if (full_name[i-1] == ' ' && full_name[i] != ' ') {
            printf("%c", toupper(full_name[i]));
        }
    }
    printf("\n");
}
