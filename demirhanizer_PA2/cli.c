//Demirhan Izer PA2 Assignment

/*

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <ctype.h>
#include <fcntl.h>
#include <assert.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <math.h>
#include <pthread.h>
#include <stdbool.h>

pthread_mutex_t lock = PTHREAD_MUTEX_INITIALIZER;
//lock_t mutex;


#define maxC 1000
void remove_newline(char *str) {
    if (str == NULL || *str == '\0') {
        return;
    }

    size_t len = strlen(str);
    if (len > 0 && str[len - 1] == '\n') {
        str[len - 1] = '\0';
    }
}

 
bool ampersandCheck = false;
char redirection = 'a';
bool fileThere = false;
char fileName[100];
char** ArrayParsed(char* line, int *size) {
    redirection = 'a';
    ampersandCheck = false;
    fileThere = false;
    char **words = NULL;
    int count = 0;
    int sizeDynamic = 15;
    words = (char **) malloc(sizeDynamic * sizeof(char *));
    
    char *token = strtok(line, " ");
    while(token != NULL) {
        if (count >= sizeDynamic) {
              sizeDynamic *= 2;
              words = (char **)realloc(words, sizeDynamic * sizeof(char *));
        }
        
        
        if (strcmp(token, "&") == 0) {
            ampersandCheck = true;
        }
        
        else if (strcmp(token, ">") == 0 || strcmp(token, "<") == 0) {
            redirection = token[0];
            fileThere = true;
        }
        
        else if (fileThere) {
            strcpy(fileName, token);
            fileThere = false;
            
        }
        else {
            char *escape = malloc((strlen(token) * 2 + 1) * sizeof(char));
            int idx = 0;
            
            for (int i = 0 ; i < strlen(token)  ; i++) {
                if (token[i] == '-') {
                    //escape[idx++] = '\\';
                }
                escape[idx++] = token[i];
            }
            escape[idx] = '\0';
            
            words[count] = strdup(escape);
            count++;
            
        }
        token = strtok(NULL, " ");
    }
    
    *size = count;
    return words;
}

bool checkFileName(const char * line) {
    int len = strlen(line);
    if (len < 4) {
        return false;
    }
    
    const char *checkTxt = &line[len-4];
    return strcmp(checkTxt, ".txt") == 0;
}
void PrintStatements(char** words, int wordCount) {
    FILE *outputFile = fopen("parse.txt", "a");
    
    
            fprintf(outputFile, "----------\n");
            fprintf(outputFile, "Command: %s\n", words[0]);
            int commandCount = 1;
            //printf("%d\n", wordCount);
            //printf("%d, total count: %d\n", commandCount, wordCount);
            //printf("%d,   %c\n", words[commandCount][0], words[commandCount][0]);
            if (commandCount < wordCount && words[commandCount][0] != '-' && !checkFileName(words[commandCount])) {
                fprintf(outputFile, "Inputs: %s\n", words[1]);
                commandCount++;
            }
            //printf("%d, total count: %d\n", commandCount, wordCount);
            if (commandCount < wordCount && checkFileName(words[commandCount])) {
                commandCount++;
            }
            
            if (commandCount < wordCount && words[commandCount][0] == '-') {
                fprintf(outputFile, "Options: %s\n", words[commandCount]);
                commandCount++;
            }
            
            if (commandCount < wordCount && (words[commandCount][0] == '<' || words[commandCount][0] == '>') ) {
                fprintf(outputFile, "Redirection: %c\n", redirection);
                commandCount++;
            }
            //printf("%d, total count: %d\n", commandCount, wordCount);
            
            if (commandCount < wordCount && checkFileName(words[commandCount])) {
                commandCount++;
            }
            
            //printf("%d, total count: %d\n", commandCount, wordCount);
            
            if (ampersandCheck) {
                    fprintf(outputFile, "Background Job: y\n");
                    fprintf(outputFile, "----------\n");
            }
            
            if (!ampersandCheck) {
                fprintf(outputFile, "Background Job: n\n");
                fprintf(outputFile, "----------\n");
            }
            
            fflush(outputFile);
            fclose(outputFile);
                    
}

#define SIZE 1024

void *CommandRun(void * arg) {
    
    
    //pthread_mutex_unlock(&lock);
    
    
    //pipe(fd);
    //close(fd[1]);
    
    //char **words = (char **) arg;
    //int wordCount = 0;
    int fdRead = *((int *)arg);
    //close(fd[1]);
    char buffer[SIZE];
    ssize_t reads;
    pthread_mutex_lock(&lock);
    printf("---- %ld\n", pthread_self());
    pthread_mutex_unlock(&lock);
    while((reads = read(fdRead, buffer, SIZE)) > 0) {
        printf("%.*s", (int) reads, buffer);
    }
    
    
    
    pthread_mutex_lock(&lock);
    printf("---- %ld\n", pthread_self());
    pthread_mutex_unlock(&lock);
    //close(fdRead);
    //wait(NULL); //YENI SILDIM !!! => GERI GELDI TAKILIYOR
    fflush(stdin);
    pthread_exit(NULL);
    
    
}


int main() {
    pthread_t threads[150] = {0};
    int threadCount = 0;
    
    FILE *file;
    char **words;
    int Max_Count = 1000;
    int Max_Line = 100;
    
    char line[Max_Count];
    char lines[Max_Line][Max_Count];
    char lineFinal[Max_Count];
    
    char processes[100];
    int processCount = 0;
    
    file = fopen("commands.txt", "r");
    int countLine = 0;
    if (file == NULL) {
        perror("Error opening the file");
        return EXIT_FAILURE;
    }
    
    while(fgets(line, sizeof(line), file) != NULL) {
        strcpy(lines[countLine], line);
        remove_newline(lines[countLine]);
        countLine++;
    }
    
    fclose(file);

    for (int i = 0 ; i < countLine ; i++) {
        strcpy(lineFinal, lines[i]);
        
        //printf("%s\n", lineFinal);//holds as ["ls −l &", "wc output1 . txt &"];
        
        int wordCount;
        words = ArrayParsed(lineFinal, &wordCount);
        
        //pthread_create(&threads[threadNum], NULL, CommandRun, lineFinal);
        //threadNum++;
            
        PrintStatements(words, wordCount);
        char *wordsArray[wordCount+1];
        for (int i = 0 ; i < wordCount ; i++) {
            wordsArray[i] = words[i];
        }
        wordsArray[wordCount] = NULL;
        
        //printf("%s\n", fileName);
        if (strcmp(wordsArray[0], "wait") == 0) {
            for (int i = 0 ; i < processCount ; i++) {
                waitpid(processes[i], NULL, 0);
            }
            
            for (int i = 0 ; i < threadCount ; i++) {
                pthread_join(threads[i], NULL);
            }
        }
        if (redirection == '>') {//printed on file
            int outRed = fork();
            
            if (outRed == 0) {
                int fileOutput = open(fileName, O_WRONLY | O_CREAT | O_TRUNC, S_IRWXU);
                
                dup2(fileOutput, STDOUT_FILENO);
                
                execvp(wordsArray[0], wordsArray);
            }
            else {
                if(!ampersandCheck) {
                    processes[processCount] = outRed;
                    processCount++;
                    wait(NULL);
                }
                
                //close(fd[1]);
                
            }
        }
        
            
        else {//using pipe for sysout file. redirection of < or nothing
            int fd[2];
            pipe(fd);
            
            int fileInput = open(fileName, O_RDONLY);
            int inputRc = fork();
            
            if (inputRc == 0) {
                close(fd[1]);
                dup2(fd[1], STDOUT_FILENO);
                close(fd[0]);
                if (redirection == '<') {
                    dup2(fileInput, STDIN_FILENO);
                }
                
                execvp(wordsArray[0], wordsArray);
            }
            else {
                
                //pthread_t listenerThread;
                //threads[threadCount++] = listenerThread;
                
                pthread_create(&threads[threadCount], NULL, CommandRun, (void *)&fd[0]);
                threadCount++;
                //pthread_create(&, NULL, CommandRun, NULL);
                
                //pthread_join(listenerThread, NULL);
                
                //close(fd[1]);
                //close(fd[0]);
                //close(fd[1]);
                wait(NULL);
                pthread_mutex
                if (!ampersandCheck) {
                    pthread_join(threads[threadCount], NULL);
                    //wait(NULL);
                }
                else {
                    processes[processCount] = inputRc;
                    processCount++;
                    //wait(NULL);
                }
                
                
            }
            close(fd[0]);
            close(fd[1]);
        }
        
    }
    for (int i = 0 ; i < processCount ; i++) {
        waitpid(processes[i], NULL, 0);
    }
            
    for (int i = 0 ; i < 15 ; i++) {
        //printf("%ld \n", threads[i]);
        pthread_join(threads[i], NULL);
    }
    
}

*/



/*
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <ctype.h>
#include <fcntl.h>
#include <assert.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <math.h>
#include <pthread.h>
#include <stdbool.h>

pthread_mutex_t lock = PTHREAD_MUTEX_INITIALIZER;
//lock_t mutex;


#define maxC 1000
void remove_newline(char *str) {
    if (str == NULL || *str == '\0') {
        return;
    }

    size_t len = strlen(str);
    if (len > 0 && str[len - 1] == '\n') {
        str[len - 1] = '\0';
    }
}

 
bool ampersandCheck = false;
char redirection = 'a';
bool fileThere = false;
char fileName[100];
char** ArrayParsed(char* line, int *size) {
    redirection = 'a';
    ampersandCheck = false;
    fileThere = false;
    char **words = NULL;
    int count = 0;
    int sizeDynamic = 15;
    words = (char **) malloc(sizeDynamic * sizeof(char *));
    
    char *token = strtok(line, " ");
    while(token != NULL) {
        if (count >= sizeDynamic) {
              sizeDynamic *= 2;
              words = (char **)realloc(words, sizeDynamic * sizeof(char *));
        }
        
        
        if (strcmp(token, "&") == 0) {
            ampersandCheck = true;
        }
        
        else if (strcmp(token, ">") == 0 || strcmp(token, "<") == 0) {
            redirection = token[0];
            fileThere = true;
        }
        
        else if (fileThere) {
            strcpy(fileName, token);
            fileThere = false;
            
        }
        else {
            char *escape = malloc((strlen(token) * 2 + 1) * sizeof(char));
            int idx = 0;
            
            for (int i = 0 ; i < strlen(token)  ; i++) {
                if (token[i] == '-') {
                    //escape[idx++] = '\\';
                }
                escape[idx++] = token[i];
            }
            escape[idx] = '\0';
            
            words[count] = strdup(escape);
            count++;
            
        }
        token = strtok(NULL, " ");
    }
    
    *size = count;
    return words;
}

bool checkFileName(const char * line) {
    int len = strlen(line);
    if (len < 4) {
        return false;
    }
    
    const char *checkTxt = &line[len-4];
    return strcmp(checkTxt, ".txt") == 0;
}
void PrintStatements(char** words, int wordCount) {
    FILE *outputFile = fopen("parse.txt", "a");
    
    
            fprintf(outputFile, "----------\n");
            fprintf(outputFile, "Command: %s\n", words[0]);
            int commandCount = 1;
            //printf("%d\n", wordCount);
            //printf("%d, total count: %d\n", commandCount, wordCount);
            //printf("%d,   %c\n", words[commandCount][0], words[commandCount][0]);
            if (commandCount < wordCount && words[commandCount][0] != '-' && !checkFileName(words[commandCount])) {
                fprintf(outputFile, "Inputs: %s\n", words[1]);
                commandCount++;
            }
            //printf("%d, total count: %d\n", commandCount, wordCount);
            if (commandCount < wordCount && checkFileName(words[commandCount])) {
                commandCount++;
            }
            
            if (commandCount < wordCount && words[commandCount][0] == '-') {
                fprintf(outputFile, "Options: %s\n", words[commandCount]);
                commandCount++;
            }
            
            if (commandCount < wordCount && (words[commandCount][0] == '<' || words[commandCount][0] == '>') ) {
                fprintf(outputFile, "Redirection: %c\n", redirection);
                commandCount++;
            }
            //printf("%d, total count: %d\n", commandCount, wordCount);
            
            if (commandCount < wordCount && checkFileName(words[commandCount])) {
                commandCount++;
            }
            
            //printf("%d, total count: %d\n", commandCount, wordCount);
            
            if (ampersandCheck) {
                    fprintf(outputFile, "Background Job: y\n");
                    fprintf(outputFile, "----------\n");
            }
            
            if (!ampersandCheck) {
                fprintf(outputFile, "Background Job: n\n");
                fprintf(outputFile, "----------\n");
            }
            
            fflush(outputFile);
            fclose(outputFile);
                    
}

#define SIZE 1024
int fd_rw[2];
bool stop = false;
void *CommandRun(void * arg) {
    
    close(fd_rw[1]);
    //pthread_mutex_unlock(&lock);
    char buff[5];

    read(fd_rw[0], buff, sizeof(buff));
    
    //pipe(fd);
    //close(fd[1]);
    
    close(fd_rw[0]);
    //char **words = (char **) arg;
    //int wordCount = 0;
    int fdRead = *((int *)arg);
    //close(fd[1]);
    char buffer[SIZE];
    ssize_t reads;
    while (!stop) {
        //pthread_mutex_lock(&lock);
        printf("---- %ld\n", pthread_self());
        
        while((reads = read(fdRead, buffer, SIZE)) > 0) {
            printf("%.*s", (int) reads, buffer);
            pthread_mutex_unlock(&lock);
            
            pthread_mutex_lock(&lock);
        }
        fflush(stdin);
        
        
        
        printf("---- %ld\n", pthread_self());
        //pthread_mutex_unlock(&lock);
    
        close(fdRead);
        //wait(NULL); //YENI SILDIM !!! => GERI GELDI TAKILIYOR
        fflush(stdin);
    }
    pthread_exit(NULL);
    
    
}


int main() {
    pthread_t threads[150] = {0};
    int threadCount = 0;
    
    FILE *file;
    char **words;
    int Max_Count = 1000;
    int Max_Line = 100;
    
    char line[Max_Count];
    char lines[Max_Line][Max_Count];
    char lineFinal[Max_Count];
    
    char processes[100];
    int processCount = 0;
    
    file = fopen("commands.txt", "r");
    int countLine = 0;
    if (file == NULL) {
        perror("Error opening the file");
        return EXIT_FAILURE;
    }
    
    while(fgets(line, sizeof(line), file) != NULL) {
        strcpy(lines[countLine], line);
        remove_newline(lines[countLine]);
        countLine++;
    }
    
    fclose(file);

    for (int i = 0 ; i < countLine ; i++) {
        strcpy(lineFinal, lines[i]);
        
        //printf("%s\n", lineFinal);//holds as ["ls −l &", "wc output1 . txt &"];
        
        int wordCount;
        words = ArrayParsed(lineFinal, &wordCount);
        
        //pthread_create(&threads[threadNum], NULL, CommandRun, lineFinal);
        //threadNum++;
            
        PrintStatements(words, wordCount);
        char *wordsArray[wordCount+1];
        for (int i = 0 ; i < wordCount ; i++) {
            wordsArray[i] = words[i];
        }
        wordsArray[wordCount] = NULL;
        
        //printf("%s\n", fileName);
        if (strcmp(wordsArray[0], "wait") == 0) {
            for (int i = 0 ; i < processCount ; i++) {
                waitpid(processes[i], NULL, 0);
            }
            
            for (int i = 0 ; i < threadCount ; i++) {
                pthread_join(threads[i], NULL);
            }
        }
        if (redirection == '>') {//printed on file
            int outRed = fork();
            
            if (outRed == 0) {
                int fileOutput = open(fileName, O_WRONLY | O_CREAT | O_TRUNC, S_IRWXU);
                
                dup2(fileOutput, STDOUT_FILENO);
                
                execvp(wordsArray[0], wordsArray);
            }
            else {
                if(!ampersandCheck) {
                    wait(NULL);
                }
                processes[processCount] = outRed;
                processCount++;
                
                
            }
        }
        
            
        else {//using pipe for sysout file. redirection of < or nothing
            int fd[2];
            pipe(fd);
            
            pipe(fd_rw);
            int fileInput = open(fileName, O_RDONLY);
            int inputRc = fork();
            
            if (inputRc == 0) {
                


                //close(fd_rw[1]);
                
                close(fd[1]);
                dup2(fd[1], STDOUT_FILENO);
                close(fd[0]);
                if (redirection == '<') {
                    dup2(fileInput, STDIN_FILENO);
                }
                
                                
                execvp(wordsArray[0], wordsArray);
            }
            else {
                close(fd_rw[0]);

                close(fd_rw[1]);
                //pthread_t listenerThread;
                //threads[threadCount++] = listenerThread;
            
                
                pthread_create(&threads[threadCount], NULL, CommandRun, (void *)&fd[0]);
                threadCount++;
                //pthread_create(&, NULL, CommandRun, NULL);
                const char* data = "Hello";

                write(fd_rw[1], data, strlen(data));
                
                stop = true;
                //pthread_join(listenerThread, NULL);
                
                //close(fd[1]);
                close(fd[0]);
                close(fd[1]);
                //wait(NULL);
                
                if (!ampersandCheck) {
                    pthread_join(threads[threadCount], NULL);
                    wait(NULL);
                }
                else {
                    processes[processCount] = inputRc;
                    processCount++;
                    //wait(NULL);
                }
                
                
            }
            
            close(fd[0]);
            close(fd[1]);
        }
        
    }
    
    for (int i = 0 ; i < processCount ; i++) {
        pid_t pid = processes[i];
        int status;
        
        while(pid != 0) {
            if (waitpid(pid, &status, 0) != 1) {
                pid = 0;
            }
            else {
                pid = processes[i];
            }
        }
    }
        
    
    for (int i = 0 ; i < threadCount ; i++) {
        //printf("%ld \n", threads[i]);
        pthread_join(threads[i], NULL);
        wait(NULL);
    }
    
    
    //printf("%ld\n", threads[threadCount-1]);
    //waitpid(processes[0], NULL, 0);
    //pthread_join(threads[threadCount],NULL);
    
}
*/
/*

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <ctype.h>
#include <fcntl.h>
#include <assert.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <math.h>
#include <pthread.h>
#include <stdbool.h>

pthread_mutex_t lock = PTHREAD_MUTEX_INITIALIZER;
//lock_t mutex;


#define maxC 1000
void remove_newline(char *str) {
    if (str == NULL || *str == '\0') {
        return;
    }

    size_t len = strlen(str);
    if (len > 0 && str[len - 1] == '\n') {
        str[len - 1] = '\0';
    }
}

 
bool ampersandCheck = false;
char redirection = 'a';
bool fileThere = false;
char fileName[100];
char** ArrayParsed(char* line, int *size) {
    redirection = 'a';
    ampersandCheck = false;
    fileThere = false;
    char **words = NULL;
    int count = 0;
    int sizeDynamic = 15;
    words = (char **) malloc(sizeDynamic * sizeof(char *));
    
    char *token = strtok(line, " ");
    while(token != NULL) {
        if (count >= sizeDynamic) {
              sizeDynamic *= 2;
              words = (char **)realloc(words, sizeDynamic * sizeof(char *));
        }
        
        
        if (strcmp(token, "&") == 0) {
            ampersandCheck = true;
        }
        
        else if (strcmp(token, ">") == 0 || strcmp(token, "<") == 0) {
            redirection = token[0];
            fileThere = true;
        }
        
        else if (fileThere) {
            strcpy(fileName, token);
            fileThere = false;
            
        }
        else {
            char *escape = malloc((strlen(token) * 2 + 1) * sizeof(char));
            int idx = 0;
            
            for (int i = 0 ; i < strlen(token)  ; i++) {
                if (token[i] == '-') {
                    //escape[idx++] = '\\';
                }
                escape[idx++] = token[i];
            }
            escape[idx] = '\0';
            
            words[count] = strdup(escape);
            count++;
            
        }
        token = strtok(NULL, " ");
    }
    
    *size = count;
    return words;
}

bool checkFileName(const char * line) {
    int len = strlen(line);
    if (len < 4) {
        return false;
    }
    
    const char *checkTxt = &line[len-4];
    return strcmp(checkTxt, ".txt") == 0;
}
void PrintStatements(char** words, int wordCount) {
    FILE *outputFile = fopen("parse.txt", "a");
    
    
            fprintf(outputFile, "----------\n");
            fprintf(outputFile, "Command: %s\n", words[0]);
            int commandCount = 1;
            //printf("%d\n", wordCount);
            //printf("%d, total count: %d\n", commandCount, wordCount);
            //printf("%d,   %c\n", words[commandCount][0], words[commandCount][0]);
            if (commandCount < wordCount && words[commandCount][0] != '-' && !checkFileName(words[commandCount])) {
                fprintf(outputFile, "Inputs: %s\n", words[1]);
                commandCount++;
            }
            //printf("%d, total count: %d\n", commandCount, wordCount);
            if (commandCount < wordCount && checkFileName(words[commandCount])) {
                commandCount++;
            }
            
            if (commandCount < wordCount && words[commandCount][0] == '-') {
                fprintf(outputFile, "Options: %s\n", words[commandCount]);
                commandCount++;
            }
            
            if (commandCount < wordCount && (words[commandCount][0] == '<' || words[commandCount][0] == '>') ) {
                fprintf(outputFile, "Redirection: %c\n", redirection);
                commandCount++;
            }
            //printf("%d, total count: %d\n", commandCount, wordCount);
            
            if (commandCount < wordCount && checkFileName(words[commandCount])) {
                commandCount++;
            }
            
            //printf("%d, total count: %d\n", commandCount, wordCount);
            
            if (ampersandCheck) {
                    fprintf(outputFile, "Background Job: y\n");
                    fprintf(outputFile, "----------\n");
            }
            
            if (!ampersandCheck) {
                fprintf(outputFile, "Background Job: n\n");
                fprintf(outputFile, "----------\n");
            }
            
            fflush(outputFile);
            fclose(outputFile);
                    
}

#define SIZE 1024
int fd_rw[2];

void *CommandRun(void * arg) {
    
    
    close(fd_rw[1]);
    //pthread_mutex_unlock(&lock);
    char buff[5];

    read(fd_rw[0], buff, sizeof(buff));
    
    //pipe(fd);
    //close(fd[1]);
    
    close(fd_rw[0]);
    //char **words = (char **) arg;
    //int wordCount = 0;
    int fdRead = *((int *)arg);
    //close(fd[1]);
    char buffer[SIZE];
    ssize_t reads;
    
    pthread_mutex_lock(&lock);
    printf("---- %ld\n", pthread_self());
    pthread_mutex_unlock(&lock);
    while((reads = read(fdRead, buffer, SIZE)) > 0) {
        printf("%.*s", (int) reads, buffer);
    }
    
    
    
    pthread_mutex_lock(&lock);
    printf("---- %ld\n", pthread_self());
    pthread_mutex_unlock(&lock);
    //close(fdRead);
    //wait(NULL); //YENI SILDIM !!! => GERI GELDI TAKILIYOR
    fflush(stdin);
    pthread_exit(NULL);
    
    
}


int main() {
    pthread_t threads[150] = {0};
    int threadCount = 0;
    
    FILE *file;
    char **words;
    int Max_Count = 1000;
    int Max_Line = 100;
    
    char line[Max_Count];
    char lines[Max_Line][Max_Count];
    char lineFinal[Max_Count];
    
    char processes[100];
    int processCount = 0;
    
    file = fopen("commands.txt", "r");
    int countLine = 0;
    if (file == NULL) {
        perror("Error opening the file");
        return EXIT_FAILURE;
    }
    
    while(fgets(line, sizeof(line), file) != NULL) {
        strcpy(lines[countLine], line);
        remove_newline(lines[countLine]);
        countLine++;
    }
    
    fclose(file);

    for (int i = 0 ; i < countLine ; i++) {
        strcpy(lineFinal, lines[i]);
        
        //printf("%s\n", lineFinal);//holds as ["ls −l &", "wc output1 . txt &"];
        
        int wordCount;
        words = ArrayParsed(lineFinal, &wordCount);
        
        //pthread_create(&threads[threadNum], NULL, CommandRun, lineFinal);
        //threadNum++;
            
        PrintStatements(words, wordCount);
        char *wordsArray[wordCount+1];
        for (int i = 0 ; i < wordCount ; i++) {
            wordsArray[i] = words[i];
        }
        wordsArray[wordCount] = NULL;
        
        //printf("%s\n", fileName);
        if (strcmp(wordsArray[0], "wait") == 0) {
            for (int i = 0 ; i < processCount ; i++) {
                waitpid(processes[i], NULL, 0);
            }
            
            for (int i = 0 ; i < threadCount ; i++) {
                pthread_join(threads[i], NULL);
            }
        }
        if (redirection == '>') {//printed on file
            int outRed = fork();
            
            if (outRed == 0) {
                int fileOutput = open(fileName, O_WRONLY | O_CREAT | O_TRUNC, S_IRWXU);
                
                dup2(fileOutput, STDOUT_FILENO);
                
                execvp(wordsArray[0], wordsArray);
            }
            else {
                if(!ampersandCheck) {
                    processes[processCount] = outRed;
                    processCount++;
                    wait(NULL);
                }
                
                //close(fd[1]);
                
            }
        }
        
            
        else {//using pipe for sysout file. redirection of < or nothing
            int fd[2];
            pipe(fd);
            
            int fileInput = open(fileName, O_RDONLY);
            int inputRc = fork();
            
            if (inputRc == 0) {
                close(fd[1]);
                dup2(fd[1], STDOUT_FILENO);
                close(fd[0]);
                if (redirection == '<') {
                    dup2(fileInput, STDIN_FILENO);
                }
                
                execvp(wordsArray[0], wordsArray);
            }
            else {
                
                close(fd_rw[0]);

                close(fd_rw[1]);
                //pthread_t listenerThread;
                //threads[threadCount++] = listenerThread;
            
                
                pthread_create(&threads[threadCount], NULL, CommandRun, (void *)&fd[0]);
                threadCount++;
                //pthread_create(&, NULL, CommandRun, NULL);
                const char* data = "Hello";

                write(fd_rw[1], data, strlen(data));
                
                close(fd[0]);
                close(fd[1]);
                if (!ampersandCheck) {
                    pthread_join(threads[threadCount], NULL);
                    fflush(stdin);
                    //wait(NULL);
                }
                else {
                    processes[processCount] = inputRc;
                    processCount++;
                    //wait(NULL);
                }
                
                
            }
            close(fd[0]);
            close(fd[1]);
        }
        
    }
    for (int i = 0 ; i < processCount ; i++) {
        waitpid(processes[i], NULL, 0);
    
    }
            
    for (int i = 0 ; i < 15 ; i++) {
        //printf("%ld \n", threads[i]);
        pthread_join(threads[i], NULL);
        wait(NULL);
    }
    
}
*/
/*
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <ctype.h>
#include <fcntl.h>
#include <assert.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <math.h>
#include <pthread.h>
#include <stdbool.h>

pthread_mutex_t lock = PTHREAD_MUTEX_INITIALIZER;
//lock_t mutex;


#define maxC 1000
void remove_newline(char *str) {
    if (str == NULL || *str == '\0') {
        return;
    }

    size_t len = strlen(str);
    if (len > 0 && str[len - 1] == '\n') {
        str[len - 1] = '\0';
    }
}

 
bool ampersandCheck = false;
char redirection = 'a';
bool fileThere = false;
char fileName[100];
char** ArrayParsed(char* line, int *size) {
    redirection = 'a';
    ampersandCheck = false;
    fileThere = false;
    char **words = NULL;
    int count = 0;
    int sizeDynamic = 15;
    words = (char **) malloc(sizeDynamic * sizeof(char *));
    
    char *token = strtok(line, " ");
    while(token != NULL) {
        if (count >= sizeDynamic) {
              sizeDynamic *= 2;
              words = (char **)realloc(words, sizeDynamic * sizeof(char *));
        }
        
        
        if (strcmp(token, "&") == 0) {
            ampersandCheck = true;
        }
        
        else if (strcmp(token, ">") == 0 || strcmp(token, "<") == 0) {
            redirection = token[0];
            fileThere = true;
        }
        
        else if (fileThere) {
            strcpy(fileName, token);
            fileThere = false;
            
        }
        else {
            char *escape = malloc((strlen(token) * 2 + 1) * sizeof(char));
            int idx = 0;
            
            for (int i = 0 ; i < strlen(token)  ; i++) {
                if (token[i] == '-') {
                    //escape[idx++] = '\\';
                }
                escape[idx++] = token[i];
            }
            escape[idx] = '\0';
            
            words[count] = strdup(escape);
            count++;
            
        }
        token = strtok(NULL, " ");
    }
    
    *size = count;
    return words;
}

bool checkFileName(const char * line) {
    int len = strlen(line);
    if (len < 4) {
        return false;
    }
    
    const char *checkTxt = &line[len-4];
    return strcmp(checkTxt, ".txt") == 0;
}
void PrintStatements(char** words, int wordCount) {
    FILE *outputFile = fopen("parse.txt", "a");
    
    
            fprintf(outputFile, "----------\n");
            fprintf(outputFile, "Command: %s\n", words[0]);
            int commandCount = 1;
            //printf("%d\n", wordCount);
            //printf("%d, total count: %d\n", commandCount, wordCount);
            //printf("%d,   %c\n", words[commandCount][0], words[commandCount][0]);
            if (commandCount < wordCount && words[commandCount][0] != '-' && !checkFileName(words[commandCount])) {
                fprintf(outputFile, "Inputs: %s\n", words[1]);
                commandCount++;
            }
            //printf("%d, total count: %d\n", commandCount, wordCount);
            if (commandCount < wordCount && checkFileName(words[commandCount])) {
                commandCount++;
            }
            
            if (commandCount < wordCount && words[commandCount][0] == '-') {
                fprintf(outputFile, "Options: %s\n", words[commandCount]);
                commandCount++;
            }
            
            if (commandCount < wordCount && (words[commandCount][0] == '<' || words[commandCount][0] == '>') ) {
                fprintf(outputFile, "Redirection: %c\n", redirection);
                commandCount++;
            }
            //printf("%d, total count: %d\n", commandCount, wordCount);
            
            if (commandCount < wordCount && checkFileName(words[commandCount])) {
                commandCount++;
            }
            
            //printf("%d, total count: %d\n", commandCount, wordCount);
            
            if (ampersandCheck) {
                    fprintf(outputFile, "Background Job: y\n");
                    fprintf(outputFile, "----------\n");
            }
            
            if (!ampersandCheck) {
                fprintf(outputFile, "Background Job: n\n");
                fprintf(outputFile, "----------\n");
            }
            
            fflush(outputFile);
            fclose(outputFile);
                    
}

#define SIZE 1024
int fd_rw[2];
bool stop = false;
void *CommandRun(void * arg) {
    //pthread_mutex_lock(&lock);
    int fdRead = *((int *)arg);
    close(fd_rw[1]);
    //pthread_mutex_unlock(&lock);
    char buff[5];

    read(fd_rw[0], buff, sizeof(buff));
    
    stop = false;
    
    
    //close(fd_rw[0]);
    char buffer[SIZE];
    ssize_t reads;
    //pthread_mutex_lock(&lock);
    //printf("---- %ld\n", pthread_self());
    //pthread_mutex_unlock(&lock);
    //pthread_mutex_lock(&lock);
    while (!stop) {
        pthread_mutex_lock(&lock);
        //stop = false;
        printf("---- %ld\n", pthread_self());
        //pthread_mutex_unlock(&lock);
        while((reads = read(fdRead, buffer, SIZE)) > 0) {
            printf("%.*s", (int) reads, buffer);
        }
        stop = true;
        //pthread_mutex_lock(&lock);
        printf("---- %ld\n", pthread_self());
        pthread_mutex_unlock(&lock);
        fflush(stdin);
        
    }
    //stop = true;
    
    //pthread_mutex_unlock(&lock);
    //pthread_mutex_lock(&lock);
    //printf("---- %ld\n", pthread_self());
    //pthread_mutex_unlock(&lock);
    pthread_exit(NULL);
    
    
}


int main() {
    pthread_t threads[150] = {0};
    int threadCount = 0;
    
    FILE *file;
    char **words;
    int Max_Count = 1000;
    int Max_Line = 100;
    
    char line[Max_Count];
    char lines[Max_Line][Max_Count];
    char lineFinal[Max_Count];
    
    char processes[100];
    int processCount = 0;
    
    file = fopen("commands.txt", "r");
    int countLine = 0;
    if (file == NULL) {
        perror("Error opening the file");
        return EXIT_FAILURE;
    }
    
    while(fgets(line, sizeof(line), file) != NULL) {
        strcpy(lines[countLine], line);
        remove_newline(lines[countLine]);
        countLine++;
    }
    
    fclose(file);
    //stop= true;
    for (int i = 0 ; i < countLine ; i++) {
        strcpy(lineFinal, lines[i]);
        
        //printf("%s\n", lineFinal);//holds as ["ls −l &", "wc output1 . txt &"];
        
        int wordCount;
        words = ArrayParsed(lineFinal, &wordCount);
        
        //pthread_create(&threads[threadNum], NULL, CommandRun, lineFinal);
        //threadNum++;
            
        PrintStatements(words, wordCount);
        char *wordsArray[wordCount+1];
        for (int i = 0 ; i < wordCount ; i++) {
            wordsArray[i] = words[i];
        }
        wordsArray[wordCount] = NULL;
        //stop = true;
        //printf("%s\n", fileName);
        if (strcmp(wordsArray[0], "wait") == 0) {
            for (int i = 0 ; i < processCount ; i++) {
                waitpid(processes[i], NULL, 0);
            }
            
            for (int i = 0 ; i < threadCount ; i++) {
                pthread_join(threads[i], NULL);
            }
        }
        if (redirection == '>') {//printed on file
            int outRed = fork();
            
            if (outRed == 0) {
                int fileOutput = open(fileName, O_WRONLY | O_CREAT | O_TRUNC, S_IRWXU);
                
                dup2(fileOutput, STDOUT_FILENO);
                
                execvp(wordsArray[0], wordsArray);
            }
            else {
                if(!ampersandCheck) {
                    processes[processCount] = outRed;
                    processCount++;
                    wait(NULL);
                }
                
                //close(fd[1]);
                
            }
        }
        
            
        else {//using pipe for sysout file. redirection of < or nothing
            int fd[2];
            pipe(fd);
            
            //stop = false;
            
            int fileInput = open(fileName, O_RDONLY);
            int inputRc = fork();
            
            if (inputRc == 0) {
                close(fd[1]);
                dup2(fd[1], STDOUT_FILENO);
                close(fd[0]);
                if (redirection == '<') {
                    dup2(fileInput, STDIN_FILENO);
                }
                //stop = true;
                execvp(wordsArray[0], wordsArray);
            }
            else {
                //stop = true;
                close(fd_rw[0]);
                
                close(fd_rw[1]);
                //stop = true;
                //stop = false;
                //pthread_t listenerThread;
                //threads[threadCount++] = listenerThread;
            
                
                pthread_create(&threads[threadCount], NULL, CommandRun, (void *)&fd[0]);
                threadCount++;
                //stop = true;
                //pthread_create(&, NULL, CommandRun, NULL);
                const char* data = "Hello";

                write(fd_rw[1], data, strlen(data));
                //stop = true;
                //close(fd[0]);
                //close(fd[1]);
                //stop = true;
                
                if (!ampersandCheck) {
                    //stop = true;
                    pthread_join(threads[threadCount], NULL);
                    //fflush(stdin);
                    
                    stop = true;
                    
                }
                else {
                    stop = true;
                    processes[processCount] = inputRc;
                    processCount++;
                    
                }
                
                //stop = true;
            }
            //stop = true;
            close(fd[0]);
            //stop = true;
            close(fd[1]);
            //stop = true;
        }
        //stop = true;
    }
    
    for (int i = 0 ; i < processCount ; i++) {
        waitpid(processes[i], NULL, 0);
    
    }
            
    for (int i = 0 ; i < 15 ; i++) {
        //printf("%ld \n", threads[i]);
        pthread_join(threads[i], NULL);
        wait(NULL);
    }
    
}
*/


/*
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <ctype.h>
#include <fcntl.h>
#include <assert.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <math.h>
#include <pthread.h>
#include <stdbool.h>

pthread_mutex_t lock = PTHREAD_MUTEX_INITIALIZER;
//lock_t mutex;


#define maxC 1000
void remove_newline(char *str) {
    if (str == NULL || *str == '\0') {
        return;
    }

    size_t len = strlen(str);
    if (len > 0 && str[len - 1] == '\n') {
        str[len - 1] = '\0';
    }
}

 
bool ampersandCheck = false;
char redirection = 'a';
bool fileThere = false;
char fileName[100];
char** ArrayParsed(char* line, int *size) {
    redirection = 'a';
    ampersandCheck = false;
    fileThere = false;
    char **words = NULL;
    int count = 0;
    int sizeDynamic = 15;
    words = (char **) malloc(sizeDynamic * sizeof(char *));
    
    char *token = strtok(line, " ");
    while(token != NULL) {
        if (count >= sizeDynamic) {
              sizeDynamic *= 2;
              words = (char **)realloc(words, sizeDynamic * sizeof(char *));
        }
        
        
        if (strcmp(token, "&") == 0) {
            ampersandCheck = true;
        }
        
        else if (strcmp(token, ">") == 0 || strcmp(token, "<") == 0) {
            redirection = token[0];
            fileThere = true;
        }
        
        else if (fileThere) {
            strcpy(fileName, token);
            fileThere = false;
            
        }
        else {
            char *escape = malloc((strlen(token) * 2 + 1) * sizeof(char));
            int idx = 0;
            
            for (int i = 0 ; i < strlen(token)  ; i++) {
                if (token[i] == '-') {
                    //escape[idx++] = '\\';
                }
                escape[idx++] = token[i];
            }
            escape[idx] = '\0';
            
            words[count] = strdup(escape);
            count++;
            
        }
        token = strtok(NULL, " ");
    }
    
    *size = count;
    return words;
}

bool checkFileName(const char * line) {
    int len = strlen(line);
    if (len < 4) {
        return false;
    }
    
    const char *checkTxt = &line[len-4];
    return strcmp(checkTxt, ".txt") == 0;
}
void PrintStatements(char** words, int wordCount) {
    FILE *outputFile = fopen("parse.txt", "a");
    
    
            fprintf(outputFile, "----------\n");
            fprintf(outputFile, "Command: %s\n", words[0]);
            int commandCount = 1;
            //printf("%d\n", wordCount);
            //printf("%d, total count: %d\n", commandCount, wordCount);
            //printf("%d,   %c\n", words[commandCount][0], words[commandCount][0]);
            if (commandCount < wordCount && words[commandCount][0] != '-' && !checkFileName(words[commandCount])) {
                fprintf(outputFile, "Inputs: %s\n", words[1]);
                commandCount++;
            }
            //printf("%d, total count: %d\n", commandCount, wordCount);
            if (commandCount < wordCount && checkFileName(words[commandCount])) {
                commandCount++;
            }
            
            if (commandCount < wordCount && words[commandCount][0] == '-') {
                fprintf(outputFile, "Options: %s\n", words[commandCount]);
                commandCount++;
            }
            
            if (commandCount < wordCount && (words[commandCount][0] == '<' || words[commandCount][0] == '>') ) {
                fprintf(outputFile, "Redirection: %c\n", redirection);
                commandCount++;
            }
            //printf("%d, total count: %d\n", commandCount, wordCount);
            
            if (commandCount < wordCount && checkFileName(words[commandCount])) {
                commandCount++;
            }
            
            //printf("%d, total count: %d\n", commandCount, wordCount);
            
            if (ampersandCheck) {
                    fprintf(outputFile, "Background Job: y\n");
                    fprintf(outputFile, "----------\n");
            }
            
            if (!ampersandCheck) {
                fprintf(outputFile, "Background Job: n\n");
                fprintf(outputFile, "----------\n");
            }
            
            fflush(outputFile);
            fclose(outputFile);
                    
}

#define SIZE 1024
int fd_rw[2];
bool stop = false;
bool checkRead = false;


void *CommandRun(void * arg) {
    //pthread_mutex_lock(&lock);
    char buff[6];

    read(fd_rw[0], buff, sizeof(buff));

    int fdRead = *((int *)arg);
    close(fd_rw[1]);
    close(fd_rw[0]);
    //pthread_mutex_unlock(&lock);
    

    //read(fd_rw[0], buff, sizeof(buff));
    
    
    pthread_mutex_lock(&lock);
    checkRead = true;
    pthread_mutex_unlock(&lock);
    stop = false;
    
    
    //close(fd_rw[0]);
    char buffer[SIZE];
    ssize_t reads;
    //pthread_mutex_lock(&lock);
    //printf("---- %ld\n", pthread_self());
    //pthread_mutex_unlock(&lock);
    

    
    
     while (!stop && checkRead) {
        pthread_mutex_lock(&lock);
        //stop = false;
        printf("---- %ld\n", pthread_self());
        fflush(stdout);
        //pthread_mutex_unlock(&lock);
        while((reads = read(fdRead, buffer, SIZE)) > 0) {
            printf("%.*s", (int) reads, buffer);
            fflush(stdout);
        }
        stop = true;
        //pthread_mutex_lock(&lock);
        printf("---- %ld\n", pthread_self());
        fflush(stdout);
        pthread_mutex_unlock(&lock);
        
        
    }
    //stop = true;
    
    //pthread_mutex_unlock(&lock);
    //pthread_mutex_lock(&lock);
    //printf("---- %ld\n", pthread_self());
    //pthread_mutex_unlock(&lock);
    pthread_exit(NULL);
    
    
}


int main() {
    pthread_t threads[150] = {0};
    int threadCount = 0;
    
    FILE *file;
    char **words;
    int Max_Count = 1000;
    int Max_Line = 100;
    
    char line[Max_Count];
    char lines[Max_Line][Max_Count];
    char lineFinal[Max_Count];
    
    char processes[100];
    int processCount = 0;
    
    file = fopen("commands.txt", "r");
    int countLine = 0;
    if (file == NULL) {
        perror("Error opening the file");
        return EXIT_FAILURE;
    }
    
    while(fgets(line, sizeof(line), file) != NULL) {
        strcpy(lines[countLine], line);
        remove_newline(lines[countLine]);
        countLine++;
    }
    
    fclose(file);
    //stop= true;
    for (int i = 0 ; i < countLine ; i++) {
        strcpy(lineFinal, lines[i]);
        
        //printf("%s\n", lineFinal);//holds as ["ls −l &", "wc output1 . txt &"];
        
        int wordCount;
        words = ArrayParsed(lineFinal, &wordCount);
        
        //pthread_create(&threads[threadNum], NULL, CommandRun, lineFinal);
        //threadNum++;
            
        PrintStatements(words, wordCount);
        char *wordsArray[wordCount+1];
        for (int i = 0 ; i < wordCount ; i++) {
            wordsArray[i] = words[i];
        }
        wordsArray[wordCount] = NULL;
        //stop = true;
        //printf("%s\n", fileName);
        if (strcmp(wordsArray[0], "wait") == 0) {
            for (int i = 0 ; i < processCount ; i++) {
                waitpid(processes[i], NULL, 0);
            }
            
            for (int i = 0 ; i < threadCount ; i++) {
                pthread_join(threads[i], NULL);
            }
        }
        if (redirection == '>') {//printed on file
            int outRed = fork();
            
            if (outRed == 0) {
                int fileOutput = open(fileName, O_WRONLY | O_CREAT | O_TRUNC, S_IRWXU);
                
                dup2(fileOutput, STDOUT_FILENO);
                
                execvp(wordsArray[0], wordsArray);
            }
            else {
                if(!ampersandCheck) {
                    processes[processCount] = outRed;
                    processCount++;
                    wait(NULL);
                }
                
                //close(fd[1]);
                
            }
        }
        
            
        else {//using pipe for sysout file. redirection of < or nothing
            int fd[2];
            pipe(fd);
            
            //stop = false;
            
            
            int fileInput = open(fileName, O_RDONLY);
            int inputRc = fork();
            
            if (inputRc == 0) {
                close(fd[1]);
                dup2(fd[1], STDOUT_FILENO);
                close(fd[0]);
                if (redirection == '<') {
                    dup2(fileInput, STDIN_FILENO);
                }
                //stop = true;
                
                
                const char* data = "Hello";

                write(fd_rw[1], data, strlen(data));
                
                execvp(wordsArray[0], wordsArray);
            }
            else {
                close(fd[1]);
                pthread_mutex_lock(&lock);
                //stop = true;
                close(fd_rw[0]);
                
                close(fd_rw[1]);
                
                pthread_mutex_unlock(&lock);
                
                //stop = true;
                //stop = false;
                //pthread_t listenerThread;
                //threads[threadCount++] = listenerThread;
                
                
                                
                pthread_create(&threads[threadCount], NULL, CommandRun, (void *)&fd[0]);
                threadCount++;
                
                
                
                close(fd[1]);
                close(fd[0]);
                
                
                if (!ampersandCheck) {
                    //stop = true;
                    pthread_join(threads[threadCount], NULL);
                    //fflush(stdin);
                    
                    stop = true;
                    
                }
                else {
                    stop = true;
                    processes[processCount] = inputRc;
                    processCount++;
                    
                }
                
                //stop = true;
            }
            //stop = true;
            close(fd[0]);
            //stop = true;
            close(fd[1]);
            //stop = true;
        }
        //stop = true;
    }
    
    for (int i = 0 ; i < processCount ; i++) {
        waitpid(processes[i], NULL, 0);
    
    }
            
    for (int i = 0 ; i < 15 ; i++) {
        //printf("%ld \n", threads[i]);
        pthread_join(threads[i], NULL);
        wait(NULL);
    }
    
}
*/



#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <ctype.h>
#include <fcntl.h>
#include <assert.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <math.h>
#include <pthread.h>
#include <stdbool.h>

pthread_mutex_t lock = PTHREAD_MUTEX_INITIALIZER;
//lock_t mutex;



bool commandExecuted = false;

#define maxC 1000
void remove_newline(char *str) {
    if (str == NULL || *str == '\0') {
        return;
    }

    size_t len = strlen(str);
    if (len > 0 && str[len - 1] == '\n') {
        str[len - 1] = '\0';
    }
}

 
bool ampersandCheck = false;
char redirection = '-';
bool fileThere = false;
char fileName[100];
char input[100];
char option[100];
char** ArrayParsed(char* line, int *size) {
    strcpy(option, "");
    strcpy(input, "");
    redirection = '-';
    ampersandCheck = false;
    fileThere = false;
    char **words = NULL;
    int count = 0;
    int sizeDynamic = 15;
    int i = 0;
    words = (char **) malloc(sizeDynamic * sizeof(char *));
    
    char *token = strtok(line, " ");
    while(token != NULL) {
        if (count >= sizeDynamic) {
              sizeDynamic *= 2;
              words = (char **)realloc(words, sizeDynamic * sizeof(char *));
        }
        
        
        if (strcmp(token, "&") == 0) {
            ampersandCheck = true;
        }
        
        else if (strcmp(token, ">") == 0 || strcmp(token, "<") == 0) {
            redirection = token[0];
            fileThere = true;
        }
        
        else if (fileThere) {
            strcpy(fileName, token);
            fileThere = false;
            
        }
        else {
            char *escape = malloc((strlen(token) * 2 + 1) * sizeof(char));
            int idx = 0;
            
            for (int i = 0 ; i < strlen(token)  ; i++) {
                if (token[i] == '-') {
                    //escape[idx++] = '\\';
                }
                escape[idx++] = token[i];
            }
            escape[idx] = '\0';
            
            words[count] = strdup(escape);
            count++;
            if (escape[0] == '-') {
                strcpy(option, escape);
            }
            else if (escape[0] != '-' && option != "" && i != 0){
                strcpy(input, escape);
            }
            
            
        }
        i++;
        token = strtok(NULL, " ");
    }
    
    *size = count;
    return words;
}

bool checkFileName(const char * line) {
    int len = strlen(line);
    if (len < 4) {
        return false;
    }
    
    const char *checkTxt = &line[len-4];
    return strcmp(checkTxt, ".txt") == 0;
}

void PrintStatements(char** words, int wordCount) {
    FILE *outputFile = fopen("parse.txt", "a");
            int i = 0;
    
            fprintf(outputFile, "----------\n");
            fprintf(outputFile, "Command: %s\n", words[0]);
            i++;
            int commandCount = 1;
            //printf("%d\n", wordCount);
            //printf("%d, total count: %d\n", commandCount, wordCount);
            //printf("%d,   %c\n", words[commandCount][0], words[commandCount][0]);
            
                fprintf(outputFile, "Inputs: %s\n", input);
                
                commandCount++;
            
            //printf("%d, total count: %d\n", commandCount, wordCount);
            if (commandCount < wordCount && checkFileName(words[commandCount])) {
                commandCount++;
            }
            
            
            fprintf(outputFile, "Options: %s\n", option);
            commandCount++;
                
                
            
            
                fprintf(outputFile, "Redirection: %c\n", redirection);
                commandCount++;
            
            //printf("%d, total count: %d\n", commandCount, wordCount);
            
            if (commandCount < wordCount && checkFileName(words[commandCount])) {
                commandCount++;
            }
            
            //printf("%d, total count: %d\n", commandCount, wordCount);
            
            if (ampersandCheck) {
                    fprintf(outputFile, "Background Job: y\n");
                    fprintf(outputFile, "----------\n");
            }
            
            if (!ampersandCheck) {
                fprintf(outputFile, "Background Job: n\n");
                fprintf(outputFile, "----------\n");
            }
            
            fflush(outputFile);
            fclose(outputFile);
                    
}

#define SIZE 1024

bool stop = false;
void *CommandRun(void * arg) {
    int fdRead = (long int)arg;
    FILE *fdFile;
    bool conti = true;
    char buffer[SIZE];
    
    //pthread_mutex_lock(&lock);
    while (conti) {
        pthread_mutex_lock(&lock);
        
        fdFile = fdopen(fdRead, "r");
        
        
        if (fgets(buffer, sizeof(buffer), fdFile) != NULL) {
            printf("---- %ld\n", pthread_self());
            printf("%s", buffer);
                
            fflush(stdout);
            while(fgets(buffer, sizeof(buffer), fdFile) != NULL) {
                printf("%s", buffer);
                fflush(stdout);
            }
            printf("---- %ld\n", pthread_self());
            conti = false;
                
            fflush(stdout);
                
        }
            //pthread_mutex_unlock(&lock);
            //fclose(fdFile);
        
        pthread_mutex_unlock(&lock);
        //fclose(fdFile);
    }
    //pthread_mutex_unlock(&lock);
    //fclose(fdFile);
    
    //close(fd_rw[1]);
    //pthread_mutex_unlock(&lock);
    
    //stop = false;
    
    
    //close(fd_rw[0]);
    //char buffer[SIZE];
    //ssize_t reads;
    //pthread_mutex_lock(&lock);
    //printf("---- %ld\n", pthread_self());
    //pthread_mutex_unlock(&lock);
    //pthread_mutex_lock(&lock);
    
    //stop = true;
    
    //pthread_mutex_unlock(&lock);
    //pthread_mutex_lock(&lock);
    //printf("---- %ld\n", pthread_self());
    //pthread_mutex_unlock(&lock);
    
    pthread_exit(NULL);
    
    
}


int main() {
    pthread_t threads[150] = {0};
    int threadCount = 0;
    
    FILE *file;
    char **words;
    int Max_Count = 1000;
    int Max_Line = 100;
    
    char line[Max_Count];
    char lines[Max_Line][Max_Count];
    char lineFinal[Max_Count];
    
    char processes[100];
    int processCount = 0;
    
    file = fopen("commands.txt", "r");
    int countLine = 0;
    if (file == NULL) {
        perror("Error opening the file");
        return EXIT_FAILURE;
    }
    
    while(fgets(line, sizeof(line), file) != NULL) {
        strcpy(lines[countLine], line);
        remove_newline(lines[countLine]);
        countLine++;
    }
    
    fclose(file);
    //stop= true;
    for (int i = 0 ; i < countLine ; i++) {
        strcpy(lineFinal, lines[i]);
        
        //printf("%s\n", lineFinal);//holds as ["ls −l &", "wc output1 . txt &"];
        
        int wordCount;
        words = ArrayParsed(lineFinal, &wordCount);
        
        //pthread_create(&threads[threadNum], NULL, CommandRun, lineFinal);
        //threadNum++;
            
        PrintStatements(words, wordCount);
        char *wordsArray[wordCount+1];
        for (int i = 0 ; i < wordCount ; i++) {
            wordsArray[i] = words[i];
        }
        wordsArray[wordCount] = NULL;
        //stop = true;
        //printf("%s\n", fileName);
        if (strcmp(wordsArray[0], "wait") == 0) {
            for (int i = 0 ; i < processCount ; i++) {
                waitpid(processes[i], NULL, 0);
            }
            
            for (int i = 0 ; i < threadCount ; i++) {
                pthread_join(threads[i], NULL);
            }
        }
        if (redirection == '>') {//printed on file
            int outRed = fork();
            
            if (outRed == 0) {
                int fileOutput = open(fileName, O_WRONLY | O_CREAT | O_TRUNC, S_IRWXU);
                
                dup2(fileOutput, STDOUT_FILENO);
                
                execvp(wordsArray[0], wordsArray);
            }
            else {
                if(!ampersandCheck) {
                    
                    wait(NULL);
                }
                else {
                    processes[processCount] = outRed;
                    processCount++;
                }
                //close(fd[1]);
                
            }
        }
        
            
        else {//using pipe for sysout file. redirection of < or nothing
            int fd[2];
            pipe(fd);
            
            //stop = false;
            
            
            int inputRc = fork();
            
            if (inputRc == 0) {
                //close(fd[0]);
                dup2(fd[1], STDOUT_FILENO);
                close(fd[1]);
                if (redirection == '<') {
                    int fileInput = open(fileName, O_RDONLY);
                    dup2(fileInput, STDIN_FILENO);
                }
                
                
                //const char* data = "Hello";

                //write(fd_rw[1], data, strlen(data));
                //waitpid(inputRc, NULL, 0);
                execvp(wordsArray[0], wordsArray);
            }
            else {
                //stop = true;
                //close(fd_rw[0]);
                
                //close(fd_rw[1]);
                //stop = true;
                //stop = false;
                //pthread_t listenerThread;
                //threads[threadCount++] = listenerThread;
                
                close(fd[1]);
                
                char buff[5];
        
                //read(fd_rw[0], buff, sizeof(buff));
                //printf("%s\n", buff);
                pthread_t th;
                
                pthread_create(&th, NULL, CommandRun, (void *)fd[0]);
                threadCount++;
                
                
                
                
                
                //stop = true;
                //pthread_create(&, NULL, CommandRun, NULL);
                
                //stop = true;
                //close(fd[0]);
                //close(fd[1]);
                //stop = true;
                
                if (!ampersandCheck) {
                    pthread_join(threads[threadCount], NULL);
                    waitpid(inputRc, NULL, NULL);
                    
                    //stop = true;
                    
                }
                else {
                    //stop = true;
                    processes[processCount] = inputRc;
                    processCount++;
                    
                    threads[threadCount] = th;
                    
                    
                }
                
                //stop = true;
            }
            //stop = true;
            //close(fd[0]);
            //stop = true;
            //close(fd[1]);
            //stop = true;
        }
        //stop = true;
        
        //wait(NULL);
    }
    
    
    for (int i = 0 ; i < processCount ; i++) {
        waitpid(processes[i], NULL, 0);
    
    }
            
    for (int i = 0 ; i < 15 ; i++) {
        //printf("%ld \n", threads[i]);
        pthread_join(threads[i], NULL);
        //wait(NULL);
    }
    
}

/*
ls -l > out1.txt &
man cat
grep rwx < out1.txt &
wait
*/

/*
grep danger < input1.txt &
ls -a &
wc hw2.c > output1.txt
grep clearly -i < input1.txt &
*/
