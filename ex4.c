// Created by Amit Cohen ,ID:315147330 on 4.5.2021.
// shell, Ex4
// 

#include <stdio.h>

#include <stdlib.h>

#include <string.h>

#include <unistd.h>

#include <sys/types.h>

#include <pwd.h>

#include <sys/wait.h>

#include <signal.h>

#define true 1
#define maxSize 512
int countWords(char * ); //This function counts how many words there are in a string
void buildCommandLine(); //This function composes the command line according to the desired format
void freeTheArray(char ** a);
char ** build_argv(char * a); //Assembling an array in favor of execvp, where each cell in the array will point to another string consisting of one word, with the last cell being NULL
int findThePipe(char * string);
void noPipes();
void one_pipe();
void two_pipes();
int findApostrophes(char * a);
char ** argv, ** firstA, ** secondA, ** thirdA;
char stringPar[maxSize];
int stat = 0, pidSon;

int main() {
    int bool = true;
    int amountCommands = 0;
    int amountPipes = 0;
    while (bool) {
        signal(SIGTSTP, SIG_IGN);//The father catches the SIGTSTP and ignores it
        buildCommandLine();
        fgets(stringPar, maxSize, stdin);
        if (strlen(stringPar) > 1)
            stringPar[strlen(stringPar) - 1] = '\0';
        else
            continue;
        int pipeNum = findThePipe(stringPar);//
        amountCommands++;
        amountPipes += (pipeNum);
        if (strcmp(stringPar, "done") == 0) {
            printf("Number of commands: %d\nNumber of pipes: %d \nSee you next time\n", amountCommands, amountPipes);
            return 0;
        }
        if (strcmp(stringPar, "fg") == 0) {//Finding fg and bringing the last process back to life
            kill(pidSon, SIGCONT);
            waitpid(pidSon, NULL, WUNTRACED);
            continue;
        }
        if (!pipeNum)
            noPipes();
        else if (pipeNum == 1)
            one_pipe();
        else if (pipeNum == 2)
            two_pipes();
        else
            continue;
    }
}
int findApostrophes(char * a) {
    for (int i = 0; i < strlen(a); i++)
        if (a[i] == '"')
            return 1;
    return 0;
}
char ** build_argv(char * a) { //Assembling an array in favor of execvp, where each cell in the array will point to another string consisting of one word, with the last cell being NULL
    int wordSize, index = 0, i = 0;
    char * word;
    if (!findApostrophes(a)) {
        argv = (char ** ) malloc(sizeof(char * ) * (countWords(a) + 1));
        if (argv == NULL) {
            fprintf(stderr, "malloc failed");
            exit(1);
        }
        while (index < strlen(a)) {
            if (a[index] != ' ' && a[index] != '\0') {
                wordSize = 0;
                for (; a[index] != ' ' && a[index] != '\0'; index++, wordSize++); //This line calculates the length of the word and advances the index
                word = (char * ) malloc(sizeof(char) * wordSize);
                if (word == NULL) {
                    fprintf(stderr, "malloc failed");
                    exit(1);
                }
                int j = 0;
                for (int p = index - wordSize; j < wordSize; p++, j++) //Copy the word from the large string to the small string
                    word[j] = a[p];
                word[j] = '\0';
                argv[i] = word;
                i++;
            } else
                index++;
        }
        argv[i] = NULL;
        return argv;
    } else {
        char * first, * second, * third;
        first = strtok(a, "\"");
        second = strtok(NULL, "\"");
        third = strtok(NULL, "\"");
        int howManyAlloc = countWords(first) + countWords(third) + 2;
        printf("HowManyAlloc%d\n", howManyAlloc);
        argv = (char ** ) malloc(sizeof(char * ) * (howManyAlloc));
        if (argv == NULL) {
            fprintf(stderr, "malloc failed");
            exit(1);
        }
        while (index < strlen(first)) {
            if (first[index] != ' ' && first[index] != '\0') {
                wordSize = 0;
                for (; first[index] != ' ' && first[index] != '\0'; index++, wordSize++); //This line calculates the length of the word and advances the index
                word = (char * ) malloc(sizeof(char) * wordSize);
                if (word == NULL) {
                    fprintf(stderr, "malloc failed");
                    exit(1);
                }
                for (int p = index - wordSize, j = 0; j < wordSize; p++, j++) //Copy the word from the large string to the small string
                {
                    word[j] = first[p];
                }
                argv[i] = word;
                i++;
            } else {
                index++;
            }
        }
        argv[i] = second;
        argv[i + 1] = NULL;
        return argv;
    }

}

int countWords(char * a) { //This function calculates how many words there are in the strings, each space defining the beginning of a new word
    int counter = 0, index;
    for (index = 0; a[index] != '\0' && index < strlen(a); index++) {
        if (a[index] != ' ' && a[index] != '\0') {
            counter++;
            for (; a[index] != '\0' && a[index] != ' ' && index < strlen(a); index++);
        }
    }
    return counter;
}

void buildCommandLine() { //This function composes the command line according to the desired format
    char user[maxSize];
    char dir[maxSize];
    struct passwd * pw;
    char cwd[maxSize];
    if ((pw = getpwuid(getuid())) == NULL)
        strcpy(user, "null");
    else
        strcpy(user, pw -> pw_name);
    if (getcwd(cwd, sizeof(cwd)) == NULL)
        strcpy(dir, "null");
    else
        strcpy(dir, cwd);
    strcat(strcat(strcat(user, "@"), dir), ">");
    printf("%s", user);
}
void freeTheArray(char ** a) {//This function runs on the two-dimensional array and free all the members of the array
    int i = 0;
    for (; a[i] != NULL; i++)
        free(a[i]);
    free(a);
}

int findThePipe(char * string) {//This function finds how many pipes there are
    int amountOfPipes = 0;
    for (int i = 0; i < strlen(string); i++)
        if (string[i] == '|')
            amountOfPipes++;
    return amountOfPipes;
}
void noPipes() {//This function refers to the case where there are no pipes
    pid_t son0;
    argv = build_argv(stringPar);
    son0 = fork(); //Creating a son process
    if (son0 == -1) {
        perror("Fork is failed");
        freeTheArray(argv);
        exit(1);
    } else if (son0 == 0) {
        signal(SIGTSTP, SIG_DFL);
        if (strcmp(argv[0], "cd") == 0) {
            puts("Command not supported (Yet)");
            freeTheArray(argv);
            exit(0);
        } else if (execvp(argv[0], argv) < 0) {
            puts("Command not found");
            freeTheArray(argv);
            exit(1);
        }
    } else {
        waitpid(son0, & stat, WUNTRACED); //Waiting for the son process
        if (WIFSTOPPED(stat))
            pidSon = son0;
        freeTheArray(argv);
    }
}

void one_pipe() {//This function refers to the case where there is one pipe

    int first_pipe_fd[2];
    pid_t son1, son2;
    if (pipe(first_pipe_fd) < 0) {
        perror("The pipe failed");
        exit(EXIT_FAILURE);
    }
    char * com1 = strtok(stringPar, "|");
    char * com2 = strtok(NULL, "|");
    firstA = build_argv(com1);
    secondA = build_argv(com2);
    if (strcmp(firstA[0], "cd") == 0 || strcmp(secondA[0], "cd") == 0) {
        puts("Command not supported (Yet)");
        freeTheArray(firstA);
        freeTheArray(secondA);
        return;
    }
    son1 = fork();
    if (son1 < 0) {
        perror("Fork is failed");
        firstA = build_argv(com1);
        secondA = build_argv(com2);
        exit(1);
    } else if (son1 > 0)
        son2 = fork();
    if (son1 == 0) {
        dup2(first_pipe_fd[1], STDOUT_FILENO);
        close(first_pipe_fd[0]);
        close(first_pipe_fd[1]);
        if (execvp(firstA[0], firstA) < 0) {
            perror("Command not found");
            freeTheArray(firstA);
            freeTheArray(secondA);
            //             freeTheArray(thirdA);
            exit(1);
        }
        exit(EXIT_SUCCESS);
    }
    if (son2 < 0) {
        perror("Fork is failed");
        freeTheArray(secondA);
        freeTheArray(firstA);
        exit(1);
    }
    if (son2 == 0) {
        dup2(first_pipe_fd[0], STDIN_FILENO);
        close(first_pipe_fd[0]);
        close(first_pipe_fd[1]);
        if (execvp(secondA[0], secondA) < 0) {
            perror("Command not found");
            freeTheArray(secondA);
            freeTheArray(firstA);
            exit(1);
        }
        exit(EXIT_SUCCESS);
    } else {
        close(first_pipe_fd[0]);
        close(first_pipe_fd[1]);
        wait(NULL);
        wait(NULL);
        freeTheArray(firstA);
        freeTheArray(secondA);
    }
}
void two_pipes() {//This function refers to the case where there are two pipes

    int first_pipe_fd[2], second_pipe_fd[2];
    pid_t son1, son2, son3;
    if (pipe(first_pipe_fd) < 0) {
        perror("The pipe failed");
        exit(EXIT_FAILURE);
    }
    if (pipe(second_pipe_fd) < 0) {
        perror("The pipe failed");
        exit(EXIT_FAILURE);
    }
    char * com1 = strtok(stringPar, "|");
    char * com2 = strtok(NULL, "|");
    char * com3 = strtok(NULL, "|");
    firstA = build_argv(com1);
    secondA = build_argv(com2);
    thirdA = build_argv(com3);
    if (strcmp(firstA[0], "cd") == 0 || strcmp(secondA[0], "cd") == 0 || strcmp(thirdA[0], "cd") == 0) {
        puts("Command not supported (Yet)");
        freeTheArray(firstA);
        freeTheArray(secondA);
        freeTheArray(thirdA);
        return;
    }
    son1 = fork();
    if (son1 < 0) {
        perror("Fork is failed");
        firstA = build_argv(com1);
        secondA = build_argv(com2);
        exit(1);
    } else if (son1 > 0)
        son2 = fork();
    if (son1 == 0) {
        dup2(first_pipe_fd[1], STDOUT_FILENO);
        close(first_pipe_fd[0]);
        close(first_pipe_fd[1]);
        close(second_pipe_fd[0]);
        close(second_pipe_fd[1]);
        if (execvp(firstA[0], firstA) < 0) {
            perror("Command not found");
            freeTheArray(firstA);
            freeTheArray(secondA);
            freeTheArray(thirdA);
            exit(1);
        }
        exit(EXIT_SUCCESS);
    }
    if (son2 < 0) {
        perror("Fork is failed");
        freeTheArray(secondA);
        freeTheArray(firstA);
        freeTheArray(thirdA);
        exit(1);
    }
    if (son2 > 0)
        son3 = fork();
    if (son2 == 0) {
        dup2(first_pipe_fd[0], STDIN_FILENO);
        dup2(second_pipe_fd[1], STDOUT_FILENO);
        close(first_pipe_fd[0]);
        close(first_pipe_fd[1]);
        close(second_pipe_fd[0]);
        close(second_pipe_fd[1]);
        if (execvp(secondA[0], secondA) < 0) {
            perror("Command not found");
            freeTheArray(thirdA);
            freeTheArray(secondA);
            freeTheArray(firstA);
            exit(1);
        }
        exit(EXIT_SUCCESS);
    }
    if (son3 < 0) {
        perror("Fork is failed");
        freeTheArray(thirdA);
        freeTheArray(secondA);
        freeTheArray(firstA);
        exit(1);
    }
    if (son3 == 0) {
        dup2(second_pipe_fd[0], STDIN_FILENO);
        close(second_pipe_fd[1]);
        close(second_pipe_fd[0]);
        close(first_pipe_fd[0]);
        close(first_pipe_fd[1]);
        if (execvp(thirdA[0], thirdA) < 0) {
            perror("Command not found");
            freeTheArray(thirdA);
            freeTheArray(secondA);
            freeTheArray(firstA);
            exit(1);
        }
        exit(EXIT_SUCCESS);
    } else {
        close(first_pipe_fd[1]);
        close(first_pipe_fd[0]);
        close(second_pipe_fd[0]);
        close(second_pipe_fd[1]);
        wait(NULL);
        wait(NULL);
        wait(NULL);
        freeTheArray(firstA);
        freeTheArray(secondA);
        freeTheArray(thirdA);
    }
}