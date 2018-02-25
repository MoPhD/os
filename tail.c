#include "types.h"
#include "user.h"
#include "stat.h"
#include "fcntl.h"

int willOpen(char **fileName)
{
	int fd = open(fileName[1], 0);
	
	if(fd > 0)
	{
		close(fd);
		return 1;
	}
	else
		return -1;
}

int countLines(char **fileName, int locationOfFileName)
{
	int newLineFound = 0;
	int fd, bytesReadIn;
	char charBuffer[2];	
	
	//Test if file was opened correctly	
	if((fd = open(fileName[locationOfFileName], 0)) < 0)
	{
		printf(1, "tail: cannot open %s\n", fileName[locationOfFileName]);
		exit();
	}
 
	bytesReadIn = read(fd, charBuffer, 1);	
	
	//Count lines
	while(bytesReadIn > 0)
	{
		if(charBuffer[0] == '\n')
			newLineFound++;

		bytesReadIn = read(fd, charBuffer, 1);
	}
		
	//Close file
	close(fd);

	return newLineFound;
}


int main(int argc, char *argv[])
{
	char charBuffer[2048];
	int fd, bytesReadIn, loopCounter;	
	int newLineFound = 0, digit=0;	

	if(argc == 3)
	{
		//File name and number of lines provided		
		newLineFound = countLines(argv, 2);

		fd = open(argv[2], 0);
		
		char charString[256];
		strcpy(charString,argv[1]);
		char *numstr = charString;
		numstr=numstr+1;
		digit = atoi(numstr);

		if(newLineFound <= digit && newLineFound > 0)
		{
			//No augmentation required....
			bytesReadIn = read(fd, charBuffer,2048);

		}
		else
		{
			//Iterate offset forward..count down from newLineCurrent
			while(newLineFound > digit)
			{
				bytesReadIn = read(fd, charBuffer, 1);

				if(charBuffer[0] == '\n')
					newLineFound--;
			}
			
			//Output lines to screen
			while((bytesReadIn = read(fd, charBuffer, 2048)) > 0)
			{
				//Iterate over entire file repetitively refreshing buffer when it runs out
				for(loopCounter = 0; loopCounter < bytesReadIn; loopCounter++)
					printf(1, "%c", charBuffer[loopCounter]);

				bytesReadIn = read(fd, charBuffer, 2048);
			}	
		}
	}	
	else if(argc == 2 && (willOpen(argv)) == 1)
	{
		//File name provided and no alteration to line output
		
		newLineFound = countLines(argv, 1);
		
		fd = open(argv[1], 0);

		if(newLineFound <= 10 && newLineFound > 0)
		{
			//no alteration required....
			bytesReadIn = read(fd, charBuffer,2048);

		}
		else
		{
			//Iterate offset forward..count down from newLineCurrent
			while(newLineFound > 10)
			{
				bytesReadIn = read(fd, charBuffer, 1);

				if(charBuffer[0] == '\n')
					newLineFound--;
			}
			
			//Output lines to screen
			while((bytesReadIn = read(fd, charBuffer, 2048)) > 0)
			{
				//Iterate over entire file repetitively refreshing buffer when it runs out
				for(loopCounter = 0; loopCounter < bytesReadIn; loopCounter++)
					printf(1, "%c", charBuffer[loopCounter]);

				bytesReadIn = read(fd, charBuffer, 2048);
			}	
		}
	}	
	else
	{
		//data coming from standard input
		int sizeOfMalloc = 5000;
		char *lineStorage= malloc(sizeOfMalloc * sizeof(char));		
		int bytesReadIn = 0, totalBytesReadIn = 0, linesToPrint = 10;		
		
		//Argc == 2 indicates specified number of lines				
		if(argc == 2)
		{
			char charString[256];
			strcpy(charString,argv[1]);
			char *numstr = charString;
			numstr=numstr+1;
			digit = atoi(numstr);
			
			linesToPrint = digit;
		}

		//Read in and store
		while((bytesReadIn = read(0, charBuffer, 1)) > 0)
		{
			lineStorage[totalBytesReadIn] = charBuffer[0];			

			if(lineStorage[totalBytesReadIn] == '\n')
			{
				newLineFound++;	
			}

			if(bytesReadIn == 0)
				printf(1, "Error!");

			 totalBytesReadIn++;			
		}
		
		//output
		for (loopCounter = 0; loopCounter < totalBytesReadIn; loopCounter++)
		{
			if(lineStorage[loopCounter] == '\n' && newLineFound > linesToPrint)
			{
				newLineFound--;
			}
			else if(newLineFound <= linesToPrint)
			{
				printf(1,"%c", lineStorage[loopCounter]);
			}
		}
	}
	//exit
	exit();
}
