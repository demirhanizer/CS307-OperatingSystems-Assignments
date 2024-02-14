#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <fcntl.h>
#include <assert.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <math.h>

int main() {
//man wget | grep -A 4 -w -e '-v' 
	//int par = (int) getpid();
	printf("I’m SHELL process, with PID: %d - Main command is: man wget | grep '-v' -A 4 -w -e \n", (int)getpid());
	
	int rc, rc2;
	int fd[2];
	int fd_rw[2];
	pipe(fd_rw);
	pipe(fd);
	
	rc = fork();
	if (rc < 0) {
		printf("Error in fork process\n");
	}
	else if(rc == 0) {//child process for 
		
		
		printf("I’m MAN process, with PID: %d - My command is: man wget\n", (int)getpid());
		
		const char* data = "Hello";
		write(fd_rw[1], data, strlen(data));
		
		//close(fd[1]);
		
		dup2(fd[1], STDOUT_FILENO);
		close(fd[1]);
		close(fd[0]);
		
		char *manArgs[3];
		
		manArgs[0] = strdup("man");
		manArgs[1] = strdup("wget");
		manArgs[2] = NULL;
		
		execvp(manArgs[0], manArgs);
		
	}
	
	else {
		rc2 = fork();
		
		if (rc2 == 0) {
		
			char buff[64];
			read(fd_rw[0], buff, sizeof(buff));
			
			printf("I’m GREP process, with PID: %d - My command is: grep '-v' -A 4 -w -e\n", (int)getpid());
			
			dup2(fd[0], STDIN_FILENO);
			close(fd[1]);
			close(fd[0]);
		
			char *grepArgs[7];
			grepArgs[0] = strdup("grep");
			grepArgs[1] = strdup("-A");
			grepArgs[2] = strdup("4");
			grepArgs[3] = strdup("-w");
			grepArgs[4] = strdup("-e");
			grepArgs[5] = strdup("-v");
			
			grepArgs[6] = NULL;
			
			int output = open("output.txt", O_WRONLY | O_CREAT | O_TRUNC, S_IRWXU);
			dup2(output, STDOUT_FILENO);
			
			
			//waitpid(par, NULL, 0);	
			execvp(grepArgs[0], grepArgs);	
			
				
		}

	
	}
	
	close(fd[0]);
	close(fd[1]);
	waitpid(rc, NULL, 0);
	waitpid(rc2, NULL, 0);
	
	
		
	//FILE *output = fopen("outputa.txt","w");
	//fprintf(output, "hello");
	//char strStore[5000];
	//ssize_t n;
	
	//fclose(output);
	
	
	
	printf("I’m SHELL process, with PID:%d - execution is completed, you can find the results in output.txt\n", (int)getpid());
	
	return 0;
}
/*
int main() {
//man wget | grep -A 4 -w -e '-v' 
	//int par = (int) getpid();
	printf("I’m SHELL process, with PID: %d - Main command is: man wget | grep '-v' -A 4 -w -e \n", (int)getpid());
	
	int rc, rc2;
	int fd[2];
	pipe(fd);
	
	rc = fork();
	if (rc < 0) {
		printf("Error in fork process\n");
	}
	else if(rc == 0) {//child process for 
		
		
		printf("I’m MAN process, with PID: %d - My command is: man wget\n", (int)getpid());
		
		const char* data = "Hello";
		write(fd[1], data, strlen(data));
		
		//close(fd[1]);
		
		dup2(fd[1], STDOUT_FILENO);
		close(fd[1]);
		close(fd[0]);
		
		char *manArgs[3];
		
		manArgs[0] = strdup("man");
		manArgs[1] = strdup("wget");
		manArgs[2] = NULL;
		
		execvp(manArgs[0], manArgs);
		
	}
	
	else {
		rc2 = fork();
		
		if (rc2 == 0) {
		
			char buff[64];
			read(fd[0], buff, sizeof(buff));
			
			printf("I’m GREP process, with PID: %d - My command is: grep '-v' -A 4 -w -e\n", (int)getpid());
			
			dup2(fd[0], STDIN_FILENO);
			close(fd[1]);
			close(fd[0]);
		
			char *grepArgs[7];
			grepArgs[0] = strdup("grep");
			grepArgs[1] = strdup("-A");
			grepArgs[2] = strdup("4");
			grepArgs[3] = strdup("-w");
			grepArgs[4] = strdup("-e");
			grepArgs[5] = strdup("-v");
			
			grepArgs[6] = NULL;
			
			int output = open("output.txt", O_WRONLY | O_CREAT | O_TRUNC, S_IRWXU);
			dup2(output, STDOUT_FILENO);
			
			
			//waitpid(par, NULL, 0);	
			execvp(grepArgs[0], grepArgs);	
			
				
		}

	
	}
	
	close(fd[0]);
	close(fd[1]);
	waitpid(rc, NULL, 0);
	waitpid(rc2, NULL, 0);
	
	
		
	//FILE *output = fopen("outputa.txt","w");
	//fprintf(output, "hello");
	//char strStore[5000];
	//ssize_t n;
	
	//fclose(output);
	
	
	
	printf("I’m SHELL process, with PID:%d - execution is completed, you can find the results in output.txt\n", (int)getpid());
	
	return 0;
}
*/
/*
int main() {
//man wget | grep -A 4 -w -e '-v' 
	//int par = (int) getpid();
	printf("I’m SHELL process, with PID: %d - Main command is: wget\n", (int)getpid());
	
	int rc, rc2;
	int fd[2];
	pipe(fd);
	
	rc = fork();
	if(rc == 0) {//child process for 
		printf("I’m MAN process, with PID: %d - My command is: wget\n", (int)getpid());
		dup2(fd[1], STDOUT_FILENO);
		close(fd[1]);
		close(fd[0]);
		
		char *manArgs[3];
		
		manArgs[0] = strdup("man");
		manArgs[1] = strdup("wget");
		manArgs[2] = NULL;
		
		execvp(manArgs[0], manArgs);
		
	}
	
	rc2 = fork();
	if (rc2 == 0) {
		
		
		printf("I’m GREP process, with PID: %d - My command is: wget\n", (int)getpid());
		dup2(fd[0], STDIN_FILENO);
		close(fd[1]);
		close(fd[0]);
		
		char *grepArgs[7];
		grepArgs[0] = strdup("grep");
		grepArgs[1] = strdup("-A");
		grepArgs[2] = strdup("4");
		grepArgs[3] = strdup("-w");
		grepArgs[4] = strdup("-e");
		grepArgs[5] = strdup("-v");
		grepArgs[6] = NULL;
			
		int output = open("output.txt", O_WRONLY | O_CREAT | O_TRUNC, S_IRWXU);
		dup2(output, STDOUT_FILENO);
			
		//waitpid(par, NULL, 0);	
		execvp(grepArgs[0], grepArgs);
	
	}
	
	close(fd[0]);
	close(fd[1]);
	waitpid(rc, NULL, 0);
	waitpid(rc2, NULL, 0);
	
	
		
	//FILE *output = fopen("outputa.txt","w");
	//fprintf(output, "hello");
	//char strStore[5000];
	//ssize_t n;
	
	//fclose(output);
	
	
	
	printf("I’m SHELL process, with PID:%d - execution is completed, you can find the results in output.txt\n", (int)getpid());
	
	return 0;
}
*/

