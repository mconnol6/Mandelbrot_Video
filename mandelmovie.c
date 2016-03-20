//Mary Connolly
//OS Lab 3

#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <errno.h>
#include <signal.h>
#include <ctype.h>

void execute(char** argv);
void wait_shell();
void print_error();
int check_if_int(char *t);
void convert_to_tokens(char **t, char *command);
void start_process(int i);


int main(int argc, char *argv[]) {

	//check that there is an argument for how many processes to run
	if (argc != 2) {
		printf("mandelmovie: usage: mandelmovie <num processes>\n");
		exit(1);
	}

	//check if the second input argument is an integer
	if (!(check_if_int(argv[1]))) {
		printf("mandelmovie: usage: mandelmovie <num processes>, <num processes> must be an integer\n");
		exit(1);
	}

	int n = atoi(argv[1]);

	//number must be less than or equal to 50
	if ( n > 50) {
		printf("mandelmovie: usage: mandelmovie <num processes>, <num processes> must be an integer\n");
		exit(1);
	}

	//create n processes
	int i;
	int count = 0; // use to keep track of how many processes have been run

	//start the first n processes
	for (i=0; i<n; i++) {
		start_process(i);
		count++;
	}

	//while less than 50 process have started, wait for a process to finish. Once it has, start the next process
	while (count < 50) {
		wait_shell();
		start_process(count);
		count++;
	}

	wait_shell(); //wait for last process to complete

	exit(0);
}

//start process number n
void start_process(int i) {

	char *tokens[50];
	char command[100];
	float inc = .039998; //increment used to vary -s option

	//create string
	sprintf(command, "./mandel -x -.501 -y -.519 -s %f -m 2000 -W 750 -H 750 -o mandel%i.bmp", 2-i*inc, i+1);

	//convert string to tokens
	convert_to_tokens(tokens, command);

	//execute command
	execute(tokens);
}

void convert_to_tokens(char **t, char *command) {
	
	char *first, *temp;

	//get first input
	first = strtok(command, " \t\n");

	t[0] = first;

	//set up variables to get tokens
	int i=1;

	//get tokens
	do {
		temp = strtok(NULL, " \t\n");
		t[i] = temp;
		i++;
	} while (temp != NULL);
}

//execute command in argv
void execute(char **argv) {
	pid_t pid;

	//fork parent process
	if ((pid = fork()) < 0) {
		//check for error
		print_error();
		exit(1);
	}

	//if pid is 0, execute command in argv
	if (pid == 0) {
		//execute child process. If pid is < 0, print error message
		if (execvp(*argv, argv) < 0) {
			print_error();
			exit(1);
		}
	}
}

//waits for a process to finish
//return value is the status
void wait_shell() {
	int status, return_value;

	return_value = wait(&status);

	//first check return value
	// if there are no children to wait for
	if (return_value < 0) {
		printf("mandelmovie: wait error: no processes left\n");
		return;
	}
	
	//now check status
	else if (status == 0) {
	//if return was successful
		return;
	} else if (WIFSIGNALED(status)) {
		//else check for errors
		printf("mandelmovie: process %i exited abnormally with signal %i: %s\n", return_value, status, strsignal(WTERMSIG(status)));
		exit(1);
	}
}

//function for printing error messages
void print_error() {
	printf("%s\n", strerror(errno));
}

//use to make sure that a string can be converted to an integer
int check_if_int(char t[]) {
	int i=0;
	while (t[i] != '\0') {
		if (!(isdigit(t[i]))) {
			return 0;
		}
		i++; //increment i
	}
	return 1;
}
