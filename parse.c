#include<stdio.h>
#include<stdlib.h>
#include<regex.h>
#include<string.h>

#define FILE_OPEN_ERROR -1
#define SUCCESS 0

// GCode only allows for max length of 256 characters
#define MAX_LINE_LENGTH 256 

#define MAX_VARIABLE_NAMES 20



/**
 * Parses comments out of GCode file, using list of selected variable names
 */
int parse_comments(const char* file_name, char** variable_names, unsigned short variable_length_arr) {

	FILE* fp = NULL;
	char line[MAX_LINE_LENGTH];
	regex_t regex;
	int reg_return;
	char regex_string[MAX_LINE_LENGTH + 1];
	int i;
	char err_buf[MAX_LINE_LENGTH];

	// Attempt to open file only reading, additionally this ensures the file does exist
	fp = fopen(file_name, "r"); 

	if(fp == NULL) { // FILE NOT OPENED
		fclose(fp);
		return FILE_OPEN_ERROR;
	}


	while(fgets(line, MAX_LINE_LENGTH, fp)) {
		for(i = 0; i < variable_length_arr; ++i) {

			// Attempt to compile regex	
			reg_return = regcomp(&regex, variable_names[i], 0);

			if(reg_return) {
				fprintf(stderr, "Could not compile regex");
				fprintf(stderr, "\n");
				exit(EXIT_FAILURE);
			}


			// Attempt to match against specific line
			reg_return = regexec(&regex, line, 0, NULL, 0);

			if(!reg_return) { // Check for match
				printf("Match on this line: %s", line);
			} else {
				//regerror(reg_return, &regex, err_buf, sizeof(err_buf));
				//fprintf(stderr, "Regex compilation failed: %s\n", err_buf);
			}

			regfree(&regex); // Free after each use
		}

	}

	fclose(fp); 
	regfree(&regex);

	return SUCCESS;
}




/**
 * Parses variable names we are looking for out of a file 
 *
 * Returns:
 * 	Size of the array being returned
 * 
 */
int parse_comment_variables(const char* file_name, char*** var_list) {
	FILE* fp = NULL;
	char line[MAX_LINE_LENGTH];
	unsigned short index = 0;
	*var_list = (char**) malloc(sizeof(char*) * MAX_VARIABLE_NAMES);

	fp = fopen(file_name, "r");

	if(fp == NULL) { // FILE DID NOT OPEN
		fclose(fp);
		return FILE_OPEN_ERROR;	
	}

	while(fgets(line, MAX_LINE_LENGTH, fp)) {
		line[strcspn(line, "\n")] = 0; // Strip out newline
		(*var_list)[index] = (char*)malloc(strlen(line) + 1);
		strcpy((*var_list)[index], line);	
		index++;
	}

	fclose(fp);

	return index - 1; // Returning index minus one to indicated the size
}



/**
 * Formats strings into a version that will be easily matched agasint regex cases
 * 
 * Returns:
 * 	Number of strings changed
 */
int format_string(char*** comment_variables, unsigned short var_length) {
	char buf[MAX_LINE_LENGTH];
	int i;
	for(i = 0; i < var_length; ++i) {

		// Formatting string in style we want so it will match the Regex
		sprintf(buf, "^;%s:.*", (*comment_variables)[i]);
		fprintf(stdout, "String: %s\n", buf);

		//free(comment_variables[i]); // Need to deallocated before reallocating

		(*comment_variables)[i] = (char*) malloc(strlen(buf) + 1);
		strcpy((*comment_variables)[i], buf);
	}
	return i;

}



int main(int argc, char** argv) {

	const char* name = "example.gcode"; // Name of GCode file for example
	const char* var_file = "variables.txt"; // Name of variable names file in docuemnt
	char** var_names = NULL; 	// Where we will be storing variable names
	unsigned short var_length = 0;  // Size of the array `var_names`

	var_length = parse_comment_variables(var_file, &var_names); // Parsing variable names from file

	format_string(&var_names, var_length); 			// Formatting strings into regex recognizable strings

	parse_comments(name, var_names, var_length);



	// Deallocating
	for(int i = 0; i < MAX_VARIABLE_NAMES; ++i) {
		free(var_names[i]);
	}
	free(var_names);
	exit(EXIT_SUCCESS);
}











