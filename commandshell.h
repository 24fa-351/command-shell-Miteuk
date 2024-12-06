#ifndef COMMANDSHELL_H
#define COMMANDSHELL_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>
#include <fcntl.h>
#include <errno.h>
#include <ctype.h>

#define MAX_INPUT_SIZE 1024
#define MAX_ARG_SIZE 100
#define MAX_ENV_VARS 100

typedef struct {
    char *name;
    char *value;
} EnvVar;

extern EnvVar env_vars[MAX_ENV_VARS];
extern int env_var_count;

void set_env_var(const char *name, const char *value);
void unset_env_var(const char *name);
char *get_env_var(const char *name);
void replace_env_vars(char *input);
void execute_command(char *input);

#endif // COMMANDSHELL_H