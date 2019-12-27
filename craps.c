//Praneeth Sangani (PRS79)
#define _GNU_SOURCE
#include <stdio.h>
#include <stddef.h>
#include <stdlib.h>
#include <string.h>

//Check to see the user started the program properly 
void check_argc_is_valid(int argc)
{
	if(argc != 2)
	{
		fprintf(stderr, "argc is not valid e.x. ./crap.c /dev/dice\n");
		exit(1);
	}
}

//Open the file, if no file error
FILE* checked_fopen(const char* name, const char* mode)
{
	FILE* f = fopen(name, mode);
	if(f == NULL)
	{
		fprintf(stderr, "could not open file '%s'\n", name);
		exit(1);
	}
	return f; 
}

//Check if the user wants to play again
int check_play_again(char* input)
{
	printf("Would you like to play again? ");
	fgets(input, 20, stdin);
	input[strlen(input) - 1] = '\0';
	if(input[0] == 'n'|| input[0] == 'N')
		return 0; 
	else
		return 1; 
}

//Sets dice so we can roll from 1-6
unsigned char set_Dice_Range(unsigned char dice)
{
	return dice % 6 + 1; 
}

//Play the game of craps
void play_game(char* input, FILE* f)
{	
	unsigned char diceRoll1; 
	unsigned char diceRoll2; 
	unsigned char point; 
	int play_game = 1; 
		
	while(play_game == 1)
	{
		int firstRoll = 1; 
		int resultFound = 0; 	
		fread(&diceRoll1, sizeof(char), 1, f); 
		fread(&diceRoll2, sizeof(char), 1, f); 
		diceRoll1 = set_Dice_Range(diceRoll1); 
		diceRoll2 = set_Dice_Range(diceRoll2); 
		unsigned char sum = diceRoll2 + diceRoll1;
		printf("You have rolled %d + %d = %d\n", diceRoll1, diceRoll2, sum);

		if(sum == 2 || sum == 3 || sum == 12)
		{
			printf("You Lost!\n");
		}
		else if(sum == 7 || sum == 11)
		{
			printf("You Win!\n");
		}
		else if(firstRoll)
		{
			point = sum; 
			firstRoll = 0; 
			while(!resultFound) //player now loses if he rolls a 7 or wins if he rolls the same number he did (point)
			{
				fread(&diceRoll1, sizeof(char), 1, f); 
				fread(&diceRoll2, sizeof(char), 1, f); 
				diceRoll1 = set_Dice_Range(diceRoll1); 
				diceRoll2 = set_Dice_Range(diceRoll2); 
				sum = diceRoll2 + diceRoll1;
				printf("You have rolled %d + %d = %d\n", diceRoll1, diceRoll2, sum);
				if(sum == 7)
					{
					printf("You Lost!\n");
					resultFound = 1; 
				}
				else if(sum == point)
				{
					printf("You Won!\n");
					resultFound = 1; 
				}
			}
		}			
		play_game = check_play_again(input); 
	}
}	

int main(int argc, char** argv)
{
	
	check_argc_is_valid(argc); 
	FILE* f = checked_fopen(argv[1], "rb"); 
	char input[20]; //To get input from player
	char name[20]; 
	
	printf("Welcome to Praneeth's Casino! Please enter your name: ");
	fgets(input, 20, stdin);
	input[strlen(input) - 1] = '\0';
	
	strcpy(name, input); //set the players name into input
	
	printf("%s, Would you like to Play or Quit? ", name);
	fgets(input, 20, stdin);
	input[strlen(input) - 1] = '\0';
	
	if(strcmp(input, "play") == 0 || strcmp(input, "Play") == 0)
	{
		play_game(input, f); 
	}	
	printf("Goodbye, %s!\n", name);
	exit(0); 
}
