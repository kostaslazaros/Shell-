#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/wait.h>
#include "shellfunc.h"


int main(int argc, char *argv[])
{
	if(argc == 2){
		//Code for execution once

		int input = 0;
		int first = 1;

		char* cmd = malloc(strlen(argv[1]) + 2);
		strcpy(cmd, argv[1]);
		strcat(cmd, "\n");

		char* next = strchr(cmd, '|'); /* Find first '|' */

		// printf("Line:%s cmd:%s next:%s\n", line, cmd, next);

		while (next != NULL) {
			/* 'next' points to '|' */
			*next = '\0';
			input = run(cmd, input, first, 0);
			cmd = next + 1;
			next = strchr(cmd, '|'); /* Find next '|' */
			first = 0;
		}
		input = run(cmd, input, first, 1);
		return 0;
	}

	printf("SHELL: Type 'exit' or send EOF to exit.\n");
	while (1) {
		/* Print the command prompt */
		printf("~> ");
		fflush(NULL);

		/* Read a command line */
		if (!fgets(line, 1024, stdin))
			return 0;

		int input = 0;
		int first = 1;

		char* cmd = line;
		char* next = strchr(cmd, '|'); /* Find first '|' */

		while (next != NULL) {
			/* 'next' points to '|' */
			*next = '\0';
			input = run(cmd, input, first, 0);
			cmd = next + 1;
			next = strchr(cmd, '|'); /* Find next '|' */
			first = 0;
		}
		input = run(cmd, input, first, 1);
		cleanup(100);
	}

	return 0;
}