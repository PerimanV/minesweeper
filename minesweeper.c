#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define MIN_ROWS 4
#define MAX_ROWS 15
#define MIN_COLS 4
#define MAX_COLS 15
#define DIMENSION_SIZE 7
#define COORDS_SIZE 9
#define COMMAND_SIZE 17

void startGame(int debugMode);
void setupNewLevel(char **minefield, char **devBoard, char *retcoords, int *rows, int *cols, int *difficulty, int *hiddenCount, int minesNumber,int *selectRow,int *selectCol,int firstTime,int debugMode);   //starts a new level when hiddenCount is 0
void getPlayerInputDimensions(int *rows, int *cols,int *hiddenCount);
void getPlayerInputDifficulty(int *difficulty);
void placeMinesRandom(char **minefield, int *rows, int *cols, int *selectRow, int *selectCol, int minesNumber);
void Declaration(char **minefield,char **devBoard,int *rows,int *cols);
void printMinefield(char **minefield,char **devBoard,int *rows,int *cols,int debugMode);
void placeNumbers(char **minefield,int *rows ,int *cols);
void getPlayerInputCommand(char **minefield, char **devBoard, char *retcoords, int *selectRow, int *selectCol, int *rows, int *cols, int *difficulty, int *hiddenCount, int firstCommand, int minesNumber);     //asks the player to give a command and then checks the command for what it is
void getPlayerInputCoordinates(int *selectRow, int *selectCol, char *retcoords);
void open(char **minefield,char **devBoard,int *selectRow,int *selectCol, int *rows, int *cols, int *difficulty, int *hiddenCount, char *retcoords, int firstCommand, int minesNumber);
void mark(char **devBoard, int *selectRow, int *selectCol);
void unmark(char **devBoard, int *selectRow, int *selectCol);
void cheat(char **devBoard,char **minefield, int *selectRow, int *selectCol);
int openEmptySquares(char **minefield, char **devBoard, int tempRow, int tempCol, int tempCount,int *rows,int *cols);
void freeMemory(char **minefield, char **devBoard, int *selectRow, int *selectCol, int *rows, int *cols, int *difficulty, int *hiddenCount,char *retcoords);
void youLose(void);
void exitGame(void);

double calculateMinesPercent(int *difficulty, int *rows, int *cols);

int main(int argc, char **argv)
{
    int debugMode;
    char *ret1,*ret2;

    if(argc == 2)
    {
        ret1 = strstr(argv[1],"start");
        ret2 = strstr(argv[1],"debug");
        if(ret1 != NULL && strlen(ret1) == 5)
             startGame(0);
        else if(ret2 != NULL && strlen(ret2) == 5)
            startGame(1);
        else
            printf("Argument must be 'start' or 'debug' \n");

    }
    else
        printf("Argument must be 'start' or 'debug' \n");

    return 0;
}

void startGame(int debugMode)            //starts the game
{
    int *rows, *cols, *difficulty, *selectRow, *selectCol, *hiddenCount;
    int minesNumber, i, firstCommand = 1, firstTime = 0;
    char  **minefield,**devBoard,*retcoords;

    srand(time(NULL));

    selectRow = malloc(sizeof(int));
    if (selectRow == NULL)
        printf("failed to allocate memory");
    selectCol = malloc(sizeof(int));
    if (selectCol == NULL)
        printf("failed to allocate memory");
    rows = malloc(sizeof(int));
    if (rows == NULL)
        printf("failed to allocate memory");
    cols = malloc(sizeof(int));
    if (cols == NULL)
        printf("failed to allocate memory");
    difficulty = malloc(sizeof(int));
    if(difficulty == NULL)
        printf("failed to allocate memory");
    hiddenCount = malloc(sizeof(int));
    if(hiddenCount == NULL)
        printf("failed to allocate memory");
    retcoords=(char *)malloc(sizeof(char) * COORDS_SIZE);
    if(retcoords == NULL)
        printf("failed to allocate memory");


    getPlayerInputDimensions(rows, cols, hiddenCount);

    minefield = (char**)malloc(sizeof(char*) * *rows);

    if(minefield==NULL)
    {
        printf("There was no available memmory \n");
    }
    for (i = 0; i < *rows; i++)
    {
        minefield[i] = (char *) malloc(sizeof(char) * *cols);

        if (minefield[i] == NULL)
        {
            printf("There was no available memmory \n");
        }
    }

    devBoard = (char**)malloc(sizeof(char*) * *rows);

    if(devBoard == NULL) //NULL instead of zero??
    {
        printf("There was no available memmory \n");
    }
    for (i = 0; i < *rows; i++)
    {
        devBoard[i] = (char *) malloc(sizeof(char) * *cols);

        if (devBoard[i] == NULL) {
            printf("There was no available memmory \n");
        }
    }

    printf("\n");

    getPlayerInputDifficulty(difficulty);

    setupNewLevel(minefield, devBoard, retcoords, rows, cols, difficulty, hiddenCount, minesNumber,selectRow,selectCol,0, debugMode);

    printf("You Won!!!\n");

    freeMemory(minefield, devBoard, selectRow, selectCol, rows, cols, difficulty, hiddenCount, retcoords);
}

void setupNewLevel(char **minefield, char **devBoard, char *retcoords, int *rows, int *cols, int *difficulty, int *hiddenCount, int minesNumber,int *selectRow,int *selectCol,int firstTime, int debugMode)    //starts a new level when hiddenCount is 0
{
    int i, firstCommand = 1;

    if(firstTime)
    {
        for(i=0;i<*rows;i++)
        {
            free(minefield[i]);
            free(devBoard[i]);
        }
        free(minefield);
        free(devBoard);

        (*rows) += 5;
        (*cols) += 5;

        devBoard=(char **)malloc(sizeof(char *) * *rows);

        if (devBoard==NULL)
        {
            printf("could not allocate memory");
        }

        for(i =0;i<*rows;i++)
        {
            devBoard[i]=(char *)malloc(sizeof(char *) * *cols);

            if (devBoard[i]==NULL)
            {
                printf("could not allocate memory");
            }
        }

        minefield=(char **)malloc(sizeof(char *) * *rows);
        if (minefield==NULL)
        {
            printf("could not allocate memory");
        }

        for(i=0;i<*rows;i++)
        {
            minefield[i]=(char *)malloc(sizeof(char) * *cols);

            if (minefield==NULL)
            {
                printf("could not allocate memory");
            }
        }

        firstTime = 0;
    }




    minesNumber = calculateMinesPercent(difficulty, rows, cols);

    *hiddenCount = *rows * *cols;

    *hiddenCount = *hiddenCount - minesNumber;

    Declaration(minefield,devBoard,rows,cols);

    printMinefield(minefield, devBoard, rows, cols, debugMode);

    do
    {
        getPlayerInputCommand(minefield, devBoard, retcoords, selectRow, selectCol, rows, cols, difficulty, hiddenCount, firstCommand, minesNumber);

        if(firstCommand)
            firstCommand = 0;

        printMinefield(minefield, devBoard, rows, cols, debugMode);

        if(*hiddenCount == 0)
        {
            if(*rows + 5 <= MAX_ROWS && *cols + 5 <= MAX_COLS)
            {
                printf("You Won!\n Setting up new level...\n");
                setupNewLevel(minefield, devBoard, retcoords, rows, cols, difficulty, hiddenCount, minesNumber,selectRow,selectCol,1, debugMode);
            }
        }

    }while(*hiddenCount);
}

void getPlayerInputDimensions(int *rows, int *cols, int *hiddenCount)       //reads the dimensions of the board from the player
{
    int validInput;
    char dimensions[DIMENSION_SIZE];

    printf("Choose the starting dimensions in MxN format:\n");

    do
    {
        fgets(dimensions, DIMENSION_SIZE, stdin);

        validInput = 1;

        if(dimensions[strlen(dimensions) - 1] == '\n')
            dimensions[strlen(dimensions) - 1] = '\0';

        if(strlen(dimensions) == 3)
        {
            *rows = dimensions[0] - '0';
            *cols = dimensions[2] - '0';

            if(*rows < MIN_ROWS || *rows > MAX_ROWS || *cols < MIN_COLS || *cols > MAX_COLS || dimensions[1] != 'x')
                validInput = 0;

        }
        else if(strlen(dimensions) == 4)
        {
            if(dimensions[1] == 'x')
            {
                *rows = dimensions[0] - '0';
                *cols = ((dimensions[2] - '0') * 10) + (dimensions[3] - '0');

                if(*rows < MIN_ROWS || *rows > MAX_ROWS || *cols < MIN_COLS || *cols > MAX_COLS)
                    validInput = 0;
            }
            else if(dimensions[2] == 'x')
            {
                *rows = ((dimensions[0] - '0') * 10) + (dimensions[1] - '0');
                *cols = dimensions[3] - '0';

                if(*rows < MIN_ROWS || *rows > MAX_ROWS || *cols < MIN_COLS || *cols > MAX_COLS)
                    validInput = 0;
            }
            else
                validInput = 0;
        }
        else if(strlen(dimensions) == 5)
        {
            *rows = ((dimensions[0] - '0') * 10) + (dimensions[1] - '0');
            *cols = ((dimensions[3] - '0') * 10) + (dimensions[4] - '0');

            if(*rows < MIN_ROWS || *rows > MAX_ROWS || *cols < MIN_COLS || *cols > MAX_COLS || dimensions[2] != 'x')
                validInput = 0;
        }

        if(validInput == 0)
            printf("Format must be MxN, with M and N between %d-%d\n", MIN_ROWS, MAX_ROWS);


    }while(!validInput);

    *hiddenCount = *rows * *cols;

}


void getPlayerInputDifficulty(int *difficulty)      // reads the difficulty from the player
{
    *difficulty = 0;

    printf("Choose difficulty:\n");
    printf("1) Easy\n");
    printf("2) Medium\n");
    printf("3) Hard\n");
    printf("4) Impossible\n");

    do
    {
        scanf("%d", difficulty);

        if(*difficulty < 1 || *difficulty > 4)
            printf("Invalid option. Try again.\n");

    }while(*difficulty < 1 || *difficulty > 4);

    while(getchar() != '\n');
}

double calculateMinesPercent(int *difficulty, int *rows, int *cols)   // calculate percentage of mines in  minefield
{
    switch(*difficulty)
    {
        case 1:
            return ( (*rows * *cols) * 0.1 );
            break;

        case 2:
            return ( (*rows * *cols) * 0.15 );
            break;

        case 3:
            return ( (*rows * *cols) * 0.2 );
            break;

        case 4:
            return ( (*rows * *cols) * 0.25 );
            break;

        default:
            return 0;
            break;
    }
}

void Declaration(char **minefield,char **devBoard,int *rows,int *cols)       //set the all boxes of devBoard to hidden and all boxes of minefield to '.'
{
    int i, j;

    for(i = 0; i < *rows; i++)
    {
        for( j = 0; j < *cols; j++)
        {
            devBoard[i][j] = 'h';
            minefield[i][j] = '.';
        }
    }
}

void getPlayerInputCommand(char **minefield, char **devBoard, char *retcoords, int *selectRow, int *selectCol, int *rows, int *cols, int *difficulty, int *hiddenCount, int firstCommand, int minesNumber)      //asks the player to give a command and then checks the command for what it is
{
    char command[COMMAND_SIZE], chpar = '(';


    printf("give the command and coords: \n");

    do
    {
        fgets(command, COMMAND_SIZE, stdin);

        if (command[strlen(command) - 1] == '\n')
            command[strlen(command) - 1] = '\0';

        if(strstr(command,"exit")!= NULL)
        {
            break;
        }
        retcoords = strchr(command, chpar);

        getPlayerInputCoordinates(selectRow, selectCol, retcoords);

        if((*selectRow<0 || *selectCol<0) || (*selectRow >= *rows || *selectCol >= *cols))
        {
            printf("Wrong input , insert coordinates again \n");
        }

    }while((*selectRow<0 || *selectCol<0) || (*selectRow >= *rows || *selectCol >= *cols));



    if(firstCommand)
    {
        placeMinesRandom(minefield, rows, cols, selectRow, selectCol, minesNumber);
        placeNumbers(minefield,rows ,cols);
    }


    if(strstr(command,"open")!= NULL)
    {
        open(minefield, devBoard, selectRow, selectCol, rows, cols, difficulty, hiddenCount, retcoords, firstCommand, minesNumber);
        printf("\n");
    }
    else if(strstr(command, "unmark") != NULL)
    {
        printf("unmarked \n");

        unmark(devBoard, selectRow, selectCol);
    }
    else if(strstr(command,"mark")!= NULL)
    {
        mark(devBoard, selectRow, selectCol);
    }

    else if(strstr(command,"cheat")!= NULL)
    {
        cheat(devBoard,minefield,selectRow,selectCol);
    }
    else if(strstr(command,"exit")!= NULL)
    {
        freeMemory(minefield, devBoard, selectRow, selectCol, rows, cols, difficulty, hiddenCount, retcoords);
        exitGame();
    }
    else
    {
        printf("Wrong input command \n");
    }

}

void placeMinesRandom(char **minefield, int *rows, int *cols, int *selectRow, int *selectCol, int minesNumber)       // place random mines on the board
{
    int minesNumberCount = minesNumber;
    int randomRow, randomCol;

    while(minesNumberCount)
    {
        randomRow = rand() % *rows;
        randomCol = rand() % *cols;

        if(minefield[randomRow][randomCol] != '@' && (randomRow != *selectRow || randomCol != *selectCol))
        {
            minefield[randomRow][randomCol] = '@';
            minesNumberCount--;
        }
    }
}

void placeNumbers (char **minefield, int *rows, int *cols)           //place all the numbers in the board
{
    int i, j, k, l, countMines, upperLimit, downLimit, leftLimit, rightLimit;

    for(i = 0; i < *rows; i++)
    {
        for(j = 0; j < *cols; j++)
        {
            if(minefield[i][j] == '.')
            {
                countMines = 0;
                if(i == 0)
                    upperLimit = 0;
                else
                    upperLimit = i - 1;

                if(i == *rows - 1)
                    downLimit = *rows - 1;
                else
                    downLimit = i + 1;

                if(j == 0)
                    leftLimit = 0;
                else
                    leftLimit = j - 1;

                if(j == *cols - 1)
                    rightLimit = *cols - 1;
                else
                    rightLimit = j + 1;

                for(k = upperLimit; k <= downLimit; k++)
                {
                    for(l = leftLimit; l <= rightLimit; l++)
                    {
                        if(minefield[k][l] == '@')
                            countMines++;
                    }
                }
                if(countMines)
                    minefield[i][j] = countMines + '0';
            }
        }
    }
}

void getPlayerInputCoordinates(int *selectRow, int *selectCol, char *retcoords)
{
    int a, b;

    if(retcoords[strlen(retcoords) - 1] == '\n')
        retcoords[strlen(retcoords) - 1] = '\0';

    if(strlen(retcoords) == 5)
    {
        *selectRow = retcoords[1] - '0';
        *selectCol = retcoords[3] - '0';
    }
    else if(strlen(retcoords) == 6)
    {
        if(retcoords[2] == ',')
        {
            *selectRow = retcoords[1] - '0';
            *selectCol = ((retcoords[3] - '0') * 10) + (retcoords[4] - '0');
        }
        else if(retcoords[3] == ',')
        {
            *selectRow = ((retcoords[1] - '0') * 10) + (retcoords[2] - '0');
            *selectCol = retcoords[4] - '0';
        }
    }
    else if(strlen(retcoords) == 7)
    {
        *selectRow = ((retcoords[1] - '0') * 10) + (retcoords[2] - '0');
        *selectCol = ((retcoords[4] - '0') * 10) + (retcoords[5] - '0');
    }
    else
    {
        *selectRow = 0;
        *selectCol = 0;
    }

    --*selectRow;
    --*selectCol;
}

void open(char **minefield,char **devBoard,int *selectRow,int *selectCol, int *rows, int *cols, int *difficulty, int *hiddenCount, char *retcoords, int firstCommand, int minesNumber)
{
    int i, j, tempRow, tempCol, tempCount;

    if(devBoard[*selectRow][*selectCol] == 's')
    {
        printf("This square is already open\n");
    }
    else if(devBoard[*selectRow][*selectCol] == 'm')
    {
        printf("This square is marked\n");
    }
    else
    {
        if(minefield[*selectRow][*selectCol] == '@')
        {
            printf("mine clicked! \n");
            freeMemory(minefield, devBoard, selectRow, selectCol, rows, cols, difficulty, hiddenCount, retcoords);
            youLose();
        }
        else if(minefield[*selectRow][*selectCol] >= '1' && minefield[*selectRow][*selectCol] <= '9' && devBoard[*selectRow][*selectCol] == 'h')
        {
            devBoard[*selectRow][*selectCol] = 's';
            --*hiddenCount;
        }
        else if(minefield[*selectRow][*selectCol] == '.' && devBoard[*selectRow][*selectCol] == 'h')
        {
            tempRow = *selectRow;
            tempCol = *selectCol;
            tempCount = *hiddenCount;

            tempCount = openEmptySquares(minefield, devBoard, tempRow, tempCol, tempCount,rows,cols);

            *hiddenCount = tempCount;
        }
    }
}

int openEmptySquares(char **minefield, char **devBoard, int tempRow, int tempCol, int tempCount,int *rows,int *cols)
{
    int upperLimit, downLimit, leftLimit, rightLimit, i, j;

    if(tempRow == 0)
        upperLimit = 0;
    else
        upperLimit = tempRow - 1;

    if(tempRow == *rows - 1)
        downLimit = *rows - 1;
    else
        downLimit = tempRow + 1;

    if(tempCol == 0)
        leftLimit = 0;
    else
        leftLimit = tempCol - 1;

    if(tempCol == *cols - 1)
        rightLimit = *cols - 1;
    else
        rightLimit = tempCol + 1;

    for(i = upperLimit; i <= downLimit; i++)
    {
        for(j = leftLimit; j <= rightLimit; j++)
        {
            if(minefield[i][j] >= '1' && minefield[i][j] <= '9' && devBoard[i][j] == 'h')
            {
                devBoard[i][j] = 's';
                tempCount--;
            }
            else if(minefield[i][j] == '.' && devBoard[i][j] == 'h')
            {
                devBoard[i][j] = 's';
                tempCount--;
                tempCount = openEmptySquares(minefield, devBoard, i, j, tempCount, rows, cols);
            }
        }
    }
    return tempCount;
}

void mark(char **devBoard, int *selectRow, int *selectCol)
{
    if(devBoard[*selectRow][*selectCol] == 'm')
        printf("This square is already marked\n");
    else if(devBoard[*selectRow][*selectCol] == 's')
        printf("This square is already opened\n");
    else
        devBoard[*selectRow][*selectCol] = 'm';

}

void unmark(char **devBoard, int *selectRow, int *selectCol)
{
    if(devBoard[*selectRow][*selectCol]=='m')
    {
        devBoard[*selectRow][*selectCol]='h';
    }
    else
    {
        printf("You can't unmark an already unmarked square!\n");
    }

}

void cheat(char **devBoard,char **minefield, int *selectRow, int *selectCol)
{
    static int cheatsTotal = 3;

    if (devBoard[*selectRow][*selectCol]== 's')
    {
        printf("Cannot use cheat on an opened square!\n");
    }
    else if (devBoard[*selectRow][*selectCol]== 'm')
    {
        printf("Cannot use cheat on a marked square!\n");
    }
    else
    {
        if(cheatsTotal > 0)
        {
            printf("behind this square is : ( %c ) \n", minefield[*selectRow][*selectCol]);
            cheatsTotal--;
            printf("You have %d cheats left!\n", cheatsTotal);
        }
        else
            printf("You have no cheats remaining!\n");
    }
}

void exitGame(void)
{
    printf("Exiting the game\n");
    exit(0);
}

void freeMemory(char **minefield, char **devBoard, int *selectRow, int *selectCol, int *rows, int *cols, int *difficulty, int *hiddenCount,char *retcoords)
{
    int i;


    for(i = 0; i < *rows; i++)
    {
        free(minefield[i]);
    }
    free(minefield);


    for(i=0;i<*rows;i++)
    {
        free(devBoard[i]);
    }
    free(devBoard);

    free(selectRow);
    free(selectCol);
    free(rows);
    free(cols);
    free(difficulty);
    free(hiddenCount);
    free(retcoords);
}

void youLose(void)
{
    printf("You clicked on a mine. BOOM!");
    exit(0);
}

void printMinefield(char **minefield, char **devBoard, int *rows, int *cols,int debugMode)
{
    int i,j,k;

    for(k = 0; k < debugMode + 1; k++)
    {
        printf("    ");
        for(i = 1; i <= *cols; i++)
        {
            if(i<9)
            {
                printf("%d ", i);
            }
            else
            {
                printf("%d", i);
            }
        }


        printf("\n");

        for(i = 1; i <= (*cols * 2) + 3; i++)
            printf("-");

        printf("\n");

        for(i=0;i<*rows;i++)
        {
            if(i <= 8)
                printf(" %d| ", i+1);
            else
                printf("%d| ", i+1);

            if(debugMode == 0)
            {
                for (j = 0; j < *cols; j++)
                {
                    if (devBoard[i][j] == 's')
                        printf("%c ", minefield[i][j]);
                    else if (devBoard[i][j] == 'h')
                        printf("# ");
                    else
                        printf("@ ");
                }
                printf("\n");
            }
            else if(debugMode == 1)
            {
                if(k == 0)
                {
                    for (j = 0; j < *cols; j++)
                    {
                        printf("%c ", minefield[i][j]);
                    }
                    printf("\n");
                }
                else if(k == 1)
                {
                    for (j = 0; j < *cols; j++)
                    {
                        printf("%c ", devBoard[i][j]);
                    }
                    printf("\n");
                }
            }
        }
        printf("\n");
    }
}