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
    for (int i = 0; i < d; i++)
    {
        for (int j = 0; j < d; j++)
        {
            if (board[i][j] == 0)
            {
                y = i;
                x = j;
            }
        }
    }
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
    // Create solved board invariant
    int solved_board[d][d];
    int c = 1;
    for (int i = 0; i < d; i++)
    {
        for (int j = 0; j < d; j++)
        {
            solved_board[i][j] = c;
            c++;
        }
    }
    solved_board[d-1][d-1] = 0;
    // Check if the first row is solved
    for (int k = 0; k < d; k++)
    {
        if (board[0][k] != solved_board[0][k])
        {
            // Move the first d-2 tiles into place
            if (k < d-2)
            {
                int target_i;
                int target_j;
                int empty_i;
                int empty_j;
                // Find the necessary tile
                for (int i = 0; i < d; i++)
                {
                    for (int j = 0; j < d; j++)
                    {
                        // Find the empty tile
                        if (board[i][j] == 0)
                        {
                            empty_i = i;
                            empty_j = j;
                        }
                        // Find the tile to move
                        if (board[i][j] == solved_board[0][k])
                        {
                            target_i = i;
                            target_j = j;
                        }
                    }
                }
                // Move empty tile to the target
                if (target_j == d-1)
				{
					// Move empty tile horizontally
                    if (empty_j > d-2)
                    {
                        move(board[empty_i][--empty_j]);
                        if (empty_j == target_j)
                        {
                            target_j++;
                        }
                    }
                    while (empty_j < d-2)
                    {
                        move(board[empty_i][++empty_j]);
                        if (empty_j == target_j)
                        {
                            target_j--;
                        }
                    }
                    // Move empty tile vertically
                    while (empty_i != target_i)
                    {
                        if (empty_i < target_i)
                        {
                            move(board[++empty_i][empty_j]);
                        }
                        else
                        {
                            move(board[--empty_i][empty_j]);
                        }
                    }
                    move(board[empty_i][++empty_j]);
                }
                else
                {
                    // Move empty tile horizontally
                    if (empty_j <= target_j)
                    {
                        while (empty_j != target_j+1)
                        {
                            move(board[empty_i][++empty_j]);
                            if (empty_j == target_j)
                            {
                                target_j--;
                            }
                        }
                    }
                    else
                    {
                        while (empty_j != target_j+1)
                        {
                            move(board[empty_i][--empty_j]);
                            if (empty_j == target_j)
                            {
                                target_j++;
                            }
                        }
                    }
                    // Move empty tile vertically
                    while (empty_i != target_i)
                    {
                        if (empty_i < target_i)
                        {
                            move(board[++empty_i][empty_j]);
                        }
                        else
                        {
                            move(board[--empty_i][empty_j]);
                        }
                    }
                }
                // Check the new target tile position
                for (int i = 0; i < d; i++)
                {
                    for (int j = 0; j < d; j++)
                    {
                        // Find the tile to move
                        if (board[i][j] == solved_board[0][k])
                        {
                            target_i = i;
                            target_j = j;
                        }
                    }
                }
                // Move target tile horizontally
                if (target_j != k)
                {
                    if (target_j < k)
                    {
                        if (target_i < d-1)
                        {
                            for (int i = 0; i < k-target_j; i++)
                            {
                                move(board[empty_i][--empty_j]);
                                move(board[++empty_i][empty_j]);
                                move(board[empty_i][++empty_j]);
                                move(board[empty_i][++empty_j]);
                                move(board[--empty_i][empty_j]);
                            }
                        }
                        else
                        {
                            for (int i = 0; i < k-target_j; i++)
                            {
                                move(board[empty_i][--empty_j]);
                                move(board[--empty_i][empty_j]);
                                move(board[empty_i][++empty_j]);
                                move(board[empty_i][++empty_j]);
                                move(board[++empty_i][empty_j]);
                            }
                        }
                    }
                    else
                    {
                        if (target_i < d-1)
                        {
                            for (int i = 0; i < target_j-k; i++)
                            {
                                move(board[++empty_i][empty_j]);
                                move(board[empty_i][--empty_j]);
                                move(board[empty_i][--empty_j]);
                                move(board[--empty_i][empty_j]);
                                move(board[empty_i][++empty_j]);
                            }
                        }
                        else
                        {
                            while (target_j > k)
                            {
                                move(board[--empty_i][empty_j]);
                                move(board[empty_i][--empty_j]);
                                move(board[empty_i][--empty_j]);
                                move(board[++empty_i][empty_j]);
                                move(board[empty_i][++empty_j]);
                                target_j--;
                            }
                        }
                    }
                }
                // Move target tile vertically
                if (target_i != 0)
                {
                    move(board[--empty_i][empty_j]);
                    move(board[empty_i][--empty_j]);
                    move(board[++empty_i][empty_j]);
                    for (int i = 0; i < target_i-1; i++)
                    {
                        move(board[empty_i][++empty_j]);
                        move(board[--empty_i][empty_j]);
                        move(board[--empty_i][empty_j]);
                        move(board[empty_i][--empty_j]);
                        move(board[++empty_i][empty_j]);
                    }
                }
            }
            // Move the last two tiles into place
            else
            {
                
            }
        }
    }
}
