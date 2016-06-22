/**
 * fifteen.c
 *
 * Computer Science 50
 * Problem Set 3
 *
 * Implements Game of Fifteen (generalized to d x d).
 *
 * Usage: fifteen d
 *
 * whereby the board's dimensions are to be d x d,
 * where d must be in [DIM_MIN,DIM_MAX]
 *
 * Note that usleep is obsolete, but it offers more granularity than
 * sleep and is simpler to use than nanosleep; `man usleep` for more.
 */
 
#define _XOPEN_SOURCE 500

#include <cs50.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <time.h>

// constants
#define DIM_MIN 3
#define DIM_MAX 4
#define GOD_MODE "G"

// board
int board[DIM_MAX][DIM_MAX];

// dimensions
int d;

// prototypes
void clear(void);
void greet(void);
void init(void);
void draw(void);
bool move(int tile);
bool won(void);
void solve(void);
// helper prototypes
void random_board(void);
void find_tile(int tile, int* r, int* c);
bool solve_row(int n);
bool zero_up(void);
bool zero_down(void);
bool zero_left(void);
bool zero_right(void);

int main(int argc, string argv[])
{
    // ensure proper usage
    if (argc != 2)
    {
        printf("Usage: fifteen d\n");
        return 1;
    }

    // ensure valid dimensions
    d = atoi(argv[1]);
    if (d < DIM_MIN || d > DIM_MAX)
    {
        printf("Board must be between %i x %i and %i x %i, inclusive.\n",
            DIM_MIN, DIM_MIN, DIM_MAX, DIM_MAX);
        return 2;
    }

    // open log
    FILE* file = fopen("log.txt", "w");
    if (file == NULL)
    {
        return 3;
    }

    // greet user with instructions
    greet();

    // initialize the board
    init();

    // randomize board
    random_board();

    // accept moves until game is won
    while (true)
    {
        // clear the screen
        clear();

        // draw the current state of the board
        draw();

        // log the current state of the board (for testing)
        for (int i = 0; i < d; i++)
        {
            for (int j = 0; j < d; j++)
            {
                fprintf(file, "%i", board[i][j]);
                if (j < d - 1)
                {
                    fprintf(file, "|");
                }
            }
            fprintf(file, "\n");
        }
        fflush(file);

        // check for win
        if (won())
        {
            printf("ftw!\n");
            break;
        }

        // Get user input 
        int tile;
        do {
            // prompt for move
            printf("Tile to move: ");
            char* input = GetString();
        
            // start the GOD mode
            if (strcmp(input, GOD_MODE) == 0 && d < 5)
            {
                tile = 0;
                solve();
                free(input);
                break;
            }
            else
            {
                tile = atoi(input);
            }
            
            if (tile > 0 && tile < d*d)
            {
                free(input);
                break;
            }
        } while (1);


        // log move (for testing)
        fprintf(file, "%i\n", tile);
        fflush(file);

        // move if possible, else report illegality
        if (tile > 0)
        {
            if (!move(tile))
            {
                printf("\nIllegal move.\n");
                usleep(500000);
            }
        }

        // sleep thread for animation's sake
        usleep(500000);
    }

    // close log
    fclose(file);

    // success
    return 0;
}

/**
 * Clears screen using ANSI escape sequences.
 */
void clear(void)
{
    printf("\033[2J");
    printf("\033[%d;%dH", 0, 0);
}

/**
 * Greets player.
 */
void greet(void)
{
    clear();
    printf("WELCOME TO GAME OF FIFTEEN\n");
    usleep(2000000);
}

/**
 * Initializes the game's board with tiles numbered 1 through d*d - 1
 * (i.e., fills 2D array with values but does not actually print them).  
 */
void init(void)
{
    int tile_number = d*d-1;
    // Assign values
    for (int i = 0; i < d; i++)
    {
        for (int j = 0; j < d; j++)
        {
            board[i][j] = tile_number;
            tile_number--;
        }
    }
    // Change tiles 1 and 2 if dimension is even
    if (board[0][0]%2 != 0)
    {
        board[d-1][d-2] = 2;
        board[d-1][d-3] = 1;
    }
}

/**
 * Function to randomize the board
 */
void random_board(void)
{
    srand(time(NULL));
    int r = 0;
    for (int i = 0; i < 2500; i++)
    {
        r = rand() % 4;
        switch (r)
        {
            case 0:
                if (zero_up())
                    break;
            case 1:
                if (zero_down())
                    break;
            case 2:
                if (zero_left())
                    break;
            case 3:
                if (zero_right())
                    break;
            default:
                break;
        }
    }
}

/**
 * Prints the board in its current state.
 */
void draw(void)
{
    for (int i = 0; i < d; i++)
    {
        for (int j = 0; j < d; j++)
        {
            if (board[i][j] == 0)
            {
                printf(" _  ");
            }
            else
            {
                if (board[i][j] < 10)
                {
                    printf(" %d  ", board[i][j]);
                }
                else
                {
                    printf("%d  ", board[i][j]);
                }
            }
            
        }
        printf("\n");
        printf("\n");
    }
}

/**
 * If tile borders empty space, moves tile and returns true, else
 * returns false. 
 */
bool move(int tile)
{
    // Variables to store empty tile coordinates
    int x;
    int y;
    
    // Check if input value is valid
    if (tile < 1 || tile > d*d-1)
    {
        return false;
    }
    
    // Find the empty tile
    find_tile(0, &y, &x);

    // Find the tile to move
    for (int i = 0; i < d; i++)
    {
        for (int j = 0; j < d; j++)
        {
            if (board[i][j] == tile)
            {
                // Move horizontal neighbor tile
                if (abs(y-i) == 1 && abs(x-j) == 0)
                {
                    board[y][x] = tile;
                    board[i][j] = 0;
                    return true;
                }
                // Move vertical neighbor tile
                else if ((y-i) == 0 && abs(x-j) == 1)
                {
                    board[y][x] = tile;
                    board[i][j] = 0;
                    return true;
                }
                else
                {
                    return false;
                }
            }
        }
    }
    
    return false;
}

/**
 * Returns true if game is won (i.e., board is in winning configuration), 
 * else false.
 */
bool won(void)
{
    // Tile counter
    int tile = 1;
    
    for (int i = 0; i < d; i++)
    {
        for (int j = 0; j < d; j++)
        {
            if (board[i][j] != tile)
            {
                return false;
            }
            tile++;
            if (tile > d*d-1)
            {
                tile = 0;
            }
        }
    }
    
    return true;
}

/**
 * Solves the puzzle in the GOD mode
 */
void solve(void)
{
    solve_row(0);
}

/**
 * Helper function to solve the n-th row
 */
bool solve_row(int n)
{
    // Variables to store the tiles coordinates
    int i, j, i0, j0;
    // Check if all tiles except the last two are solved
    for (int c = 0; c < d-2; c++)
    {
        int target = d*n+1+c;
        if (board[n][c] != target)
        {
            // Find the zero tile
            find_tile(0, &i0, &j0);
            // Find the target tile
            find_tile(target, &i, &j);
            // Move zero tile to the right of target tile
            printf("%d, %d, %d, %d\n", i, j, i0, j0);
            // Check if target tile is not at the last column
            if (j == d-1)
            {
                // Place zero tile one column before last
                if (j0 == d-1)
                {
                    move(board[i0][--j0]);
                }
                else if (j0 < d-2)
                {
                    while (j0 != d-2)
                    {
                        move(board[i0][++j0]);
                    }
                }
            }
            else
            {
                // Check if on the same row
                if (i == i0)
                {
                    // Place zero tile to the right of target tile
                    if (j0 < j)
                    {
                        while(j0 != j+1)
                        {
                            move(board[i0][++j0]);
                            if (j0 == j)
                                j--;
                        }
                    }
                    else
                    {
                        while(j0 != j+1)
                            move(board[i0][--j0]);
                    }
                }
            }
        }
    }

    return true;
}

/**
 * Helper function to find a tile
 */
void find_tile(int tile, int* r, int* c)
{
    for (int i = 0; i < d; i++)
    {
        for (int j = 0; j < d; j++) 
        {
            if (board[i][j] == tile)
            {
                *r = i;
                *c = j;
            }
        }
    }
}

/**
 * Helper functions to move the zero tile in particular direction
 */
bool zero_up(void)
{
    int i, j;
    find_tile(0, &i, &j);
    if (i > 0)
        return move(board[--i][j]);
    else
        return false;
}

bool zero_down(void)
{
    int i, j;
    find_tile(0, &i, &j);
    if (i < d-1)
        return move(board[++i][j]);
    else
        return false;
}

bool zero_left(void)
{
    int i, j;
    find_tile(0, &i, &j);
    if (j > 0)
        return move(board[i][--j]);
    else
        return false;
}

bool zero_right(void)
{
    int i, j;
    find_tile(0, &i, &j);
    if (j < d-1)
        return move(board[i][++j]);
    else
        return false;
}