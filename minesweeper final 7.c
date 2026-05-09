#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>

#define MAX_SIZE 16

int rows, cols, mines;

char board[50][50];
char visible[50][50];

int flagged[50][50];
int firstMove = 1;

time_t startTime;

void initializeBoard()
 {
    for (int i = 0; i < rows; i++) 
	{
        for (int j = 0; j < cols; j++) 
		{
            board[i][j] = '0';
            visible[i][j] = '-';
            flagged[i][j] = 0;
        }
    }
}

void placeMines(int excludeRow, int excludeCol)
 {
    int placed = 0;
    srand(time(NULL));
    
    while (placed < mines)
	 {
        int r = rand() % rows;
        int c = rand() % cols;
        
        if (board[r][c] != '*' && !(r == excludeRow && c == excludeCol)) 
		{
            board[r][c] = '*';
            placed++;
            
      
	  
	  
	    }
    }
}

void calculateNumbers()
 {
    int dx[] = {-1, -1, -1, 0, 0, 1, 1, 1};
    int dy[] = {-1, 0, 1, -1, 1, -1, 0, 1};
    
    for (int i = 0; i < rows; i++)
	
	 {
        for (int j = 0; j < cols; j++) 
		{
            if (board[i][j] == '*') continue;
            
            int count = 0;
            for (int k = 0; k < 8; k++) 
			{
                int ni = i + dx[k];
                int nj = j + dy[k];
                
                if (ni >= 0 && ni < rows && nj >= 0 && nj < cols && board[ni][nj] == '*')
				 {
                    count++;
                }
            }
            board[i][j] = '0' + count;
            
        }
        
    }
}

void displayBoard()

 {
    printf("\n   ");
    for (int j = 0; j < cols; j++)
	 {
        printf("%2d ", j);
    }
    printf("\n");
    
    for (int i = 0; i < rows; i++)
	 {
        printf("%2d ", i);
        for (int j = 0; j < cols; j++)
		 {
            if (flagged[i][j])
			 {
                printf(" F ");
            } else {
                printf(" %c ", visible[i][j]);
            }
        }
        printf("\n");
    }
}

void revealCell(int r, int c) 
{
    if (r < 0 || r >= rows || c < 0 || c >= cols || visible[r][c] != '-' || flagged[r][c])
	 {
        return;
    }
    
    visible[r][c] = board[r][c];
    
    if (board[r][c] == '0') {
        int dx[] = {-1, -1, -1, 0, 0, 1, 1, 1};
        int dy[] = {-1, 0, 1, -1, 1, -1, 0, 1};
        
        for (int k = 0; k < 8; k++)
		 {
            revealCell(r + dx[k], c + dy[k]);
            
        }
    }
}

int checkWin()
 {
    for (int i = 0; i < rows; i++)
	
	 {
        for (int j = 0; j < cols; j++)
		 {
            if (board[i][j] != '*' && visible[i][j] == '-' && !flagged[i][j]) {
                return 0;
            }
        }
    }
    return 1;
}

void revealAllMines() {
    for (int i = 0; i < rows; i++)
	 {
        for (int j = 0; j < cols; j++) 
		{
            if (board[i][j] == '*') 
			{
                visible[i][j] = '*';
            }
        }
    }
}

void saveGame()
 {
    FILE *file = fopen("minesweeper_save.txt", "w");
    
    if (file == NULL)
	 {
        printf("Error saving game!\n");
        return;
    }
    
    fprintf(file, "%d %d %d\n", rows, cols, mines);
    fprintf(file, "%d\n", firstMove);
    fprintf(file, "%ld\n", (long)(time(NULL) - startTime));
    
    for (int i = 0; i < rows; i++) 
	{
        for (int j = 0; j < cols; j++) 
		
		{
            fprintf(file, "%c ", board[i][j]);
        }
        fprintf(file, "\n");
    }
    
    for (int i = 0; i < rows; i++)
	 {
        for (int j = 0; j < cols; j++) {
            fprintf(file, "%c ", visible[i][j]);
        }
        fprintf(file, "\n");
    }
    
    for (int i = 0; i < rows; i++)
	
	 {
        for (int j = 0; j < cols; j++) {
            fprintf(file, "%d ", flagged[i][j]);
        }
        fprintf(file, "\n");
    }
    
    fclose(file);
    printf("Game saved successfully!\n");
}

int loadGame() {
    FILE *file = fopen("minesweeper_save.txt", "r");
    if (file == NULL) {
        return 0;
    }
    
    long elapsedTime;
    
    fscanf(file, "%d %d %d", &rows, &cols, &mines);
    
    fscanf(file, "%d", &firstMove);
    
    fscanf(file, "%ld", &elapsedTime);
    
    for (int i = 0; i < rows; i++)
	 {
        for (int j = 0; j < cols; j++)
		 {
            fscanf(file, " %c", &board[i][j]);
        }
    }
    
    for (int i = 0; i < rows; i++) 
	
	{
        for (int j = 0; j < cols; j++)
		
		 {
            fscanf(file, " %c", &visible[i][j]);
        }
    }
    
    for (int i = 0; i < rows; i++)
	
	 {
        for (int j = 0; j < cols; j++)
		
		
		 {
            fscanf(file, "%d", &flagged[i][j]);
        }
    }
    
    fclose(file);
    startTime = time(NULL) - elapsedTime;
    printf("Game loaded successfully!\n");
    return 1;
}

int main() 

{
    int choice, loadChoice;
    
    printf("WELCOME TO MINESWEEPER GAME\n");
    printf("Do you want to load a saved game? (1=Yes, 0=No): ");
    scanf("%d", &loadChoice);
    
    if (loadChoice == 1 && loadGame())
	 {
        printf("Game loaded!\n");
    } else {
        printf("\nSelect Difficulty:\n");
        
        printf("1. Easy (5x5, 5 mines)\n");
        
        printf("2. Medium (9x9, 10 mines)\n");
        
        printf("3. Hard (16x16, 40 mines)\n");
        
        printf("Enter choice: ");
        
        scanf("%d", &choice);
        
        switch (choice)
		 {
            case 1:
                rows = cols = 5;
                mines = 5;
                break;
                
            case 2:
                rows = cols = 9;
                mines = 10;
                break;
                
            case 3:
                rows = cols = 16;
                mines = 40;
                break;
                
            default:
                rows = cols = 5;
                mines = 5;
        }
        
        initializeBoard();
        startTime = time(NULL);
    }
    
    printf("\nInstructions:\n");
    
    printf("1. Enter row and column to reveal a cell\n");
    
    printf("2. Enter -1 as row to flag/unflag a cell\n");
    
    printf("3. Enter -2 as row to save and exit\n\n");
    
    while (1)
	 {
        displayBoard();
        
        time_t currentTime = time(NULL);
        int elapsedSeconds = (int)(currentTime - startTime);
        printf("\nTime: %d seconds\n", elapsedSeconds);
        
        int r, c;
        printf("Enter row: ");
        scanf("%d", &r);
        
        if (r == -2) {
            saveGame();
            printf("Game saved. Exiting...\n");
            break;
        }
        
        printf("Enter column: ");
        scanf("%d", &c);
        
        if (r < -1 || r >= rows || c < 0 || c >= cols) {
            printf("Invalid input! Try again.\n");
            continue;
        }
        
        if (r == -1) {
            if (visible[c][0] == '-') 
			{
                flagged[c][0] = !flagged[c][0];
                
                printf("Cell %s!\n", flagged[c][0] ? "flagged" : "unflagged");
            } else {
                printf("Cannot flag revealed cell!\n");
            }
            continue;
        }
        
        if (firstMove) {
            placeMines(r, c);
            calculateNumbers();
            firstMove = 0;
        }
        
        if (flagged[r][c])
		 {
            printf("Cell is flagged! Unflag it first.\n");
            continue;
        }
        
        if (board[r][c] == '*') 
		{
            revealAllMines();
            
            displayBoard();
            
            printf("\n GAME OVER! You hit a mine! \n");
            
            printf("Time taken: %d seconds\n", elapsedSeconds);
            
            break;
        }
        
        revealCell(r, c);
        
        if (checkWin())
		 {
            displayBoard();
            printf("\n CONGRATULATIONS! You won! \n");
            
            printf("Time taken: %d seconds\n", elapsedSeconds);
            
            break;
        }
    }
    
    return 0;
}
