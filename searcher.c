#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>


char* Treat(char* raw);

int main()
{
	char Filename[80];
	char raw_subject[80];
	char subject[80];
	
	//1.0 get the location of file
	printf("Enter file path: ");
	gets(Filename);
	//debug
	printf("File name or path entered is: %s\n", Filename);
	
	
	//2.0 Get the search subject
	printf("Enter the search word: ");
	gets(raw_subject);
	//debug
	printf("The search word entered is %s. \n", raw_subject);
	
	//2.1 Condition the search word
	//strcpy(subject,  raw_subject);
	strcpy(subject,Treat(raw_subject));
	int sub_length = strlen(subject);
	
	printf("\n Query: %s \nLength: %d\n", subject, sub_length);
	
	
	
	//3.0 Open a file given the name or location
	FILE* myFile;
	myFile = fopen(Filename, "r");
	
	if( myFile == NULL) {
		printf("File failed to open\a\n!");
			
	}
	
	else {
		printf("File opened successfully.\n");
	}
	
	//3.1 Read the first line
	char buffer[100];
	fgets(buffer, 12, myFile);
	printf("First word read is: %s\n", buffer);
	
	//3.2 Drive the cursor to where you want it
	
	
	//3.3 Determine the number of characters in the file
	long int chars = 0;
	long int rows = 0;
	char buff_c =0;
	
	for ( chars;  buff_c != EOF; chars++){
		buff_c = fgetc(myFile);
		if (buff_c == '\n') rows++;
	}
	printf("Number of characters is: %d and %d rows. \n", chars, rows);
	
	
	//3.4 Search for first occurence of query
	rewind(myFile);
	long int cursor = 0;
	long int limit = chars - sub_length;
	rows = 0;
	int matches = 0;
	//char buffer2[sub_length];
	
	for ( cursor;  cursor < limit; cursor++){
		fseek(myFile, cursor, 0);
		fgets(buffer, sub_length +1, myFile);
		
		if (strchr(buffer, '\n')) rows++;
		
		if ( strcmp(Treat(buffer), subject) == 0) {
			matches++;
			printf("match number %d found at line: %d!\n", matches, rows);
		}
		//if (buff == '\n') rows++;
	}
	
	printf("Total matches founds: %d. \n", rows);
	
	
	
	//
	
	
	
	

	fclose(myFile);
return 0;
}


char* Treat(char raw[]) 
{
	//step 1: remove extra spaces	
	int scounter =0;
	int cursor = 0;
	
	for(int i=0; i<strlen(raw); i++) {
			
		if(raw[cursor] == ' ') {
			
			for (scounter = 1; raw[cursor + scounter] ==' '; scounter++) {
				//
			}
			
			if ((raw[cursor + scounter-1] ==' ') && 
			(raw[cursor + scounter] =='.')) {
				cursor += (scounter);
			}
			
			else {
				cursor += (scounter-1);
			}
			
			scounter = 0;			
		}
		
		raw[i] = raw[cursor];
		cursor++;		
	}
	
	//step 2: convert to all small letters
	for (int i=0; i< strlen(raw); i++) {
		raw[i] = tolower(raw[i]);
	}
	
	//printf("Search Query: %s\n", raw);
	
	return raw;
}