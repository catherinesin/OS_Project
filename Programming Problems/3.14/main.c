#include<sys/types.h>
#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<sys/wait.h>

int collatz(int n){
	if(n%2==0)
		return n/2;
	else
		return 3*n+1;
}

int main(int argc, char **argv){
	int n;
	pid_t pid;

	if (argc > 1)
    {
        //sscanf(argv[1], "%d", &n);
        n = atoi(argv[1]);
        if (n < 1){
            printf("Invalid input: %d, positive integer required\n", n);
            return 0;
        }
    }else{
        return 1;
    }
	
	// fork a children process
	pid = fork();
	
	if(pid<0){ // error encountered
		fprintf(stderr, "Fork Failed");
		return 1;
	}else if(pid == 0){ // child process
		while(n!=1){
			printf("%d\t", n);
			n = collatz(n);
		}
		printf("1\n");	
		printf("Child process is done\n");	
	}else{ // parent process if(pid>0)
		wait(NULL);
		printf("Parent process is done\n");
	}
	return 0;
}
