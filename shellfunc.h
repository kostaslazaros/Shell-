#ifndef SHELL_FUNCTIONS_1
#define SHELL_FUNCTIONS_1

static char* args[512];
pid_t pid;
int command_pipe[2];

#define READ  0
#define WRITE 1


static char line[1024];

int command(int input, int first, int last);
void cleanup(int n);
int runshell(char* argv);
int run(char* cmd, int input, int first, int last);
void split(char* cmd);
char* skipwhite(char* s);


#endif