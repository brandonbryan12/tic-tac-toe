#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <time.h>

// Tictactoe struct
typedef struct ticTacToeDescription{
    char square[3][10];
    char rawData[9];
    char player;
    char user;
    char AI;
    unsigned short int turnCount;
    unsigned short int numberPlayers;
    unsigned short int difficulty;
}ticTacToeBoard;

// Initialization Functions
void setUpGame(ticTacToeBoard*, char*, unsigned short int*);
void coinFlip(char*);
void initBoard(char*);
char playerSelect(void);
int numberOfPlayers(void);
int findDifficulty(ticTacToeBoard*);

// Turn Functions
void switchPlayer(char*);
void renderBoard(char*, unsigned short int);
void displayError(void);
int runGame(ticTacToeBoard*, unsigned short int*, unsigned short int*, unsigned short int, char*, unsigned short int*);
int findWinningMove(ticTacToeBoard*, char*, int);
int isWinningPosition(ticTacToeBoard*, char*, char*, int);
int replay(ticTacToeBoard*, unsigned short int*);
int userTurn(ticTacToeBoard*, unsigned short int, char*, unsigned short int);
int winPositionInfo(ticTacToeBoard*, char*);

// AI Functions
void noviceAI(char*);
void intermediateAI(ticTacToeBoard*, char*, int);
int expertAI(ticTacToeBoard*, unsigned short int*, unsigned short int*, unsigned short int, char*, unsigned short int*, int);
int aiTurn(ticTacToeBoard*, unsigned short int*, unsigned short int*, unsigned short int, unsigned short int, char*, unsigned short int*);
int startingPosition(char*, unsigned short int*);
int aiCheckWin(ticTacToeBoard*, char*, int);

int main()
{
    unsigned short int corners[4] = {0,2,6,8};
    unsigned short int sides[4] = {1,3,5,7};
    unsigned short int center = 4;

    // Pointer for tictactoe structure
    ticTacToeBoard *selector = malloc(sizeof(*selector));
    unsigned short int* intPtr = (unsigned short int*) (selector+5);
    char *charPtr = (char*) selector;

    *intPtr = 1;

    while(*intPtr) {
        // Set up game
        setUpGame(selector, charPtr, intPtr);

        // Run Game
        runGame(selector, corners, sides, center, charPtr, intPtr);

        // Ask to replay
        replay(selector, intPtr);
    }

    printf("                          Thanks for playing!\n");

    // Exits program
    exit(0);
}

/**
 * Set up game with default values
 */
void setUpGame (ticTacToeBoard* selector, char *charPtr, unsigned short int* intPtr)
{
    // Sets turn number to 0
    *intPtr = 1;

    // Stores number of players as "num_players"
    intPtr[1] = numberOfPlayers();

    // Stores selected player as "user"
    charPtr[41] = playerSelect();

    // If there is an AI, ask for difficulty
    if(intPtr[1] == 1)
        intPtr[2] = findDifficulty(selector);

    // Flip coin to see who goes first
    coinFlip(charPtr);

    // Create a fresh board
    initBoard(charPtr);
}

/**
 * Flips coin to choose who goes first
 */
void coinFlip(char *charPtr)
{
    // Var to be set to a random int
    int random;

    // Creates new seed set to value of time
    srand(time(NULL));

    // Rolls a 0 or 1
    random = rand() %2;

    printf("\n             We are now flipping a coin to see who goes first...\n");

    switch(random)
    {
        case 0:
            printf("                             ");
            printf("Player X wins!\n\n");
            charPtr[40] = 'X';
            break;
        case 1:
            printf("                             ");
            printf("Player O wins!\n\n");
            charPtr[40] = 'O';
            break;
        default:
            displayError();
            break;
    }
}

/**
 * Fills tictactoe structure with layout for new game
 */
void initBoard(char* charPtr)
{
    // Looping variable
    int i;

    // Looping variable
    int k;

    // Fills square with shell
    for(i = 0; i < 3; i++)
    {
        for(k = 0; k < 9; k += 3)
        {
            *(charPtr + (i * 10) + k) = '[';
            *(charPtr + (i * 10) + (k + 1)) = ' ';
            *(charPtr + (i * 10) + (k + 2)) = ']';
            *(charPtr + (i * 10) + (k + 3)) = '\n';
        }
    }

    // Resets raw data to empty
    for(i = 0; i < 9; i++)
        charPtr[30 + i]=' ';
}

/**
 * Runs all main game functions
 */
int runGame(ticTacToeBoard* selector, unsigned short int* corners, unsigned short int* sides, unsigned short int center, char* charPtr, unsigned short int* intPtr)
{
    unsigned short int aiPlay;
    unsigned short int win = 0;

    //Checks if AI is playing and sets respective character value
    if(intPtr[1] == 1)
        aiPlay = 1;
    else aiPlay=0;


    if(aiPlay == 1){
        // Sets user and AI character values
        if(charPtr[41] == 'X')
            charPtr[42] = 'O';
        else charPtr[42]= 'X';

    }

    switch(aiPlay)
    {
        //Runs game without AI
        case 0:
            while(win == 0)
            {
                // Runs user turn and returns a win of 0 or 1
                win = userTurn(selector, win, charPtr, aiPlay);

                // Increases turn by 1
                intPtr[0] = intPtr[0] + 1;
            }
            break;
        // Runs game with AI
        case 1:
            while(win == 0)
            {
                if(charPtr[42] == charPtr[40])
                {
                    // Runs AI turn
                    win = aiTurn(selector, corners, sides, center, win, charPtr, intPtr);

                    // Increases turn by 1
                    intPtr[0] = intPtr[0] + 1;
                    if(win == 1)
                        break;
                }
                win = userTurn(selector, win, charPtr, aiPlay);

                // Increases turn by 1
                intPtr[0] = intPtr[0] + 1;
            }
            break;
        default:
            displayError();
            break;
    }

    // Print board to console
    renderBoard(charPtr, win);

    return 0;
}

/**
 * Runs user turn
 */
int userTurn(ticTacToeBoard* selector, unsigned short int win, char* charPtr, unsigned short int aiPlay)
{
    int i = 0;
    int playSpace = 0;

    renderBoard(charPtr, win);

    while(i == 0)
    {
        if(aiPlay == 1)
            printf("                       Please choose a space to play.\n");
        else printf("                Player %c: Please choose a space to play.\n", charPtr[40]);

        printf("\n                                0  1  2");
        printf("\n                                3  4  5");
        printf("\n                                6  7  8\n\n");

        printf("Selection: ");
        scanf("%i", &playSpace);
        printf("\n");

        if(((int)playSpace >= 0) && ((int)playSpace <= 8))
        {
            if(charPtr[30 + playSpace] == ' ')
            {
                charPtr[30 + playSpace] = charPtr[40];
                i++;
            }
            else
            {
                printf("               This space is already occupied. Try again.\n");
                renderBoard(charPtr, win);
            }
        }
        else
        {
            printf("                      *Invalid input. Try again*\n");
            renderBoard(charPtr, win);
        }
    }

    // Checks if winning position
    if(winPositionInfo(selector, charPtr))
        return 1;

    // Switches player
    switchPlayer(charPtr);
    printf("\n\n");

    return 0;
}

/**
 * Prints board to console
 */
void renderBoard(char *charPtr, unsigned short int win)
{
    //Looping variables
    int i;
    int k;
    int j = 0;

    // Format to center
    printf("                             ");

    if(win == 0)
        printf("Current Board\n\n");
    else printf(" Final Board\n\n");

    //Fills board with raw data
    for(i = 0; i < 3; i++)
    {
        for(k = 1; k <= 7; k += 3)
        {
            *(charPtr + (i * 10) + k) = *(charPtr + 30 + j);
            j++;
        }
    }
    printf("                               ");

    //Prints board
    for(i = 0; i < 10; i++)
    {
        for(k = 0; k < 3; k++)
        {
            printf("%c", *(charPtr + (i * 3) + k));
            if(*(charPtr + (i * 3) + k) == '\n')
                printf("                               ");
        }
    }

    printf("\n");
}

/**
 * Displays how the position was won
 */
int winPositionInfo(ticTacToeBoard* selector, char* charPtr)
{
    int test = 0;

    switch(isWinningPosition(selector, charPtr, charPtr, test))
    {
        //No Win
        case 0: return 0;

        //Draw
        case 1: printf("                               **DRAW**\n\n"); return 1;

        //Horizontal
        case 10: printf("             **Horizontal win on the first row for Player %c!**\n\n", charPtr[40]); return 1;
        case 13: printf("             **Horizontal win on the second row for Player %c!**\n\n",charPtr[40]); return 1;
        case 16: printf("             **Horizontal win on the third row for Player %c!**\n\n",charPtr[40]); return 1;

        //Vertical
        case 20: printf("            **Vertical win on the first collumn for Player %c!**\n\n",charPtr[40]); return 1;
        case 21: printf("            **Vertical win on the second collumn for Player %c!**\n\n",charPtr[40]); return 1;
        case 22: printf("            **Vertical win on the third collumn for Player %c!**\n\n",charPtr[40]); return 1;

        //Diagonal
        case 31: printf("                **Descening diagonal win for Player %c!**\n\n",charPtr[40]); return 1;
        case 32: printf("                **Ascending diagonal win for Player %c!**\n\n",charPtr[40]); return 1;

        default: displayError(); break;
    }

    return 0;
}

/**
 * Finds a winning move for AI
 */
int findWinningMove(ticTacToeBoard* selector, char* charPtr, int defense)
{
    char testPosition[10];
    int i;
    int test;

    for(i = 0; i < 9; i++)
    {
        if(charPtr[30 + i] == ' ')
        {
            strcpy(testPosition, selector->rawData);
            testPosition[i] = charPtr[40];
            test = 1;
                if(isWinningPosition(selector, charPtr, testPosition, test))
                {
                    if(defense == 1)
                        switchPlayer(charPtr);
                    test = 0;
                    charPtr[30 + i] = charPtr[40];
                    return i;
                }
        }
    }

    return 10;
}

/**
 * Checks if a there is a win and returns corresponding value to position won
 */
int isWinningPosition(ticTacToeBoard* selector, char* charPtr, char* test_position, int test)
{

    int i; //Looping variable

    if(test == 1)
    {
        //Test for horizontal win
        for(i = 0; i <= 6; i += 3)
        {
            if(test_position[i] == charPtr[40] && test_position[1 + i] == charPtr[40] &&
            test_position[2 + i] == charPtr[40])
                return (10 + i);
        }

        //Test for vertical win
        for(i = 0; i < 3; i++)
        {
            if (test_position[i] == charPtr[40] && test_position[3 + i] == charPtr[40] &&
            test_position[6 + i] == charPtr[40])
                return (20 + i);
        }

        //Test for diagonal win
        if(test_position[4] == charPtr[40])
        {
            if(test_position[0] == charPtr[40] && test_position[8] == charPtr[40])
                return 31;
            if(test_position[2] == charPtr[40] && test_position[6] == charPtr[40])
                return 32;
        }

    }
    else
    {
        //Test for horizontal win
        for(i = 0; i <= 6; i += 3){
            if(charPtr[30 + i] == charPtr[40] && charPtr[31 + i] == charPtr[40] &&
            charPtr[32 + i] == charPtr[40])
                return (10 + i);
        }

        //Test for vertical win
        for(i = 0; i < 3; i++)
        {
            if (charPtr[30 + i] == charPtr[40] && charPtr[33 + i] == charPtr[40] &&
            charPtr[36 + i] == charPtr[40])
                return (20 + i);
        }

        //Test for diagonal win
        if(charPtr[34] == charPtr[40])
        {
            if(charPtr[30] == charPtr[40] && charPtr[38] == charPtr[40])
                return 31;
            if(charPtr[32] == charPtr[40] && charPtr[36] == charPtr[40])
                return 32;
        }

        //Test for Draw
        for(i = 0; i < 9; i++)
        {
            if(charPtr[30 + i] == ' ')
                return 0;
        }

        //If there are no empty spaces return a draw case
        if(i == 9)
            return 1;
    }

    return 0;
}

/**
 * Asks player for number of human players and returns that amount
 */
int numberOfPlayers()
{
    int playerAnswer = 0;

    while(playerAnswer == 0){
        printf("               Number of human players this game (1 or 2): ");
        scanf("%i", &playerAnswer);
        printf("\n");
        if(playerAnswer != 1 && playerAnswer != 2)
        {
            printf("                         Invalid option. Try again.\n\n");
            playerAnswer = 0;
        // Clears stdin
        } while(getchar() != '\n');
    }

    return playerAnswer;
}

/**
 * Asks player if they would like to be X or O
 */
char playerSelect(void)
{
    char player = 'n';

    while(player=='n')
    {
        printf("                       Select your player (X or O): ");
        scanf(" %c", &player);

        if(player != 'X' && player!= 'x' && player != 'O' && player!= 'o'){
            printf("                          Invalid input. Try again\n\n");
            player = 'n';

            // Clears stdin
            while(getchar() != '\n');
        }
    // Clears stdin
    } while(getchar() != '\n');

    if(player == 'o'){
        player = 'O';
    }
    else if(player == 'x'){
        player = 'X';
    }

    return player;
}

/**
 * Asks user for difficulty of AI
 */
int findDifficulty(ticTacToeBoard* selector)
{
    int i = 0;
    unsigned short int* difficultyPtr = (unsigned short int*) (selector + 4);

    while(i == 0)
    {
        printf("\n                                 Difficulty\n                   Novice[1]   Intermediate[2]   Expert[3]\nSelection: ");
        scanf("%hu", difficultyPtr);

        switch(*difficultyPtr)
        {
            case 1:
                return 1;
            case 2:
                return 2;
            case 3:
                return 3;
            default:
                printf("                         Invalid input. Try again. \n");
                break;
        }
    }

    return 0;
}

/**
 * Returns 1 if want to replay. Return 2 if not
 */
int replay(ticTacToeBoard* selector, unsigned short int* intPtr)
{
    char replay = 'n';

    while(replay != 'Y' && replay != 'N')
    {
        // Clear an existing '\n'
        getchar();
        printf("            Good game! Do you wish to play again? (Y or N): ");
        scanf("%c",&replay);
        getchar();
        printf("\n");

        if(replay == 'Y' || replay == 'y')
        {
            *intPtr = 1;
            return 1;
        }
        else if(replay == 'N' || replay == 'n')
        {
            *intPtr = 0;
            return 0;
        }
        else printf("                         Invalid input. Try again.\n");
    }

    return 0;
}

/**
 * Displays an error
 */
void displayError(void)
{
    printf("Some unknown error occurred.");
}

/**
 * Switches player
 */
void switchPlayer(char* charPtr)
{

    if(charPtr[40] == 'X')
        charPtr[40] = 'O';
    else charPtr[40] = 'X';
}

/**
 * Runs AI turn
 */
int aiTurn(ticTacToeBoard* selector, unsigned short int* corners, unsigned short int* sides, unsigned short int center, unsigned short int win, char* charPtr, unsigned short int* intPtr)
{
    int defense = 1;

    // Checks difficulty chosen when setting up game
    switch(intPtr[2])
    {
        case 1:
            noviceAI(charPtr);
            break;
        case 2:
            intermediateAI(selector, charPtr, defense);
            break;
        case 3:
            expertAI(selector, corners, sides, center, charPtr, intPtr, defense);
            break;
    }

    // Checks if winning position
    if(winPositionInfo(selector, charPtr))
        return 1;

    //Switches player
    switchPlayer(charPtr);
    printf("\n\n");

    return 0;
}

/**
 * Novice: Plays random empty space unless the user can create a win next turn
 */
void noviceAI(char* charPtr)
{
    //Looping variables
    int i;
    int k = 0;

    int random;
    int play;
    int emptySpaces[9];

    //Fills all empty positions into array of integers
    for(i = 0; i < 9; i++)
    {
        if(charPtr[30 + i] == ' ')
        {
            emptySpaces[k] = i;
            k++;
        }
    }

    // Sets new seed
    srand(time(NULL));

    // Rolls random int
    random = rand() % k;
    play = emptySpaces[random];

    //Pulls a random int from array and plays in that position
    charPtr[30+play] = charPtr[40];

    printf("\n                        **AI played position %i**", play);
}

/**
 * Intermediate: Plays random empty space unless the user can create a win next turn
 */
void intermediateAI(ticTacToeBoard* selector, char* charPtr, int defense)
{
    //Looping variables
    int i;
    int k = 0;

    int random;
    int play;
    int emptySpaces[9];

    play = aiCheckWin(selector, charPtr, defense);

    if(play == 20){
        //Fills all empty positions into array of integers
        for(i = 0; i < 9; i++)
        {
            if(charPtr[30 + i] == ' ')
            {
                emptySpaces[k] = i;
                k++;
            }
        }

        // Sets new seed
        srand(time(NULL));

        // Rolls random int
        random = rand() % k;
        play = emptySpaces[random];

        //Pulls a random int from array and plays in that position
        charPtr[30 + play] = charPtr[40];

        printf("\n                        **AI played position %i**", play);
    }
}

/**
 * Expert: Intermediate but plays corner first and counters a corner with center
 */
int expertAI(ticTacToeBoard* selector, unsigned short int* corners, unsigned short int* sides, unsigned short int center, char* charPtr, unsigned short int* intPtr, int defense)
{
    // Looping variables
    int i;
    int k;

    // Random int to be set
    int random;

    int play;
    int emptySpaces[9];

    // If play is greater than or equal to 3
    if(intPtr[0] >= 3){

        // Check if AI wins with given play
        play = aiCheckWin(selector, charPtr, defense);

        if(play==20){

            // Set player back to AI
            charPtr[40] = charPtr[42];

            //Fills all empty positions into array of integers
            for(i = 0; i < 9; i++)
            {
                if(charPtr[30 + i] == ' ')
                {
                    emptySpaces[k] = i;
                    k++;
                }
            }

            // Sets new seed
            srand(time(NULL));

            // Rolls random int
            random = rand() % k;
            random = emptySpaces[random];

            //Pulls a random int from array and plays in that position
            charPtr[30 + random] = charPtr[40];
            play = random;
        }
    }

    if(intPtr[0] == 2){
        play = 0;

        // Switches player to check their move
        switchPlayer(charPtr);
        for(i = 0; i < 4; i++)
        {
            // If user plays corner, play center
            if((charPtr[30 + corners[i]] == charPtr[40]) || (charPtr[30 + sides[i]] == charPtr[40]) && (charPtr[30 + center] == ' '))
            {
                // Switches to AI to counter move
                switchPlayer(charPtr);
                charPtr[30 + center] = charPtr[40];
                play = center;
            }
        }
        if(play == 0){
            // Set player back to AI
            charPtr[40] = charPtr[42];

            //Fills all empty positions into array of integers
            for(i = 0; i < 9; i++)
            {
                if(charPtr[30 + i]==' '){
                    emptySpaces[k] = i;
                    k++;
                }
            }

            // Sets new seed
            srand(time(NULL));

            // Rolls random int
            random = rand() % k;
            random = emptySpaces[random];

            //Pulls a random int from array and plays in that position
            charPtr[30 + random] = charPtr[40];
            play = random;
            switchPlayer(charPtr);
        }
    }

    else if(intPtr[0] == 1){
        play = startingPosition(charPtr,corners);
    }

    printf("\n                        **AI played position %i**\n", play);

    return 0;
}

/**
 * AI checks for win and returns position value
 */
int aiCheckWin(ticTacToeBoard* selector, char* charPtr, int defense)
{
    int play;

    // Aggressive check
    defense = 0;

    // Resets win detector
    play = 0;

    //Checks if AI can win. Play if true
    play = findWinningMove(selector, charPtr, defense);

    if(play != 10){
        return play;
    }

    // Switches player to check if they can win
    switchPlayer(charPtr);

    // Defensive check
    defense = 1;

    // Checks if user can win. Counter if true
    play = findWinningMove(selector, charPtr, defense);

    if(play != 10){
        return play;
    }

    // Switches to AI
    charPtr[40] = charPtr[42];

    return 20;
}

/**
 * Places starting position on board
 */
int startingPosition(char* charPtr, unsigned short int* corners)
{
    unsigned short int randomCorner;
    int random;

    // Sets new seed
    srand(time(NULL));

    // Rolls random int
    random = rand() % 4;

    // Rolls a corner
    randomCorner = corners[random];

    // Places X or O corresponding to the value of random
    charPtr[30+randomCorner] = charPtr[40];

    return randomCorner;
}
