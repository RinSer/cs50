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
    // TODO: implement an O(n^2) sorting algorithm
    // Insertion sort
    for (int i = 0; i < n; i++)
    {
        int next_value = values[i];
        int j = 0;
        while (j < i)
        {
            if (next_value < values[j])
            {
                for (int k = i; k > j; k--)
                {
                    values[k] = values[k-1];
                }
                values[j] = next_value;
                break;
            }
            j++;
        }
    }
    
    return;
}