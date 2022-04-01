#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>

#define MAX_LINE 80 // The maximum length of a command 
#define BUFFER_SIZE 50
#define buffer "\n\Shell Command History:\n"
/*Enter command  'history' for history feature and CTRL - c to exit the 'osh>' shell */
//declarations
char history[10][BUFFER_SIZE]; // array to store history commands
int count = 0;

// function to display the history of commands
void displayHistory(){ 
    printf("Shell command history:\n");
    
    int i;
    int j = 0;
    int histCount = count;
    
    // loop for iterating through commands
    for (i = 0; i<10;i++){
        // command index
        printf("%d.  ", histCount);
        while (history[i][j] != '\n' && history[i][j] != '\0'){	
		// printing command
            printf("%c", history[i][j]);
            j++;
        }
        printf("\n");
        j = 0;
        histCount--;
        if (histCount ==  0)
            break;
    }
    printf("\n");
} 

// Fuction to get the command from shell, tokenize it and set the args parameter
int formatCommand(char inputBuffer[], char *args[],int *flag){
   	int length; // # of chars in command line
    int i;     // loop index for inputBuffer
    int start;  // index of beginning of next command
    int ct = 0; // index of where to place the next parameter into args[]

 	length = read(STDIN_FILENO, inputBuffer, MAX_LINE);	
   
    start = -1;
    if (length == 0)
        exit(0);   // end of command
    if (length < 0){
        printf("Command not read\n");
        exit(-1);  // terminate
    }
    
   // examine each character
    for (i=0;i<length;i++){
        switch (inputBuffer[i]){
            case ' ':
            case '\t' :               // to seperate arguments
                if(start != -1){
                    args[ct] = &inputBuffer[start];    
                    ct++;
                }
                inputBuffer[i] = '\0'; // add a null char at the end
                start = -1;
                break;
                
            case '\n':                 // final char 
                if (start != -1){
                    args[ct] = &inputBuffer[start];
                    ct++;
                }
                inputBuffer[i] = '\0';
                args[ct] = NULL; // no more args
                break;
                
            default :           
                if (start == -1)
                    start = i;
                if (inputBuffer[i] == '&'){
                    *flag  = 1; // the flag is to differentiate whether the child process is invoked in background
                    inputBuffer[i] = '\0';
                }
        }
    }  
    args[ct] = NULL; // if the input line was > 80
	if(strcmp(args[0], "exit")==0){
		exit(-1);
	}
	if(strcmp(args[0],"history")==0){		
        if(count>0)
            displayHistory();
		else
			printf("No commands in history\n");
		return -1;
    }
	else if (args[0][0]-'!' ==0){	
		int x = args[0][1]- '0'; 
		int y = args[0][2]- '0'; 
		
		if(x>count){ //second letter check	
		printf("No such command in history\n");
		} 
		else{
			if(x==-15){ //Checking for '!!',ascii value of '!' is 33.
				if(count>0)
					strcpy(inputBuffer,history[0]);  // the last command
				else
					printf("No commands in history\n");
			}else if(x==0){ //Check for '!0'
				printf("No such command in history\n");
			}else if(x>=1){ //Check for '!n', n >=1
				if(y==0)
					strcpy(inputBuffer,history[0]); //!10
				else			
					strcpy(inputBuffer,history[count-x]);
			}			
		}
	}
 	for (i = 9;i>0; i--) //Moving the history elements one step higher
       	strcpy(history[i], history[i-1]);
		       
    strcpy(history[0],inputBuffer); //Updating the history array with input buffer
    count++;
	if(count>10){
		count=10;
	}
}

int main(void){
    char inputBuffer[MAX_LINE]; // buffer to hold the input command
    int flag; // equals 1 if a command is followed by "&"
    char *args[MAX_LINE/2 + 1]; // max arguments
    int should_run =1;
    
    pid_t pid,tpid;
    int i;   
    
    while (should_run){ // infinite loop for shell prompt               
        flag = 0; // flag =0 by default
        printf("osh>");
        fflush(stdout);
        if(-1!=formatCommand(inputBuffer,args,&flag)){ // get next command  	
		pid = fork();
        
        	if (pid < 0){ // if pid is less than 0, forking fails       	            
            		printf("Fork failed.\n");
            		exit (1);
        	}        
       		 else if (pid == 0){        	            
           	 	// command not executed
            	if (execvp(args[0], args) == -1){			
                	printf("Error executing command\n");
            	}
       		 }        
       		 // if flag == 0, the parent will wait,
        	// otherwise returns to the formatCommand() function.
        	else{        	
            	i++;
           	 	if (flag == 0){           		 
                	i++;
                	wait(NULL);
           		}
        	}
   	 	}
    }
}
