/*
*Author: Chris Chiu
*Date: February 16 2016
*Purpose: Compilation of small games using functions and arrays
*/

#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <Windows.h>

#define ORIGINAL "textfile.txt"
#define ANIMALS "Animals.txt"
#define COUNTRIES "Countries.txt"
#define COMPANIES "Companies.txt"
#define SCIENCE "Science.txt"
#define PALINDROMES "Palindromes.txt"

#define ANIMALS_COL 0
#define SCIENCE_COL 1
#define COUNTRIES_COL 2
#define COMPANIES_COL 3
#define PALINDROMES_COL 4
#define NUMCOLS 5
#define NUMCOLSPALINDROME 12
#define NUMROWS 38
#define START 0
#define MAX_GUESSES 6

#define MIN_BET 5
#define WON 1
#define LOST 0
#define TRUE 1
#define FALSE 0
#define TIE 3
#define MAX_ROLL 6
#define MAX_NAME_LENGTH 50

#define ROW_SIZE 5
#define COLUMN_SIZE 33
#define POS_ONE grid[0][1]
#define POS_TWO grid[0][5]
#define POS_THR grid[0][9]
#define POS_FOU grid[2][1]
#define POS_FIV grid[2][5]
#define POS_SIX grid[2][9]
#define POS_SEV grid[4][1]
#define POS_EIG grid[4][5]
#define POS_NIN grid[4][9]

//function prototypes

//craps
void playCraps(void);
int getWallet(void);
int makeBet(int);
int doAgain(void);
char playAgain(void);
void goodbye(int);
int playRound(void);
int rollForPoint(int);
int rollDice(void);
int rollDie(void);
//tic tac toe
void playTicTacToe(void);
int winRound(char grid[][COLUMN_SIZE], char ordersymbol[], char ordername[][MAX_NAME_LENGTH], int count);
void initializeGrid(char array[ROW_SIZE][COLUMN_SIZE]);
void printGrid(char array[ROW_SIZE][COLUMN_SIZE]);
int fillGrid(int position, char symbol, char grid[ROW_SIZE][COLUMN_SIZE]);
int round_TTT(char grid[][COLUMN_SIZE], char ordersymbol[], char ordername[][MAX_NAME_LENGTH]);
//hangman
void playHangMan(void);
void initializeHangMan(int wordlength, int row_index, int flag, char copyarray[], char guessed_letters[]);
int printbars(int wordlength, int start, char copyarray[], int loopnum);
void fill_bar(int length, char wordarray[][MAX_NAME_LENGTH], int row_index, char guessed_letters[26], int flag, char copyarray[]);

//universal
void adding_apos_s(int name_position, char name[][MAX_NAME_LENGTH], char copyname[]);
void copy_1D_to_2D(char word[MAX_NAME_LENGTH], char wordarray[][MAX_NAME_LENGTH], int i);


int main(void)
{
	//char name[MAX_NAME_LENGTH];
	int gamenum;
	char response;
	srand(time(NULL));

	printf("Hi my name is Pixel...Welcome to the Game Centre,\n");
	//scanf("%s", &name);

	do{
		printf("\nWhat game would you like to play?\n"
			"1. Craps\n"
			"2. Tic-Tac-Toe\n"
			"3. Hangman\n"
			"4. Guess the number\n"
			"Enter the number: ");
		scanf("%d", &gamenum);

		if (gamenum == 1)
			playCraps();
		else if (gamenum == 2)
			playTicTacToe();
		else if (gamenum == 3)
			playHangMan();
		else if (gamenum == 4)
			printf("Game under development.\n");

		response = playAgain();
	} while (response == 'y' || response == 'Y');



	system("pause");
	return 0;
}
/*
plays a round of hangman, calling all appropriate functions
Parameters/return type: none
*/
void playHangMan(void)
{
	FILE* inFile;
	int col_index;
	int row_index;
	char word[MAX_NAME_LENGTH];
	char wordarray[NUMROWS][MAX_NAME_LENGTH];
	char copyarray[MAX_NAME_LENGTH] = { 0 };
	char category[NUMCOLS][20] = { "Animals.txt", "Countries.txt", "Companies.txt", "Science.txt", "Palindromes.txt" };
	int i = 0;
	int length;
	int flag = 0;
	char guessed_letters[26] = { ' ' };

	col_index = rand() % NUMCOLS;
	col_index = 4;///////////////////////////////////
	inFile = fopen(category[4], "r");				//CHANGE FROM 4 BACK TO col_index after done debugging
	if (inFile == NULL)
		printf("Error: could not locate file.\n");
	else
	{
		while (fscanf(inFile, "%s", &word) == 1)
		{
			copy_1D_to_2D(word, wordarray, i);
			i++;
		}
		fclose(inFile);

		if (col_index == 4)
			row_index = rand() % (NUMCOLSPALINDROME-1);
		else
			row_index = rand() % NUMROWS;

		length = strlen(wordarray[row_index]);

		for (i = 0; i < length; i++)
		{
			if (wordarray[row_index][i] == '-')
				copyarray[i] = ' ';
			else if (wordarray[row_index][i] == '\'')
				copyarray[i] = '\'';
			else if (wordarray[row_index][i] == '?')
				copyarray[i] = '?';
			else if (wordarray[row_index][i] == ',')
				copyarray[i] = ',';
			else if (wordarray[row_index][i] == '.')
				copyarray[i] = '.';
			else
				copyarray[i] = '_';
		}
		copyarray[i] = '\0';

		initializeHangMan(length, row_index, flag, copyarray, guessed_letters);
		printf("%s", wordarray[row_index]);
	
		fill_bar(length, wordarray, row_index, guessed_letters, flag, copyarray);
	}
			
}

/*
Fills in letters that are correct
Parameter: the word length, the word, the row index, array of guessed letters, and a flag (0 for empty and 1 to change), copyarray
Return Type: none
*/
void fill_bar(int wordlength, char wordarray[][MAX_NAME_LENGTH], int row_index, char guessed_letters[], int flag, char copyarray[])
{
	int match;
	int i = 0;
	int j;
	int z = 0;
	int count = 0;
	int gameend = -1;
	
	flag = 0;
	do
	{	
		match = FALSE;
		printf("Enter a letter: ");
		scanf(" %c", &guessed_letters[i]);		//I don't know why putting a space in front of %c makes it work???
		guessed_letters[i + 1] = '\0';

		for (j = 0; j < wordlength; j++)
		{
			if (guessed_letters[i] == wordarray[row_index][j])
			{
				copyarray[j] = guessed_letters[i];
				match = TRUE;
			}
		}

		if (!match)
		{
			count++;
			printf("WRONG");
			printf(" %d", count);
			Sleep(1000);

			if (count == MAX_GUESSES)
				gameend = LOST;
		}

		flag++;
		i++;
		initializeHangMan(wordlength, row_index, flag, copyarray, guessed_letters);
	} while (gameend == -1);

	
}



/*
Draws the hangman picture
Parameter: Takes in number of lives lost, number of letters, int flag, and an array to copy
Return Type: none (prints pic)
*/
void initializeHangMan(int wordlength, int row_index, int flag, char copyarray[], char guessed_letters[26])
{
	int i;
	int cutoff;
	int loopnum=0;
	int length;

	//memset(underline, '_ ', wordlength);		useless due to for loop below but memset is cool
	system("cls");
	printf(" ____________________________\n"
		"|  ______   ___________))____|\n"
		"|  |    /  /           ||\n"
		"|  |   /  /            ||      ");
	if (wordlength <= 10)
		printf("            ");
	if (flag == 0)
	{
		cutoff = printbars(wordlength, 0, copyarray, 0);
		printf("\n");
		printf("|  |  /  /             ||        ");
		if (cutoff != 0)
		{
			loopnum++;
			printbars(wordlength, cutoff, copyarray, loopnum);
		}
		printf("\n");
	}
	else
	{
		cutoff = printbars(wordlength, 0, copyarray, 0);
		printf("\n");
		printf("|  |  /  /             ||        ");
		if (cutoff != 0)
		{
			loopnum++;
			printbars(wordlength, cutoff, copyarray, loopnum);
		}
		printf("\n");
	}
	


	printf("|  | /  /             .-~-.\n"
		"|  |/  /             :     :\n"
		"|  /  /              ' x  x'\n"
		"|    /               '.___.'\n");
	printf("|   /                .~| |~.\n"
		"|  /                /Y .  .Y\\     Guessed: ");

	if (flag)
	{
		length = strlen(guessed_letters);
		for (i = 0; i < 13; i++)
			printf("%c ", guessed_letters[i]);
		printf("\n|  |               //|     |\\\\               ");
		while (i < length)
		{
			printf("%c ", guessed_letters[i]);
			i++;
		}
		printf("\n");
	}
	else
		printf("\n|  |               //|     |\\\\ \n");							//MUST INCLUDE two backslashes in order for \ to show
	printf("|  |              // |   . | \\\\ \n"
		"|  |             (o  \\_____/  o) \n"
		"|  |                 (     ) \n"
		"|  |                 | /^\\ | \n"
		"|  |                 | | | | \n"
		"|  |                 | | | | \n"
		"|  |                (__) (__) \n"
		"|  |________________   \n"
		"|                   \\ \n"
		"|____________________\\ \n");
}

/*
Prints underscores and any symbols that aren't letters
Parameters: length of the word being printed, index of starting, copy array, loopnum to indicate it's been through
Return Type: the index where it cuts off
*/
int printbars(int wordlength, int start, char copyarray[], int loopnum)
{
	int i;		//static means once the function is left and is called again, the value is saved

	for (i = start; i < wordlength; i++)
	{
		printf("%c ", copyarray[i]);

		if (i >= 20 && copyarray[i] == ' ' && loopnum == 0)
		{
			loopnum++;
			return i;
		}
	}
	
	return 0;
}

/*
Copies a string stored in a 1D array into a 2D array
Parameters: char 1D array, char 2D array, int index
Return Type: none
*/
void copy_1D_to_2D(char word[MAX_NAME_LENGTH], char wordarray[][MAX_NAME_LENGTH], int i)
{
	int length;
	int count;

	length = strlen(word);
	for (count = 0; count < length; count++)
	{
		wordarray[i][count] = word[count];
	}
	wordarray[i][count] = '\0';
}





/*
play TicTacToe, calling all appropriate functions
Parameters: name of original player
Return Type: none
*/
void playTicTacToe(void)
{
	char copyname[MAX_NAME_LENGTH];
	char grid[ROW_SIZE][COLUMN_SIZE];
	char ordername[2][MAX_NAME_LENGTH];
	char ordersymbol[2];
	char symbol1;
	char symbol2;
	int index = 0;
	int result = FALSE;

	printf("Who wants to go first? Enter name: ");
	scanf("%s", ordername[index]);
	printf("Enter the name of the opponent: ");
	scanf("%s", ordername[index+1]);
	printf("%s would you like to be X or O? Enter the character: ", ordername[index]);
	scanf(" %c", &symbol2);			//the empty space in front of %c tells to ignore invisible character i.e. '\n'

	if (symbol2 == 'X' || symbol2 == 'x')
	{
		symbol1 = 'O';
		symbol2 = 'X';
	}
	else if (symbol2 == 'O' || symbol2 == 'o')
	{
		symbol1 = 'X';
		symbol2 = 'O';
	}
	else
	{
		printf("Entered invalid character. %s is assigned X and %s is assigned O.\n(you had one job...)\n", ordername[index], ordername[index + 1]);
		symbol1 = 'O';
		symbol2 = 'X';
	}
	ordersymbol[index] = symbol2;
	ordersymbol[index + 1] = symbol1;

	do
	{
		initializeGrid(grid);
		printf("Enter the number corresponding to grid.\n\n");

		index = round_TTT(grid, ordersymbol, ordername);

		if (index == TIE)
			printf("Tie game!\n");
		else
			printf("%s wins!\n", ordername[abs(index - 1)]);
	} while (doAgain());
}

/*
plays round of tic tac toe, two turns
Parameters: grid, ordername and ordersymbol (2 arrays)
Return Type: index of name/symbol
*/
int round_TTT(char grid[][COLUMN_SIZE], char ordersymbol[], char ordername[][MAX_NAME_LENGTH])
{
	int result;
	int index=0;
	int position;
	char copyname[MAX_NAME_LENGTH];
	char name;
	int empty_space;
	int count = 0;

	do
	{
		adding_apos_s(index, ordername, copyname);
		printf("%s: ", copyname);
		do
		{
			scanf("%d", &position);
			empty_space = fillGrid(position, ordersymbol[index], grid);
			count++;
			if (empty_space == FALSE)
			{
				printf("Invalid move. Enter another number: ");
				count--;
			}
		} while (empty_space == FALSE);

		if (index == 0)
			index++;
		else
			index = 0;
		if (count == 9)
			index = TIE;		
	} while (!winRound(grid,ordersymbol,ordername,count));

	return index;
}

/*
adds a "'s" to end of name in another array
Parameters: Two strings, one original and one copy of name also index of which position name string
Return Type: none
*/
void adding_apos_s(int name_position, char name[][MAX_NAME_LENGTH], char copyname[])
{
	int length;
	int index = 0;

	char extension[] = "'s turn";

	length = strlen(name[name_position]);

	for (index = 0; index < length; index++)
		copyname[index] = name[name_position][index];

	for (index = 1; index < 9; index++)
	{
		copyname[length + index - 1] = extension[index - 1];
	}
	copyname[length + index] = '\0';
}

/*
checks and sees if there is a three in a row
Parameters: the grid array, and ordersymbol array
Return Type: int, 1 for TRUE (won), 0 for FALSE (not won)
*/
int winRound(char grid[][COLUMN_SIZE], char ordersymbol[], char ordername[][MAX_NAME_LENGTH], int count)
{
	int index;
	int result = FALSE;
	const char X = 'X';
	const char O = 'O';

	for (index = 0; index < 2; index++)
	{
		if ((POS_ONE == ordersymbol[index]) && (POS_TWO == ordersymbol[index]) && (POS_THR == ordersymbol[index]))
		{
			result = TRUE;
			break;
		}
		else if ((POS_FOU == ordersymbol[index]) && (POS_FIV == ordersymbol[index]) && (POS_SIX == ordersymbol[index]))
		{
			result = TRUE;
			break;
		}
		else if ((POS_SEV == ordersymbol[index]) && (POS_EIG == ordersymbol[index]) && (POS_NIN == ordersymbol[index]))
		{
			result = TRUE;
			break;
		}
		else if ((POS_ONE == ordersymbol[index]) && (POS_FOU == ordersymbol[index]) && (POS_SEV == ordersymbol[index]))
		{
			result = TRUE;
			break;
		}
		else if ((POS_TWO == ordersymbol[index]) && (POS_FIV == ordersymbol[index]) && (POS_EIG == ordersymbol[index]))
		{
			result = TRUE;
			break;
		}
		else if ((POS_THR == ordersymbol[index]) && (POS_SIX == ordersymbol[index]) && (POS_NIN == ordersymbol[index]))
		{
			result = TRUE;
			break;
		}
		else if ((POS_ONE == ordersymbol[index]) && (POS_FIV == ordersymbol[index]) && (POS_NIN == ordersymbol[index]))
		{
			result = TRUE;
			break;
		}
		else if ((POS_THR == ordersymbol[index]) && (POS_FIV == ordersymbol[index]) && (POS_SEV == ordersymbol[index]))
		{
			result = TRUE;
			break;
		}
		if (count == 9)
			return TIE;
	}
	
	return result;
}

/*
prompts user for character and fills in corresponding place in grid
Parameter: position where you want to go, the symbol, and grid array
Return Type: false if grid already filled
*/
int fillGrid(int position, char symbol, char grid[ROW_SIZE][COLUMN_SIZE])
{
	do
	{
		if (position == 1 && POS_ONE==' ')
			POS_ONE = symbol;
		else if (position == 2 && POS_TWO == ' ')
			POS_TWO = symbol;
		else if (position == 3 && POS_THR == ' ')
			POS_THR = symbol;
		else if (position == 4 && POS_FOU == ' ')
			POS_FOU = symbol;
		else if (position == 5 && POS_FIV == ' ')
			POS_FIV = symbol;
		else if (position == 6 && POS_SIX == ' ')
			POS_SIX = symbol;
		else if (position == 7 && POS_SEV == ' ')
			POS_SEV = symbol;
		else if (position == 8 && POS_EIG == ' ')
			POS_EIG = symbol;
		else if (position == 9 && POS_NIN == ' ')
			POS_NIN = symbol;
		else
			return FALSE;
	} while (position < 1 && position>9);

	printGrid(grid);

	return TRUE;
}

/*
initializes a brand new board
Parameter: takes in an array
Return: none
*/
void initializeGrid(char grid[ROW_SIZE][COLUMN_SIZE])
{
	int row;
	int column;
	int count = 1;

	for (row = 0; row < ROW_SIZE; row++)
	{
		for (column = 0; column < COLUMN_SIZE; column++)
		{
			if ((row == 1 && (column == 3 || column == 7 || column == 25 || column == 29)) || (row == 3 && (column == 3 || column == 7 || column == 25 || column == 29)))
				grid[row][column] = '+';
			else if (((row == 1 || row == 3) && (column < 11)) || ((row == 1 || row == 3) && (column > 21)))
			{
				grid[row][column] = '-';
			}

			else if (column == 3 || column == 7 || column == 25 || column == 29)
			{
				grid[row][column] = '|';
			}

			else
				grid[row][column] = ' ';
		}
	}
	for (row = 0; row < ROW_SIZE; row += 2)
	{
		for (column = 23; column < COLUMN_SIZE; column += 4)
		{
			grid[row][column] = count + '0';			//convert from int to char (+'0')
			count++;									//convert from char to int (-'0')
		}
	}
	printGrid(grid);
}

/*
takes in 2D array and prints it
Parameters: 2D array
Return Type: none
*/
void printGrid(char grid[ROW_SIZE][COLUMN_SIZE])
{
	int row;
	int column;

	for (row = 0; row < ROW_SIZE; row++)
	{
		for (column = 0; column < COLUMN_SIZE; column++)
		{
			printf("%c", grid[row][column]);
		}
		printf("\n");
	}
	printf("\n");
}










//play Craps game, calling all appropriate functions
//Parameter: none
//Return Type: none
void playCraps(void)
{
	int wallet;
	int bet;

	wallet = getWallet();
	do
	{
		bet = makeBet(wallet);
		if (playRound())		//this just means if it's true
		{
			printf("You win!\n");
			wallet += bet;
		}
		else                   //if playRound is false
		{
			printf("You lose!\n");
			wallet -= bet;
		}
	} while (wallet >= MIN_BET&&doAgain());

	goodbye(wallet);
}


/*
prompts user for money, if less than min bet repeatedly prompts for another value.
Parameter: none
Return: amount of money entered by user
*/
int getWallet(void)
{
	int wallet;

	printf("How much money do you have to play with: ");
	scanf("%d", &wallet);

	while (wallet<MIN_BET)
	{
		printf("You must need at least %d to play.\n", MIN_BET);
		printf("How much money do you have to play with: ");
		scanf("%d", &wallet);
	}

	return wallet;
}

/*
prompts user for a bet, if entered value is less than min bet or greater than wallet repeatedly prompts another value
Parameters: wallet
Return: amount user entered as bet
*/
int makeBet(int wallet)
{
	int bet;
	printf("\nYou have $%d in your wallet.\n", wallet);
	printf("Place your bet (minimum $%d): ", MIN_BET);
	scanf("%d", &bet);

	while (bet<MIN_BET || bet>wallet)
	{
		if (bet<MIN_BET)
			printf("Your bet is lower than the minimum bet. Enter another number: ");
		else
			printf("You do not have enough money to bet this much. Enter another number: ");
		scanf("%d", &bet);
	}
	return bet;
}

/*
prompts user to return to main menu
Returns: TRUE if yes, false otherwise
*/
char playAgain(void)
{
	char response;
	printf("Would you like to return to the main menu? (Y/N): ");
	scanf(" %c", &response);

	return response;
}

/*
prompts user to play again
Returns: TRUE if yes, false otherwise
*/
int doAgain(void)
{
	int response;
	printf("Enter 1 to play again, 0 to quit: ");
	scanf("%d", &response);

	return response;
}
/*
prints goodbye, if money is less than min bet says broke, else prints money left in wallet
Parameter: Wallet
*/
void goodbye(int wallet)
{
	if (wallet<MIN_BET)
		printf("You broke AF gtfo...");
	else
		printf("You have $%d left in your wallet. Goodbye!\n", wallet);
}
/*
plays a round of craps
Returns: True if won, false otherwise
*/
int playRound(void)
{
	int roll;
	int point;
	int wonRound;

	roll = rollDice();
	printf("You rolled a %d.\n", roll);

	if ((roll == 2) || (roll == 3) || (roll == 12))
		wonRound = FALSE;
	else if ((roll == 7) || (roll == 11))
		wonRound = TRUE;
	else
	{
		point = roll;
		printf("Rolling for point: %d...\n", point);
		wonRound = rollForPoint(point);
	}

	return wonRound;
}

/*
repeatedly roll until rolls point (win) or 7(lose)
Parameter: point
Return: True if rolled point number, false if rolled 7
*/
int rollForPoint(int point)
{
	int gotPoint = FALSE;
	int roll;

	roll = rollDice();
	Sleep(1500);
	printf("\nYou rolled a %d...\n", roll);

	while (roll != point&&roll != 7)
	{
		Sleep(1500);
		roll = rollDice();
		printf("You rolled a %d...\n", roll);
	}

	if (roll == point)
		gotPoint = TRUE;

	return gotPoint;
}

/*
rolls a pair of dice
Return: Sum of each die
*/
int rollDice(void)
{
	return rollDie() + rollDie();
}

/*
rolls a die
Returns: number of die rolled
*/
int rollDie(void)
{
	return rand() % MAX_ROLL + 1;
}