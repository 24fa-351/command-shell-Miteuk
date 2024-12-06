#define _POSIX_C_SOURCE 200809L
#include <ctype.h>
#include "commandshell.h"

extern EnvVar env_vars[MAX_ENV_VARS];
extern int env_var_count;

void set_env_var(const char *name, const char *value) {
    for (int i = 0; i < env_var_count; i++) {
        if (strcmp(env_vars[i].name, name) == 0) {
            free(env_vars[i].value);
            env_vars[i].value = strdup(value);
            return;
        }
    }
    env_vars[env_var_count].name = strdup(name);
    env_vars[env_var_count].value = strdup(value);
    env_var_count++;
}

void unset_env_var(const char *name) {
    for (int i = 0; i < env_var_count; i++) {
        if (strcmp(env_vars[i].name, name) == 0) {
            free(env_vars[i].name);
            free(env_vars[i].value);
            env_vars[i] = env_vars[env_var_count - 1];
            env_var_count--;
            return;
        }
    }
}

char *get_env_var(const char *name) {
    for (int i = 0; i < env_var_count; i++) {
        if (strcmp(env_vars[i].name, name) == 0) {
            return env_vars[i].value;
        }
    }
    return NULL;
}

void replace_env_vars(char *input) {
    char buffer[MAX_INPUT_SIZE];
    char *start = input;
    char *end;
    buffer[0] = '\0';

    while ((start = strchr(start, '$')) != NULL) {
        strncat(buffer, input, start - input);
        end = start + 1;
        while (*end && (isalnum(*end) || *end == '_')) {
            end++;
        }
        char var_name[MAX_INPUT_SIZE];
        strncpy(var_name, start + 1, end - start - 1);
        var_name[end - start - 1] = '\0';
        char *value = get_env_var(var_name);
        if (value) {
            strcat(buffer, value);
        } else {
            // If the variable is not found, keep the original text
            strncat(buffer, start, end - start);
        }
        input = end;
        start = end;
    }
    strcat(buffer, input);
    strcpy(input, buffer);
}

void execute_command(char *input) {
    char *args[MAX_ARG_SIZE];
    int arg_count = 0;
    char *token = strtok(input, " ");
    while (token != NULL) {
        args[arg_count++] = token;
        token = strtok(NULL, " ");
    }
    args[arg_count] = NULL;

    if (strcmp(args[0], "cd") == 0) {
        if (arg_count < 2) {
            fprintf(stderr, "cd: missing argument\n");
        } else if (chdir(args[1]) != 0) {
            perror("cd");
        }
    } else if (strcmp(args[0], "pwd") == 0) {
        char cwd[MAX_INPUT_SIZE];
        if (getcwd(cwd, sizeof(cwd)) != NULL) {
            printf("%s\n", cwd);
        } else {
            perror("pwd");
        }
    } else if (strcmp(args[0], "set") == 0) {
        if (arg_count < 3) {
            fprintf(stderr, "set: missing arguments\n");
        } else {
            set_env_var(args[1], args[2]);
        }
    } else if (strcmp(args[0], "unset") == 0) {
        if (arg_count < 2) {
            fprintf(stderr, "unset: missing argument\n");
        } else {
            unset_env_var(args[1]);
        }
    } else {
        pid_t pid = fork();
        if (pid == 0) {
            execvp(args[0], args);
            perror("execvp");
            exit(EXIT_FAILURE);
        } else if (pid > 0) {
            wait(NULL);
        } else {
            perror("fork");
        }
    }
}