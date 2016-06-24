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
#define DIM_MAX 9
#define GOD_MODE "GOD"
#define NEW_GAME "NEW"

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
void almost_solve_row(int n);
bool solve_col(int n);
void almost_solve_col(int n);
void brute_force_last(void);
void place_right(int tile);
void move_diagonally(int tile, int r, int c);
void move_vertically(int tile, int r);
void move_horizontally(int tile, int c);
bool zero_up(void);
bool zero_down(void);
bool zero_left(void);
bool zero_right(void);
bool row_invariant(int r);
bool col_invariant(int c);

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
            if (strcmp(input, GOD_MODE) == 0)
            {
                if (d > 4)
                {
                    printf("\nThat is too big for me!\n");
                    usleep(50000);
                    break;
                }
                tile = 0;
                solve();
                free(input);
                break;
            }
            else if (strcmp(input, NEW_GAME) == 0)
            {
                random_board();
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
    for (int i = 0; i < 250; i++)
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
    // Solve the first d-2 rows and columns
    for (int i = 0; i < d-2; i++)
    {
        solve_row(i);
        solve_col(i);
    }
    // Place the last four tiles
    brute_force_last();
}

/**
 * Helper function to solve the n-th row
 */
bool solve_row(int n)
{
    // Solve d-1 tiles
    almost_solve_row(n);
    // Check if everything is correct
    for (int c = 0; c < d-1; c++)
    {
        // Find the target tile
        int target;
        if (c == d-2)
            target = d*n+d; // t2
        else
            target = d*n+1+c;
        // Check if it is in the proper place
        if (board[n][c] != target)
        {
            almost_solve_row(n);
        }
    }
    // Find the last two tiles
    int t1 = d*n+d-1;
    int t2 = d*n+d;
    // Move zero down one row
    if (board[n][d-1] == 0 && board[n+1][d-1] != t1)
    {
        zero_down();
    }
    else
    {
        zero_left();
        zero_down();
        zero_right();
        move_horizontally(t1, d-3);
        zero_up();
        zero_right();
        zero_down();
    }
    // Place the last tile one column before last
    // Check if the last two tiles are solved
    while (board[n][d-2] != t1 || board[n][d-1] != t2)
    {
        // Place t1 under t2
        while (board[n+1][d-2] != t1)
        {
            // Place zero to the right of t1
            place_right(t1);
            // Move t1 to row n+1
            move_vertically(t1, n+1);
            place_right(t1);
            // Move t1 to col d-2
            move_horizontally(t1, d-2);
        }
        // Place the last two tiles into proper places
        zero_up();
        zero_left();
        zero_down();
    }

    return row_invariant(n);
}

/**
 * Helper function to solve d-1 tile on the n-th row
 */
void almost_solve_row(int n)
{
    // Check if all tiles except the last two are solved
    for (int c = 0; c < d-1; c++)
    {
        // Find the target tile
        int target;
        if (c == d-2)
            target = d*n+d; // t2
        else
            target = d*n+1+c;
        // Check if it is in the proper place
        if (board[n][c] != target)
        {
            // Place zero tile to the right of target
            place_right(target);
            // Move target tile diagonally
            move_diagonally(target, n, c);
            // Place zero tile to the right of target
            place_right(target);
            // Move target tile vertically
            move_vertically(target, n);
            // Place zero tile to the right of target
            place_right(target);
            // Move target tile horizontally
            move_horizontally(target, c);
        }

        // Check if one of the previous has been undone
        for (int ci = 0; ci < c; ci++)
        {
            int invariant = d*n+1+ci;
            if (board[n][ci] != invariant)
            {
                zero_left();
                zero_left();
                zero_down();
                zero_right();
                zero_up();
                zero_right();
            }
        }
    }
}

/**
 * Helper function to solve the n-th row
 */
bool solve_col(int n)
{
    // Solve d-1 tiles
    almost_solve_col(n);
    // Check if everything is correct
    for (int r = 0; r < d-1; r++)
    {
        int target;
        if (r == d-2)
            target = d*(d-1)+1+n; // t2
        else
            target = d*r+1+n;
        // Check if it is in the proper place
        if (board[r][n] != target)
        {
            almost_solve_col(n);
        }
    }
    // Find the last two tiles
    int t1 = d*(d-2)+1+n;
    int t2 = d*(d-1)+1+n;
    // Check if t1 is not on the last place
    if (board[d-1][n] == t1)
    {
        place_right(t2);
        zero_down();
        zero_left();
        zero_up();
        zero_right();
        zero_right();
        zero_down();
        zero_left();
        zero_up();
        zero_left();
        zero_down();
        zero_right();
        zero_right();
        zero_up();
        zero_left();
        zero_down();
    }
    // Check if the last two tiles are solved
    while (board[d-2][n] != t1 || board[d-1][n] != t2)
    {
        // Place t1 under t2
        while (board[d-2][n+1] != t1)
        {
            // Place zero to the right of t1
            place_right(t1);
            // Move t1 to row d-2
            move_vertically(t1, d-2);
            place_right(t1);
            // Move t1 to col n+1
            move_horizontally(t1, n+1);
        }
        // Place the last two tiles into proper places
        // Check zero tile position
        if (board[d-2][n+2] == 0)
        {
            zero_down();
            zero_left();
        }
        zero_left();
        zero_up();
        zero_right();
    }

    return col_invariant(n);
}

/**
 * Helper function to solve d-1 tile on the n-th row
 */
void almost_solve_col(int n)
{
    // Check if all tiles except the last two are solved
    for (int r = 0; r < d-1; r++)
    {
        // Find the target tile
        int target;
        if (r == d-2)
            target = d*(d-1)+1+n; // t2
        else
            target = d*r+1+n;
        // Check if it is in the proper place
        if (board[r][n] != target)
        {
            // Place zero tile to the right of target
            place_right(target);
            // Move target tile diagonally
            move_diagonally(target, r, n);
            // Place zero tile to the right of target
            place_right(target);
            // Move target tile vertically
            move_vertically(target, r);
            // Place zero tile to the right of target
            place_right(target);
            // Move target tile horizontally
            move_horizontally(target, n);
        }
    }
}

/**
 * Helper function to place the last four tiles 
 * iteratively shuffling them
 */
void brute_force_last(void)
{
    // Place zero to the lower corner
    zero_down();
    zero_right();
    // Iterate zero rotation until the proper placement
    while (!won())
    {
        zero_up();
        zero_left();
        zero_down();
        zero_right();
    }
}

/**
 * Helper function to place zero tile to the right of 
 * a target tile
 */
void place_right(int tile)
{
    // Variables to store tile coordinates
    int ti, tj, i0, j0;
    // Find zero tile
    find_tile(0, &i0, &j0);
    // Find target tile
    find_tile(tile, &ti, &tj);
    // Check if zero tile is in the right place
    while (ti != i0 || tj+1 != j0)
    {
        // If target tile is at the last column
        if (tj == d-1)
        {
            // Place zero tile one col before last
            if (j0 == d-1)
            {
                zero_left();
                j0--;
            }
            else
            {
                //for (int c = 0; c < abs(j0 - (d-2)); c++)
                while (j0 != (d-2))
                {
                    zero_right();
                    j0++;
                }
            }
            // Place zero tile to the target row
            //for (int r = 0; r < abs(i0 - ti); r++)
            while (i0 != ti)
            {
                if (i0 < ti)
                {
                    zero_down();
                    i0++;
                }
                else
                {
                    zero_up();
                    i0--;
                }
            }
            // Swap the tiles
            zero_right();
            j0++;
            tj--;
        }
        // If tiles are on the same row
        else if (ti == i0)
        {
            // Check if zero is already on the right
            if (j0 != tj+1)
            {
                // If zero tile is on the right
                if (j0 > tj)
                {
                    //for (int c = 0; c < abs(j0 - (tj+1)); c++)
                    while (j0 != (tj+1))
                    {
                        zero_left();
                        j0--;
                    }
                }
                // If zero tile is on the left
                else
                {
                    //for (int c = 0; c < abs(j0 - tj); c++)
                    while (j0 != tj)
                    {
                        zero_right();
                        j0++;
                    }
                    tj--;
                }
            }
        }
        // All other juxtapositions
        else
        {
            // Place zero tile one column past the target
            //for (int c = 0; c < abs(j0 - (tj+1)); c++)
            while (j0 != (tj+1))
            {
                if (j0 < (tj+1))
                {
                    zero_right();
                    j0++;
                }
                else
                {
                    zero_left();
                    j0--;
                } 
            }
            // Place zero tile to the target row
            //for (int r = 0; r < abs(i0 - ti); r++)
            while (i0 != ti)
            {
                if (i0 < ti)
                {
                    zero_down();
                    i0++;
                }
                else
                {
                    zero_up();
                    i0--;
                }
            }
        }
    }
}

/**
 * Helper function to move target tile one place diagonally
 * until it reaches target row or column
 */
void move_diagonally(int tile, int r, int c)
{
    // Variables to store tiles coordinates
    int ti, tj, i0, j0;
    // Find zero tile
    find_tile(0, &i0, &j0);
    // Find target tile
    find_tile(tile, &ti, &tj);
    // Move target tile until it reaches 
    // the necessary row or column
    while (ti > r && tj > c)
    {
        zero_up();
        zero_left();
        zero_down();
        // Adjust target tile row
        ti--;
        zero_left();
        zero_up();
        zero_right();
        // Adjust target tile col
        tj--;
    }
}

/**
 * Helper function to move target tile vertically 
 * until it reaches the necessary row
 */
void move_vertically(int tile, int r)
{
    // Variables to store tiles coordinates
    int ti, tj, i0, j0;
    // Find zero tile
    find_tile(0, &i0, &j0);
    // Find target tile
    find_tile(tile, &ti, &tj);
    // Move target tile one place until it 
    // reaches the necessary row
    while (ti != r)
    {
        // Move up
        if (ti > r)
        {
            zero_up();
            zero_left();
            zero_down();
            // Adjust target tile row
            ti--;
            zero_right();
            zero_up();
        }
        // Move down
        else if (ti < r)
        {
            zero_down();
            zero_left();
            zero_up();
            // Adjust target tile row
            ti++;
            zero_right();
            zero_down();
        }
    }
}

/**
 * Helper function to move target tile horizontally 
 * until it reaches the necessary column
 */
void move_horizontally(int tile, int c)
{
    // Variables to store tiles coordinates
    int ti, tj, i0, j0;
    // Find zero tile
    find_tile(0, &i0, &j0);
    // Find target tile
    find_tile(tile, &ti, &tj);
    // Move target tile one place until it 
    // reaches the necessary column
    if (ti == d-1) // Check if target tile is in the last row
    {
        while (tj != c)
        {
            // Move left
            if (tj > c)
            {
                zero_up();
                zero_left();
                zero_left();
                zero_down();
                zero_right();
                // Adjust target tile col
                tj--;
                if (tj == c)
                    break;
            }
            // Move right
            else if (tj < c)
            {
                zero_left();
                // Adjust target tile col
                tj++;
                // Stop if target tile has reached the last col
                if (tj == d-1)
                    break;
                zero_up();
                zero_right();
                zero_right();
                zero_down();
            }
        }
    }
    else
    {
        while (tj != c)
        {
            // Move left
            if (tj > c)
            {
                zero_down();
                zero_left();
                zero_left();
                zero_up();
                zero_right();
                // Adjust target tile col
                tj--;
            }
            // Move right
            else if (tj < c)
            {
                zero_left();
                // Adjust target tile col
                tj++;
                // Stop if target tile has reached the last col
                if (tj == d-1)
                    break;
                zero_down();
                zero_right();
                zero_right();
                zero_up();
            }
        }
    }
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

/**
 * Helper function to assess the r-th row integrity
 */
bool row_invariant(int r)
{
    // Final check
    for (int c = 0; c < d; c++)
    {
        int invariant = d*r+1+c;
        if (board[r][c] != invariant)
        {
            return false;
        }
    }
    
    return true;
}

/**
 * Helper function to assess the c-th col integrity
 */
bool col_invariant(int c)
{
    // Final check
    for (int r = 0; r < d; r++)
    {
        int invariant = d*r+1+c;
        if (board[c][r] != invariant)
        {
            return false;
        }
    }
    
    return true;
}