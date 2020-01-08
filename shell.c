/**********************************************************
*           Shell and client server programs              *
*   Made by: Konstantinos Lazaros - Nefeli Stefanatou     *
*	           Lesson: Advanced Programming               *
*	           D.I.B, University of Thessaly              *
**********************************************************/

#include <stdio.h>
#include <stdlib.h>
#include "shellfunc.h"


int main(int argc, char *argv[])
{
	if(argc == 2){ // Εκτέλεση με ορίσματα
		/* Επειδή δοκιμάσαμε σαν argument το '|' και διαπιστώσαμε ότι δεν περνάνε τα ορίσματα,
		μέσα στην main, αποφασίσαμε να χρησιμοποιούμε ένα εξτρα argument με χρήση διπλών εισαγωγικών (πχ: "argument") */
		runshell(argv[1]);
		return 0;
	}

	// Εκτέλεση διαδραστικά
	printf("SHELL: Type 'exit' or send EOF to exit.\n");
	while (1) {
		// Εκτύπωση του command prompt
		printf("~> ");
		fflush(NULL);

		// Ανάγνωση εντολής
		if (!fgets(line, 1024, stdin))
			return 0;
		run_children(line);
	}
	return 0;
}