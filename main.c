#include "commandshell.h"

EnvVar env_vars[MAX_ENV_VARS];
int env_var_count = 0;

int main() {
    char input[MAX_INPUT_SIZE];
    while (1) {
        printf("xsh# ");
        if (fgets(input, sizeof(input), stdin) == NULL) {
            break;
        }
        input[strcspn(input, "\n")] = '\0';
        replace_env_vars(input);
        if (strcmp(input, "quit") == 0 || strcmp(input, "exit") == 0) {
            break;
        }
        execute_command(input);
    }
    return 0;
}