/*
Name: Fionn McCloskey
Student Number: 21445606
I acknowledge the DCU Academic Integrity Policy
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <signal.h>
#include <fcntl.h>

// #include <myshell.h>


#define LINE_LIMIT 512
#define WORD_LIMIT 32
#define SEPARATORS " \t\n"

// function declarations
void print_prompt(char *prompt);
void read_input(char *line, int limit);
int tokenize_input(char *line, char **words, int limit);
void execute_command(char **words);
void change_directory(char **words);
void print_working_directory();

// main function
int main(int argc, char* argv[]) {
    // declare variables
    char line[LINE_LIMIT];
    char *words[WORD_LIMIT];
    char *prompt = "->";

    // loop until program is exited
    while (1) {
        // print prompt
        print_prompt(prompt);
        // read input
        read_input(line, LINE_LIMIT);
        // tokenize input into words
        int num_words = tokenize_input(line, words, WORD_LIMIT);
        if (num_words > 0) {
            // execute command
            execute_command(words);
        }
    }
    // exit program
    return 0;
}


// function to print prompt
void print_prompt(char *prompt) {
    // allocate memory for current working directory
    char *cwd = malloc(LINE_LIMIT * sizeof(char));
    // get current working directory
    getcwd(cwd, LINE_LIMIT);
    // print prompt and current working directory
    printf("%s [%s] ", prompt, cwd);
    // flush output buffer
    fflush(stdout);
    // free memory
    free(cwd);
}


// function to read input
void read_input(char *line, int limit) {
    // read input from standard input
    if (fgets(line, limit, stdin) == NULL) {
        // exit program if end of input is reached
        exit(0);
    }
}

// function to tokenize input into words
int tokenize_input(char *line, char **words, int limit) {
    // initialize variables
    int num_words = 0;
    char *token = strtok(line, SEPARATORS);
    // loop through input and separate into words
    while (token != NULL && num_words < limit) {
        words[num_words++] = token;
        token = strtok(NULL, SEPARATORS);
    }
    // set the last word to NULL to terminate the array
    words[num_words] = NULL;
    // return the number of words
    return num_words;
}

// Executes a command entered by the user
void execute_command(char **words) {

    // file descriptors for input and output redirection
    int input_fd = -1;
    int output_fd = -1;
    int i = 0;
    while (words[i] != NULL) {
        if (strcmp(words[i], "<") == 0) { // check if input redirection is specified
            // open input file
            input_fd = open(words[i+1], O_RDONLY);
            if (input_fd == -1) { // check for error
                perror("open");
                return;
            }
            // remove "<" and inputfile from command
            memmove(&words[i], &words[i+2], (WORD_LIMIT-i-2) * sizeof(char *));
            break;
        }
        // same last code block - check for output, open file, check for error
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

    // checking for commands and executing them
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
    else if (strcmp(words[0], "echo") == 0) {
        // iterate over all input and print
        for (int j = 1; words[j] != NULL; j++) {
            printf("%s ", words[j]);
        }
        printf("\n");
    }
    else if (strcmp(words[0], "pause") == 0) {
        printf("Progam Paused. Press Enter\n");
        getchar(); // program sleeps until a key is pressed
        printf("Program resumed.\n");
    }
    else if (strcmp(words[0], "environ") == 0) {
        system("env");
    }
    // execute external commands
    else {
        // execute other commands here
        pid_t pid = fork(); // fork for to run commands simultaneously
        if (pid == -1) {// check for error
            perror("fork");
            exit(EXIT_FAILURE);
        } else if (pid == 0) { // only child process executes this
            execvp(words[0], words);
            perror("execvp");
            exit(EXIT_FAILURE);
        } else { // only parent process executes this
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
        if (result == -1) { // error checking
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
    // allocating memory for the path
    char *cwd = malloc(LINE_LIMIT * sizeof(char));
    // getting current working directory
    getcwd(cwd, LINE_LIMIT);
    printf("%s\n", cwd);
    free(cwd);
}
