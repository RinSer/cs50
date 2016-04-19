/**
 * recover.c
 *
 * Computer Science 50
 * Problem Set 4
 *
 * Recovers JPEGs from a forensic image.
 */
 
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

typedef uint8_t  BYTE;

#define CLUSTER 512
#define CARD_RAW "card.raw"

int main(int argc, char* argv[])
{
    if (argc > 1)
    {
        printf("Should not receive any arguments.\n");
    }
    
    // Open file with raw data
    FILE* raw = fopen(CARD_RAW, "r");
    if (raw == NULL)
    {
        printf("Chould not open %s\n", CARD_RAW);
        return 2;
    }
    
    BYTE buffer[CLUSTER];
    
    // Initialize the necessary variables
    int file_count = -1;
    char* file_name = malloc(sizeof(char)*8);
    FILE* jpg = NULL;
    
    while (fread(&buffer, CLUSTER, 1, raw) == 1)
    {
        
        if (buffer[0] == 0xff && buffer[1] == 0xd8 && buffer[2] == 0xff)
        {
            if (jpg != NULL)
            {
                fclose(jpg);
            }
            
            // Adjust file counter and jpg file name
            file_count++;
            sprintf(file_name, "%03d.jpg", file_count);
            
            // Create new jpg file
            jpg = fopen(file_name, "w");
            if (jpg == NULL)
            {
                printf("Could not open %s\n", file_name);
                return 2;
            }
            
            // Write current buffer to the file
            fwrite(&buffer, CLUSTER, 1, jpg);
        }
        else if (jpg != NULL)
        {
            // Write current buffer to the file
            fwrite(&buffer, CLUSTER, 1, jpg);
        }
    }
    
    free(file_name);
    fclose(jpg);
    fclose(raw);
    
}
