/**********************************************************
*           Shell and client server programs              *
*   Made by: Konstantinos Lazaros - Nefeli Stefanatou     *
*	           Lesson: Advanced Programming               *
*	           D.I.B, University of Thessaly              *
**********************************************************/


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/wait.h>
#include "shellfunc.h"


int command(int input, int first, int last) // Εκτέλεση εντολών και redirection (για τα pipes)
{
	int pipettes[2];
	/* Invoke pipe */
	pipe(pipettes);
	pid = fork();

	if (pid == 0) {

		if (first == 1 && last == 0 && input == 0) {
			// Redirect για πρώτη εντολή
			dup2(pipettes[WRITE], STDOUT_FILENO);
		} else if (first == 0 && last == 0 && input != 0) {
			// Redirect για δεύτερη εντολή
			dup2(input, STDIN_FILENO);
			dup2(pipettes[WRITE], STDOUT_FILENO);
		} else {
			// Για την τελευταία εντολή
			dup2(input, STDIN_FILENO);
		}
		if (execvp(args[0], args) == -1) // Εδώ εκτελούνται οι εντολές
			_exit(EXIT_FAILURE); // Σε περίπτωση που αποτύχει το child process
	}

	if (input != 0)
		close(input);

	// Κλείνει το pipe (δεν χρειάζεται να γραφεί κάτι άλλο)
	close(pipettes[WRITE]);

	// Αν είναι η τελευταία εντολή τότε δεν χρειάζεται να διαβαστεί κάτι άλλο (pipe closing...)
	if (last == 1)
		close(pipettes[READ]);

	return pipettes[READ];
}


void cleanup(int n) // Συνάρτηση αναμονής (καθυστέρηση της κύριας διαδικασίας για να τυπωθούν τα αποτελέσματα των child processes στην οθόνη)
{
	int i;
	for (i = 0; i < n; ++i)
		wait(NULL);
}


int runshell(char* argv) // Εκτέλεση του shell
{
	char* cmd = malloc(strlen(argv) + 2);
	strcpy(cmd, argv);
	strcat(cmd, "\n");
	// Εκτέλεση μια φορά
	run_children(cmd);
	free(cmd); // Απελευθέρωση μνήμης που δεσμεύτηκε με malloc
	return 0;
}


int run_children(char* cmd)
{

	/* Για το redirection του output δεν χρησιμοποιήσαμε το ">>". Χρησιμοποιήσαμε pipe το
	οποίο κάνει redirect το output σε file. Η εντολή που χρησιμοποιούμε είναι η εξής:
	*insert command here* | tee *insert_filename_here*. */
	int input = 0;
	int first = 1;
	char* next = strchr(cmd, '|'); // Βρίσκουμε το πρώτο '|' το οποίο δηλώνει pipe
	while (next != NULL) {
		// 'next' points to '|'
		*next = '\0';
		input = run(cmd, input, first, 0);

		cmd = next + 1;
		next = strchr(cmd, '|'); // Εύρεση επόμενου '|'
		first = 0;
	}
	input = run(cmd, input, first, 1);
	cleanup(100);
	return 0;
}


int run(char* cmd, int input, int first, int last) // Έλεγχος για exit και κλείση στην συνάρτηση command για εκτέλεση εντολής
{
	split(cmd);
	if (args[0] != NULL) {
		if (strcmp(args[0], "exit") == 0)
			exit(0);
		return command(input, first, last);
	}
	return 0;
}


char* skipwhite(char* s) // Περνάει/αγνοεί τα κενά
{
	while (isspace(*s)) ++s;
	return s;
}


void split(char* cmd) // Σπάει το string σε κομμάτια χωρίς κενά
{
	cmd = skipwhite(cmd);
	char* next = strchr(cmd, ' ');
	int i = 0;

	while(next != NULL) {
		next[0] = '\0';
		args[i] = cmd;
		++i;
		cmd = skipwhite(next + 1);
		next = strchr(cmd, ' ');
	}

	if (cmd[0] != '\0') {
		args[i] = cmd;
		next = strchr(cmd, '\n');
		next[0] = '\0';
		++i;
	}
	args[i] = NULL;
}


