//Stefon Miller
//CS449 Project2
//Recitation # 11579
#include <stdio.h>
#include <stdlib.h>

int main(int argc, char **argv)
{
	char *file = argv[1];								//File to open
	FILE *sFile = fopen(file, "rb");						//Open file in binary mode
	fseek(sFile, 0L, SEEK_END);							//Seek to end of file
	int size = ftell(sFile);							//Get size of file
	fseek(sFile, 0L, SEEK_SET); 							//Return to start of file
	char *buffer = (char*) malloc(sizeof(char*) * size);				//Malloc buffer to read file into
	fread(buffer, sizeof(char), size, sFile);					//Read file into buffer
	char *temp = malloc(sizeof(char) * size);					//Create temporary array for testing
	int tmpCounter = 0;								//Create tempcounter to determine if string is valid
	int i;										//Create i for for loop
	for(i = 0; i < size; i++)							//Loop through buffer
	{
		if(buffer[i] == 9 || ((buffer[i] <= 126) &&(buffer[i] >= 32)))		//If the character is a valid ascii character, put it in temp
		{
			if(tmpCounter >= size)						//If the buffer is full, print what's in the buffer and continue reading
			{
				printf("%s\n", temp);
				tmpCounter = 0;
			}
			temp[tmpCounter] = buffer[i];					
			tmpCounter++;							//Increment counter
		}
		else
		{	
			if(tmpCounter > 3)						//If the counter is more than 3, it is a valid string, print it out
			{
				printf("%s\n", temp);
				int j;							//Set temp buffer to 0 when done
				for(j = 0; j < tmpCounter; j++)
				{
					temp[j] = 0;
				}
				tmpCounter = 0;						//Reset counter
				
			}
			else
			{
				int j;
				for(j = 0; j < tmpCounter; j++)				//If buffer is not a valid string, empty buffer and move on
				{
					temp[j] = 0;
				}
				tmpCounter = 0;
			}
		}
	} 
	free(buffer);									//free malloc'd memory and close files
	free(temp);
	fclose(sFile);
	return 0;
}
