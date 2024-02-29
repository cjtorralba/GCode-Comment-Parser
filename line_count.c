#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include <pthread.h>


#define MAX_SIZE 100000
#define NUM_THREADS 4

unsigned long int get_file_line_count(const char* file_name) {
	unsigned long long ret_val;
	char shell_cmd[MAX_SIZE];
	char line[MAX_SIZE];
	int status;
	sprintf(shell_cmd, "wc -l < %s", file_name);
	FILE* fp;
	
	if(!system(NULL)) {
		fprintf(stderr, "Processor busy, cannot execute command");
		exit(EXIT_FAILURE);
	} 

	fp = popen(shell_cmd, "r");	

	if(fp == NULL) {
		fprintf(stderr, "Could not open pipe to bash. Exiting...");
		exit(EXIT_FAILURE);
	}

	while(fgets(line, MAX_SIZE, fp) != NULL)
		fprintf(stdout, "Line from bash: %s", line);

	status = pclose(fp);

	if(status == -1) {
		fprintf(stderr, "Error closing file. Exiting...");
		exit(EXIT_FAILURE);
	}


	ret_val = strtoul(line, NULL, 10); // Turning into unsigned long long base 10

	return ret_val;

}





int main(int argc, char** argv) {
	int arr[MAX_SIZE];
	int i;
	pthread_t thread_id;
	unsigned long line_count;
	unsigned long lines_per_thread;
	int remainder;
	time_t time_val;
	srand((unsigned) time(&time_val)); // See random num generator

	for(i = 0; i < MAX_SIZE; ++i) {
		arr[i] = rand() % MAX_SIZE;	
		//fprintf(stdout, "[%d]:%d\n", i, arr[i]);
	}




	line_count = get_file_line_count((const char*) "example.gcode");

	lines_per_thread = line_count / NUM_THREADS; // How many lines each thread will be seraching through

	remainder = line_count % NUM_THREADS; 	     // Remainder amount of lines
	fprintf(stderr, "Val from func: %lu", line_count); 









	exit(EXIT_SUCCESS);
}
