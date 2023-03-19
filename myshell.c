#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <signal.h>
#include <fcntl.h>

#define LINE_LIMIT 512
#define WORD_LIMIT 32
#define SEPARATORS " \t\n"

void print_prompt(char *prompt);
void read_input(char *line, int limit);
int tokenize_input(char *line, char **words, int limit);
void execute_command(char **words);
void change_directory(char **words);
void print_working_directory();

int main(int argc, char* argv[]) {
    char line[LINE_LIMIT];
    char *words[WORD_LIMIT];
    char *prompt = "->";

    while (1) {
        print_prompt(prompt);
        read_input(line, LINE_LIMIT);
        int num_words = tokenize_input(line, words, WORD_LIMIT);
        if (num_words > 0) {
            execute_command(words);
        }
    }

    return 0;
}

void print_prompt(char *prompt) {
    char *cwd = malloc(LINE_LIMIT * sizeof(char));
    getcwd(cwd, LINE_LIMIT);
    printf("%s [%s] ", prompt, cwd);
    fflush(stdout);
    free(cwd);
}

void read_input(char *line, int limit) {
    if (fgets(line, limit, stdin) == NULL) {
        exit(0);
    }
}

int tokenize_input(char *line, char **words, int limit) {
    int num_words = 0;
    char *token = strtok(line, SEPARATORS);
    while (token != NULL && num_words < limit) {
        words[num_words++] = token;
        token = strtok(NULL, SEPARATORS);
    }
    words[num_words] = NULL;
    return num_words;
}

void execute_command(char **words) {

    int input_fd = -1;
    int output_fd = -1;
    int i = 0;
    while (words[i] != NULL) {
        if (strcmp(words[i], "<") == 0) {
            input_fd = open(words[i+1], O_RDONLY);
            if (input_fd == -1) {
                perror("open");
                return;
            }
            // remove "<" and inputfile from command
            memmove(&words[i], &words[i+2], (WORD_LIMIT-i-2) * sizeof(char *));
            break;
        }
        else if (strcmp(words[i], ">") == 0) {
            output_fd = open(words[i+1], O_CREAT | O_WRONLY | O_TRUNC, 0666);
            if (output_fd == -1) {
                perror("open");
                return;
            }
            // remove ">" and outputfile from command
            memmove(&words[i], &words[i+2], (WORD_LIMIT-i-2) * sizeof(char *));
            break;
        }
        else if (strcmp(words[i], ">>") == 0) {
            output_fd = open(words[i+1], O_CREAT | O_WRONLY | O_APPEND, 0666);
            if (output_fd == -1) {
                perror("open");
                return;
            }
            // remove ">>" and outputfile from command
            memmove(&words[i], &words[i+2], (WORD_LIMIT-i-2) * sizeof(char *));
            break;
        }
        i++;
    }

    if (strcmp(words[0], "clr") == 0) {
        system("clear");
    }
    else if (strcmp(words[0], "dir") == 0) {
        system("ls -al");
    }
    else if (strcmp(words[0], "quit") == 0) {
        exit(0);
    }
    else if (strcmp(words[0], "cd") == 0) {
        change_directory(words);
    }
    else {
        // execute other commands here
        pid_t pid = fork();
        if (pid == -1) {
            perror("fork");
            exit(EXIT_FAILURE);
        } else if (pid == 0) { // child process
            execvp(words[0], words);
            perror("execvp");
            exit(EXIT_FAILURE);
        } else { // parent process
            int status;
            waitpid(pid, &status, 0);
        }
    }
}

void change_directory(char **words) {
    if (words[1] == NULL) {
        // no argument provided, print current directory
        print_working_directory();
        return;
    }
    else {
        // change to the specified directory
        int result = chdir(words[1]);
        if (result == -1) {
            perror("chdir");
            return;
        }
        // set PWD environment variable
        char *cwd = malloc(LINE_LIMIT * sizeof(char));
        getcwd(cwd, LINE_LIMIT);
        setenv("PWD", cwd, 1);
        free(cwd);
        // print new working directory
        print_working_directory();
    }
}

void print_working_directory() {
    char *cwd = malloc(LINE_LIMIT * sizeof(char));
    getcwd(cwd, LINE_LIMIT);
    printf("%s\n", cwd);
    free(cwd);
}
