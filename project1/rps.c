//Stefon Miller
//Tue Thu 4-5:15PM
//Recitation 11579

#include <stdio.h>

int main()
{
	char play[10];		//If user wants to play
	char choice[10];	//User's choice
	int computer;		//Computer's score
	int user;		//User's score
	const char *rps[3];				//Make array of 3 strings
	srand((unsigned int)time(NULL));		//Seed RNG
	rps[0] = "Rock";				//Make index 1 rock
	rps[1] = "Paper"; 				//Make index 2 paper	
	rps[2] = "Scissors";				//Make index 3 scissors
	printf("Welcome to rock paper scissors\n");	//Welcoming statement
	printf("Would you like to play?\n");		//Prompt for intput
	scanf("%s", play);				//Get play
	if(strcmp(play, "yes") == 0)			//See if user says yes
	{
		do					//loop for playing again
		{
			computer = 0;				//initialize computer score
			user = 0;				//initialize user score
			while((user < 3 && computer < 3))	//Loop until someone wins 3 times
			{
				do
				{						
					printf("What is your choice?\n");	//Ask for choice
					scanf("%s", choice);
					int i;
					for(i = 0; choice[i]; i++)
                                       		 choice[i] = tolower((unsigned char)choice[i]);                                                          //Change user choice to all lower case

				}while(!(strcmp(choice,"rock")==0 || strcmp(choice,"paper")==0 || strcmp(choice, "scissors") == 0));		//Make sure input is valid
				int value = rand() % (3-0) + 0;											//get random number
				if(strcmp(choice, "rock") == 0)			
				{
					if(strcmp(rps[value], "Paper") == 0)									//If user chooses rock, paper wins and rock ties
					{
						printf("The computer chooses paper, you lose!\n");
						computer++;
					}
					else if(strcmp(rps[value], "Rock") == 0)
					{
						printf("The computer chooses rock, it's a tie!\n");
					}
					else
					{
						printf("The computer chooses scissors, you win!\n");
						user++;
					}
				}
				else if(strcmp(choice, "paper") == 0)										//If user chooses paper, scissors wins and paper ties
				{
					if(strcmp(rps[value], "Paper") == 0)
                       	        	{
                       	                	printf("The computer chooses paper, it's a tie!\n");
                       	        	}
                        	        else if(strcmp(rps[value], "Rock") == 0)
                       	         	{
                                        	printf("The computer chooses rock, you win!\n");
						user++;
                                	}
                                	else
                                	{
                                       		printf("The computer chooses scissors, you lose!\n");
                                        	computer++;
                                	}

				}
				else														//If user chooses scissors, rock wins and scissors ties
				{
					if(strcmp(rps[value], "Paper") == 0)
                                	{
                                        	printf("The computer chooses paper, you win!\n");
                                        	user++;
                                	}
                               		else if(strcmp(rps[value], "Rock") == 0)
                                	{
                                        	printf("The computer chooses rock, you lose!\n");
						computer++;
                                	}
                                	else
                                	{
                                        	printf("The computer chooses scissors, it's a tie!\n");
                                	}

				}
			
				printf("The score is now you: %d computer: %d\n", user, computer);						//Print score every round
			}
			
		printf("The game is now over!  Final score is you: %d computer : %d\n", user, computer);					//Print final score

		if(user > computer)														//Print win/loss messages
		{
			printf("Congratulations!  You won!\n");
		}
		else
		{	
			printf("The computer won, better luck next time!\n");
		}
			
		printf("Would you like to play again?\n");											//Ask to play again
		scanf("%s", choice);					
		}while(strcmp(choice,"yes") == 0);
	}
	
		printf("Thank you for using my program.\n");
		return 1;
	
}
