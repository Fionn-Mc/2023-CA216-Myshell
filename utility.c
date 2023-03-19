/*
Name: Fionn McCloskey
Student Number: 21445606
I acknowledge the DCU Academic Integrity Policy
*/


// void print_prompt(char *prompt) {
//     char *cwd = malloc(LINE_LIMIT * sizeof(char));
//     getcwd(cwd, LINE_LIMIT);
//     printf("%s [%s] ", prompt, cwd);
//     fflush(stdout);
//     free(cwd);
// }

// void read_input(char *line, int limit) {
//     if (fgets(line, limit, stdin) == NULL) {
//         exit(0);
//     }
// }

// int tokenize_input(char *line, char **words, int limit) {
//     int num_words = 0;
//     char *token = strtok(line, SEPARATORS);
//     while (token != NULL && num_words < limit) {
//         words[num_words++] = token;
//         token = strtok(NULL, SEPARATORS);
//     }
//     words[num_words] = NULL;
//     return num_words;
// }

// void execute_command(char **words) {

//     int input_fd = -1;
//     int output_fd = -1;
//     int i = 0;
//     while (words[i] != NULL) {
//         if (strcmp(words[i], "<") == 0) {
//             input_fd = open(words[i+1], O_RDONLY);
//             if (input_fd == -1) {
//                 perror("open");
//                 return;
//             }
//             // remove "<" and inputfile from command
//             memmove(&words[i], &words[i+2], (WORD_LIMIT-i-2) * sizeof(char *));
//             break;
//         }
//         else if (strcmp(words[i], ">") == 0) {
//             output_fd = open(words[i+1], O_CREAT | O_WRONLY | O_TRUNC, 0666);
//             if (output_fd == -1) {
//                 perror("open");
//                 return;
//             }
//             // remove ">" and outputfile from command
//             memmove(&words[i], &words[i+2], (WORD_LIMIT-i-2) * sizeof(char *));
//             break;
//         }
//         else if (strcmp(words[i], ">>") == 0) {
//             output_fd = open(words[i+1], O_CREAT | O_WRONLY | O_APPEND, 0666);
//             if (output_fd == -1) {
//                 perror("open");
//                 return;
//             }
//             // remove ">>" and outputfile from command
//             memmove(&words[i], &words[i+2], (WORD_LIMIT-i-2) * sizeof(char *));
//             break;
//         }
//         i++;
//     }

//     if (strcmp(words[0], "clr") == 0) {
//         system("clear");
//     }
//     else if (strcmp(words[0], "dir") == 0) {
//         system("ls -al");
//     }
//     else if (strcmp(words[0], "quit") == 0) {
//         exit(0);
//     }
//     else if (strcmp(words[0], "cd") == 0) {
//         change_directory(words);
//     }
//     else if (strcmp(words[0], "environ") == 0) {
//         system("env");
//     }
//     else {
//         // execute other commands here
//         pid_t pid = fork();
//         if (pid == -1) {
//             perror("fork");
//             exit(EXIT_FAILURE);
//         } else if (pid == 0) { // child process
//             execvp(words[0], words);
//             perror("execvp");
//             exit(EXIT_FAILURE);
//         } else { // parent process
//             int status;
//             waitpid(pid, &status, 0);
//         }
//     }
// }

// void change_directory(char **words) {
//     if (words[1] == NULL) {
//         // no argument provided, print current directory
//         print_working_directory();
//         return;
//     }
//     else {
//         // change to the specified directory
//         int result = chdir(words[1]);
//         if (result == -1) {
//             perror("chdir");
//             return;
//         }
//         // set PWD environment variable
//         char *cwd = malloc(LINE_LIMIT * sizeof(char));
//         getcwd(cwd, LINE_LIMIT);
//         setenv("PWD", cwd, 1);
//         free(cwd);
//         // print new working directory
//         print_working_directory();
//     }
// }

// void print_working_directory() {
//     char *cwd = malloc(LINE_LIMIT * sizeof(char));
//     getcwd(cwd, LINE_LIMIT);
//     printf("%s\n", cwd);
//     free(cwd);
// }
