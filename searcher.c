#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>


char* Treat(char* raw);

typedef struct{
	
	long int pos_t;
	long int row_t;
	long int rowkey_t;
	char message[200];
}package;

int main()
{
	char Filename[80];
	char raw_subject[80];
	char subject[80];
	char status;
	
	//1.0 get the location of file
	printf("Enter file path: ");
	gets(Filename);
	//debug
	//printf("File name or path entered is: %s\n", Filename);
	
	//2.0 Open a file given the name or location
	FILE* myFile;
	myFile = fopen(Filename, "r");
	
	if( myFile == NULL) {
		printf("%s failed to open\a\n!", Filename);
		status= 'n';
		return 1;	
	}
	
	else {
		printf("%s opened successfully.\n", Filename);
		status = 'y';
	}
	
	while(status == 'y') {
		
		//3.0 Get the search subject
		printf("Enter the search word: ");
		gets(raw_subject);
		//printf("The search word entered is %s. \n", raw_subject);
		
		//3.1 Condition the search word
		strcpy(subject,Treat(raw_subject));
		int sub_length = strlen(subject);
		//printf("\n Query: %s \nLength: %d\n", subject, sub_length);

				
		//4.0 Read the first line
		char buffer[100];
		fgets(buffer, sub_length, myFile);
		//printf("First word read is: %s\n", buffer);
				
		//4.1 Determine the number of characters in the file
		long int chars = 0;
		long int rows = 1;
		char buff_c =0;
		
		rewind(myFile);
		for ( chars;  buff_c != EOF; chars++){
			buff_c = fgetc(myFile);
			if (buff_c == '\n') rows++;
		}
		//printf("Number of characters is: %d and %d rows. \n", chars, rows);
				
		//4.2 Search for all occurences of query
		rewind(myFile);
		long int cursor = 0;
		long int limit = chars - sub_length;
		rows = 0;
		int matches = 0;
		
		for ( cursor;  cursor < limit; cursor++){
			fseek(myFile, cursor, 0);
			fgets(buffer, sub_length +1, myFile);
			
			if (strchr(buffer, '\n')) rows++;
			
			if ( strcmp(Treat(buffer), subject) == 0) {
				matches++;
			}		
		}
		
		printf("\n\nTotal matches found: %d. \n\n", matches);

		
		//4.3 Create a data type to store result location
		
		package results[matches]; 
		rewind(myFile);		
		cursor = 0;
		limit = chars - sub_length;
		rows = 0;
		matches = 0;
		long int key = 0;
		
		// Stores the locations of matches
		for ( cursor;  cursor < limit; cursor++){
			fseek(myFile, cursor, 0);
			fgets(buffer, sub_length +1, myFile);		
			
			if (strchr(buffer, '\n')) {
				rows++;
				key = ftell(myFile);
			}
			
			if ( strcmp(Treat(buffer), subject) == 0) {
				matches++;
				
				results[matches-1].pos_t  = cursor;
				results[matches-1].row_t = rows;
				results[matches-1].rowkey_t = key;
			}
		}
		
		//Goes to the matched locations and retrieves the data
		for (long int i=0; i<matches; i++) {
			
			printf("Match %ld location: %ld \t", i+1, results[i].rowkey_t);
			fseek(myFile, results[i].rowkey_t, 0);
			fgets(results[i].message, 160, myFile);
			printf("%s\n\n", results[i].message); 
			
		}
		
		
		printf("\nDo you want to search again in the same file? y or n? \n");
		status = getc(stdin);
		getc(stdin);		//why ?
		if(status =='y' ) {			
			//
		}
		
		else {
				printf("\n...The end...\n");
				status ='n';
		}
	
}
	
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
	
	return raw;
}