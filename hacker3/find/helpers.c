/**
 * helpers.c
 *
 * Computer Science 50
 * Problem Set 3
 *
 * Helper functions for Problem Set 3.
 */
       
#include <cs50.h>

#include "helpers.h"

// constant
#define LIMIT 65536

/**
 * Returns true if value is in array of n values, else false.
 */
bool search(int value, int values[], int n)
{
    // TODO: implement a searching algorithm
    if (n < 1)
    {
        return false;
    }
    
    // Binary search
    int start;
    int end;
    
    if (values[n/2] == value)
    {
        return true;
    }
    
    if (values[n/2] > value)
    {
        start = 0;
        end = n/2-1;
    }
    else
    {
        start = n/2+1;
        end = n-1;
    }
    
    if (start > end)
    {
        return false;
    }
    
    int length = end-start+1;
    int next_half[length];
    int i = 0;
    while (start <= end)
    {
        next_half[i] = values[start];
        i++;
        start++;
    }
    
    return search(value, next_half, length);
}

/**
 * Sorts array of n values.
 */
void sort(int values[], int n)
{
    // TODO: implement an O(n) sorting algorithm
    // Counting sort
    int count[LIMIT];
    int output[n];
    int total;
    int oldCount;
	// Initialize counting array
	for (int i = 0; i < LIMIT; i++)
	{
		count[i] = 0;
	}

    // Calculate the histogram of value frequences
    for (int i = 0; i < n; i++)
    {
        count[values[i]] += 1;
    }
    // Calculate the starting index for each value
    total = 0;
    for (int i = 0; i < LIMIT; i++)
    {
        oldCount = count[i];
        count[i] = total;
        total += oldCount;
    }
    // Copy initial values to output array, preserving equal values order
    for (int i = 0; i < n; i++)
    {
        output[count[values[i]]] = values[i];
        count[values[i]] += 1;
    }
    // Set initial array pointers to the output array
    for (int i = 0; i < n; i++)
    {
        values[i] = output[i];
    }
    
    return;
}
