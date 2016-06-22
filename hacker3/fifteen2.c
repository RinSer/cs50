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
void solve(int sd);
// helper prototypes
void find_tile(int tile, int* r, int* c);
void solve_first_row(int l);
void solve_first_col(int l);
void align_vertically(int* first, int second, int col);
void align_horizontally(int* first, int second, int row);

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
                solve(d);
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
void solve(int sd)
{
    int k;
    if (sd == d)
    {
        k = 0;
    }
    else
    {
        k = d - sd;
    }
    // Check if the first row is solved
    // and solve it if not
    solve_first_row(k);
    
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
 * Helper function to align the tiles vertically 
 * given they are placed in different columns
 */
void align_vertically(int* first, int second, int col)
{
    while (*first != second)
    {
        if (*first > second)
            move(board[--*first][col]);
        else
            move(board[++*first][col]);
    }
}

/**
 * Helper function to align the tiles horizontally 
 * given they are placed in different columns
 */
void align_horizontally(int* first, int second, int row)
{
    while (*first != second)
    {
        if (*first > second)
            move(board[row][--*first]);
        else
            move(board[row][++*first]);
    }
}

/**
 * Helper function to solve the first row
 */
void solve_first_row(int l)
{
	// Initialize vars to store the tile positions
    int i = 0;
    int j = 0;
    int i0 = 0;
    int j0 = 0;
    // Check the first row integrity
    for (int k = l; k < d-2; k++)
    {
        // Check if the first d-2 tiles are correct
        if (board[l][k] != k+1)
        {
            printf("Tile: %d\n", k+1+d*l);
            // Find the misplaced tile
            find_tile(k+1+d*l, &i, &j);
            // Find the zero tile
            find_tile(0, &i0, &j0);
            // Move zero to the target
            if (j == d-1)
            {
                // If tile zero is at the last column
                if (j0 == d-1) 
                    move(board[i0][--j0]);
                // If tile zero is somewhere at the left
                while (j0 != d-2)
                {
                    move(board[i0][++j0]);
                }
                // Allign the tiles vertically
                align_vertically(&i0, i, j0);
                // Swap the tiles
                move(board[i][j]);
                // Swap the tiles coordinates
                int t = 0;
                t = i;
                i = i0;
                i0 = t;
                t = j;
                j = j0;
                j0 = t;
            }
            else
            {
                // If the tiles are at the same col
                // move zero tile one step right
                if (j0 == j)
                    move(board[i0][++j0]);
                // Align the tiles vertically
                align_vertically(&i0, i, j0);
                // Move zero tile to the left of target tile
                while (j0 != j+1)
                {
                    if (j0 > j+1)
                        move(board[i0][--j0]);
                    else
                        move(board[i0][++j0]);
                    // Check if we skipped the target tile
                    if (j0 == j)
                        break;
                }
                // Set the target tile x-coordinates
                j = j0 - 1;
            }
            // Move the target tile to its proper place
            // Six moves for diagonal shift
            while (i != l && j != k)
            {
                // Move the zero tile up
                move(board[--i0][j0]);
                // Move the zero tile left
                move(board[i0][--j0]);
                // Move the target tile up
                move(board[++i0][j0]);
                i--;
                if (i == l)
                    break;
                // Move the zero tile left
                move(board[i0][--j0]);
                // Move the zero tile up
                move(board[--i0][j0]);
                // Swap the tiles
                move(board[i0][++j0]);
                j--;
                if (j == k)
                    break;
            }
            // Five moves for vertical or horizontal shift
            if (j == k) // Vertically
            {
                while (i != l)
                {
                    // Move the zero tile up
                    move(board[--i0][j0]);
                    // Move the zero tile left
                    move(board[i0][--j0]);
                    // Move the target tile up
                    move(board[++i0][j0]);
                    i--;
                    // Move the zero tile right
                    move(board[i0][++j0]);
                    // Move the zero tile up
                    move(board[--i0][j0]);
                }
            }
            else // Horizontally
            {
                while (j != k)
                {
                    // Move the zero tile down
                    move(board[++i0][j0]);
                    // Move the zero tile left twice
                    move(board[i0][--j0]);
                    move(board[i0][--j0]);
                    // Move the zero tile up
                    move(board[--i0][j0]);
                    // Swap the tiles
                    move(board[i0][++j0]);
                    j--;
                }
            }
        }
    }
    // Check if the last two tiles are not correct
    if (board[l][d-2] != (d-1)+(l*(d-1)) || board[l][d-1] != d+(l*(d-1)))
    {
        printf("Tile: %d\n", (d-1)+(l*(d-1)));
        // Find the second to last tile for the row and move it to the last position in the row
        find_tile((d-1)+(l*(d-1)), &i, &j);
        // Check if the target tile is in the last column
        if (j == d-1 && i != l)
        {
            // Move zero to the target tile's row
            while (i0 != i-1)
            {
                move(board[i0++][j0]);
            }
            // Move the target tile until it reaches its proper row
            while (i != l)
            {
                // Move zero tile to the right
                move(board[i0][++j0]);
                // Swap the tiles
                move(board[++i0][j0]);
                i--;
                if (i == l)
                    break;
                // Move zero tile to the left
                move(board[i0][--j0]);
                // Move zero tile up two times
                move(board[--i0][j0]);
                move(board[--i0][j0]);
            }  
        }
        else if (i != l)
        {
            // Move zero tile to the last column
            move(board[i0][++j0]);
            // Move zero tile to the same row with the target tile
            while (i0 != i)
            {
                if (i0 < i)
                {
                    move(board[++i0][j0]);
                }
                else
                {
                    move(board[--i0][j0]);
                }
                
            }
            // Move zero tile to the right of the target tile
            while (j0 != j+1)
            {
                if (j0 > j+1)
                {
                    move(board[i0][--j0]);
                }
                else
                {
                    move(board[i0][++j0]);
                }
            }
            // If the target tile is on the last row
            if (i == d-1)
            {
                // Move the target tile to the last column
                while (j != d-1)
                {
                    // Swap the tiles
                    move(board[i0][--j0]);
                    j++;
                    // Move zero tile up
                    move(board[--i0][j0]);
                    if (j == d-1)
                        break;
                    // Move zero tile to the right twice
                    move(board[i0][++j0]);
                    move(board[i0][++j0]);
                    // Move zero tile down
                    move(board[++i0][j0]);
                }
            }
            else
            {
                // Move the target tile to the last column
                while (j != d-1)
                {
                    // Swap the tiles
                    move(board[i0][--j0]);
                    j++;
                    if (j == d-1)
                    {
                        // Move zero tile up
                        move(board[--i0][j0]);
                        break;
                    }
                    // Move zero tile down
                    move(board[++i0][j0]);
                    // Move zero tile to the right twice
                    move(board[i0][++j0]);
                    move(board[i0][++j0]);
                    // Move zero tile up
                    move(board[--i0][j0]);
                }
            }
            // Move the target tile until it reaches its proper row
            while (i != l)
            {
                // Move zero tile to the right
                move(board[i0][++j0]);
                // Swap the tiles
                move(board[++i0][j0]);
                i--;
                if (i == l)
                    break;
                // Move zero tile to the left
                move(board[i0][--j0]);
                // Move zero tile up two times
                move(board[--i0][j0]);
                move(board[--i0][j0]);
            }
        }
        // Find the last tile for the row and move it to the last position in the row below
        find_tile(d+(l*(d-1)), &i, &j);
        // Check if the target tile is in the last column
        if (j == d-1 && i != l+1)
        {
            // Move zero tile to one column before the last
            move(board[i0][--j0]);
            // Move zero to the target tile's row
            while (i0 != i-1)
            {
                if (i0 < i-1)
                {
                    move(board[++i0][j0]);
                }
                else
                {
                    break;
                }
                
            }
            // Move the target tile until it reaches its proper row
            while (i != l+1)
            {
                // Move zero tile to the right
                move(board[i0][++j0]);
                // Swap the tiles
                move(board[++i0][j0]);
                i--;
                if (i < 0)
                    break;
                if (i == l+1)
                    break;
                // Move zero tile to the left
                move(board[i0][--j0]);
                // Move zero tile up two times
                move(board[--i0][j0]);
                move(board[--i0][j0]);
            }  
        }
        else if (i != l+1)
        {
            // Move zero tile to the same row with the target tile
            while (i0 != i)
            {
                if (i0 < i)
                {
                    move(board[++i0][j0]);
                }
                else
                {
                    break;
                }
            }
            // Move zero tile to the right of the target tile
            while (j0 != j+1)
            {
                if (j0 > j+1)
                {
                    move(board[i0][--j0]);
                }
                else
                {
                    break;
                }
            }
            // If the target tile is on the last row
            if (i == d-1)
            {
                // Move the target tile to the last column
                while (j != d-1)
                {
                    // Swap the tiles
                    move(board[i0][--j0]);
                    j++;
                    // Move zero tile up
                    move(board[--i0][j0]);
                    if (j == d-1)
                        break;
                    // Move zero tile to the right twice
                    move(board[i0][++j0]);
                    move(board[i0][++j0]);
                    // Move zero tile down
                    move(board[++i0][j0]);
                }
            }
            else
            {
                // Move the target tile to the last column
                while (j != d-1)
                {
                    // Swap the tiles
                    move(board[i0][--j0]);
                    j++;
                    if (j == d-1)
                    {
                        // Move zero tile up
                        move(board[--i0][j0]);
                        break;
                    }
                    // Move zero tile down
                    move(board[++i0][j0]);
                    // Move zero tile to the right twice
                    move(board[i0][++j0]);
                    move(board[i0][++j0]);
                    // Move zero tile up
                    move(board[--i0][j0]);
                }
            }
            // Move the target tile until it reaches its proper row
            while (i != l+1)
            {
                // Move zero tile to the right
                move(board[i0][++j0]);
                // Swap the tiles
                move(board[++i0][j0]);
                i--;
                if (i < 0)
                    break;
                if (i == l+1)
                    break;
                // Move zero tile to the left
                move(board[i0][--j0]);
                // Move zero tile up two times
                move(board[--i0][j0]);
                move(board[--i0][j0]);
            }
        }
        // Place the target tiles on the proper row
        // Move zero tile to the left
        move(board[i0][--j0]);
        // Move zero tile up twice
        move(board[--i0][j0]);
        move(board[--i0][j0]);
        // Move zero tile to the right
        move(board[i0][++j0]);
        // Move zero tile down
        move(board[++i0][j0]);
    }
}

/**
 * Helper function to solve the first column
 */
void solve_first_col(int l)
{
    // Initialize vars to store the tile positions
    int i = 0;
    int j = 0;
    int i0 = 0;
    int j0 = 0;
    // Check the first column integrity
    for (int k = l+1; k < d-2; k++)
    {
        printf("FCI %d\n", k);
        if (board[k][l] != d*k+1+l)
        {
            // Find the target tile
            find_tile(d*k+1+l, &i, &j);
            // Find the zero tile
            find_tile(0, &i0, &j0);
            // Check if the target tile is in the last column
            // Move zero tile to the left
            move(board[i0][--j0]);
            // Move zero tile to the target tile row
            while (i0 != i)
            {
                if (i0 < i)
                {
                    move(board[++i0][j0]);
                }
                else
                {
                    break;
                }
            }
            // Swap the tiles
            move(board[i0][++j0]);
            j--;
        }
        else
        {
            // Move zero tile to the target tile row
            while (i0 != i)
            {
                if (i0 < i)
                {
                    move(board[++i0][j0]);
                }
                else
                {
                    break;
                }
            }
            // Move zero tile to the right of target tile
            while (j0 != j+1)
            {
                if (j0 > j+1)
                {
                    move(board[i0][--j0]);
                }
                else
                {
                    break;
                }
            }
        }
        // Move target tile to the proper row
        while (i != k)
        {
            // Move zero tile up
            move(board[--i0][j0]);
            // Move zero tile to the left
            move(board[i0][--j0]);
            // Swap the tiles
            move(board[++i0][j0]);
            i--;
            if (i < 0)
                break;
            if (j == l)
                break;
            // Move zero tile to the left
            move(board[i0][--j0]);
            // Move zero tile up
            move(board[--i0][j0]);
            // Swap the tiles
            move(board[i0][++j0]);
            j--;
            if (j < 0)
                break;
        }
        // Move zero tile to the right of target tile
        while (j0 != j+1)
        {
            if (j0 < j+1)
                move(board[i0][++j0]);
            else
                break;
        }
        // Check if tiles are on the same row
        if (i != i0)
        {
            if (i0 > i)
                move(board[--i0][j0]);
            else
                break;
        }
        // Move the target tile horizontally
        while (j != l)
        {
            // Move zero tile down
            move(board[++i0][j0]);
            // Move zero tile to the left twice
            move(board[i0][--j0]);
            // Move zero tile up
            move(board[--i0][j0]);
            // Swap the tiles
            move(board[i0][++j0]);
            j--;
            if (j < 0)
                break;
        }
    }
    // Move zero tile to the last column
    while (j0 != d-1)
    {
        if (j0 < d-1)
            move(board[i0][++j0]);
        else
            break;
    }
    // Place the first column two last elements
    if (board[d-2][l] != d*(d-2)+1+l && board[d-1][l] != d*(d-1)+1+l)
    {
        printf("Wrong first col!\n");
        // Find the second to last tile
        find_tile(d*(d-2)+1+l, &i, &j);
        // Find the zero tile
        find_tile(0, &i0, &j0);
        // If it on the same row with zero tile
        if (j0 == j)
        {
            // Place zero tile to the right of target
            while (j0 != j+1)
            {
                if (j0 > j+1)
                    move(board[i0][--j0]);
                else
                    break;
            }
            // Move target tile to the last row
            while (i != d-1)
            {
                // Move zero tile down
                move(board[++i0][j0]);
                // Move zero tile left
                move(board[i0][--j0]);
                // Swap the tiles
                move(board[--i0][j0]);
                i++;
                if (i > d-1)
                    break;
                // Move zero tile right
                move(board[i0][++j0]);
                // Move zero tile down
                move(board[++i0][j0]);
            }
        }
        else
        {
            // Check if target tile is on the same column as zero tile
            if (j0 == j)
            {
                // Move zero tile left
                move(board[i0][--j0]);
                // Move zero tile to the same row as target tile
                while (i0 != i)
                {
                    if (i0 > i)
                        move(board[--i0][j0]);
                    else
                        break;
                }
                // Swap the tiles
                move(board[i0][++j0]);
                j--;
            }
            else
            {
                // Move zero tile to the same row as target tile
                while (i0 != i)
                {
                    if (i0 > i)
                    {
                        move(board[--i0][j0]);
                    }
                    else
                    {
                        break;
                    }
                    
                }
                // Move zero tile to the right of target tile
                while (j0 != j+1)
                {
                    if (j0 > j+1)
                        move(board[i0][--j0]);
                    else
                        break;
                }
            }
            // Check the current row
            while (i0 != d-1)
            {
                // Move zero tile down
                move(board[++i0][j0]);
                // Move zero tile left
                move(board[i0][--j0]);
                // Swap the tiles
                move(board[--i0][j0]);
                i++;
                if (i > d-1)
                    break;
                // Move zero tile right
                move(board[i0][++j0]);
                // Move zero tile down
                move(board[++i0][j0]);
            }
            // Move target tile horizontally
            while (j != l)
            {
                // Move zero tile up
                move(board[--i0][j0]);
                // Move zero tile to the left twice
                move(board[i0][--j0]);
                move(board[i0][--j0]);
                // Move zero tile down
                move(board[++i0][j0]);
                // Swap the tiles
                move(board[i0][++j0]);
                j--;
                if (j < 0)
                    break;
            }
        }
        // Find the last tile of the first column
        find_tile(d*(d-1)+1+l, &i, &j);
        // Place zero tile to the right of target tile
        // If target tile is in the last column
        if (j == d-1)
        {
            // Move zero tile one column before last
            while (j0 != d-2)
            {
                if (j0 < d-2)
                    move(board[i0][++j0]);
                else
                    break;
            }
            // Move zero tile to the same row as target tile
            while (i0 != i)
            {
                if (i0 > i)
                    move(board[--i0][j0]);
                else
                    break;
            }
            // Swap the tiles
            move(board[i0][++j0]);
            j--;
        }
        else
        {
            // Move zero tile one column after target tile column
            while (j0 != j+1)
            {
                if (j0 < j+1)
                    move(board[i0][++j0]);
                else
                    break;
            }
            // Move zero tile to the same row as target tile
            while (i0 != i)
            {
                if (i0 > i)
                    move(board[--i0][j0]);
                else
                    break;
            }
        }
        // Move target tile to the last row
        while (i != d-1)
        {
            // Move zero tile down
            move(board[++i0][j0]);
            // Move zero tile left
            move(board[i0][--j0]);
            // Swap the tiles
            move(board[--i0][j0]);
            i++;
            if (i > d-1)
                break;
            // Move zero tile right
            move(board[i0][++j0]);
            // Move zero tile down
            move(board[++i0][j0]);
        }
        // Move target tile to the second column
        while (j != l+1)
        {
            // Move zero tile up
            move(board[--i0][j0]);
            // Move zero tile left twice
            move(board[i0][--j0]);
            move(board[i0][--j0]);
            // Move zero tile down
            move(board[++i0][j0]);
            // Swap the tiles
            move(board[i0][++j0]);
            j--;
            if (j < 0)
                break;
        }
        // Place the target tiles to the proper positions
        // Move zero tile up
        move(board[--i0][j0]);
        // Move zero tile left twice
        move(board[i0][--j0]);
        move(board[i0][--j0]);
        // Move zero tile down
        move(board[++i0][j0]);
        // Move zero tile right
        move(board[i0][++j0]);
    }
}