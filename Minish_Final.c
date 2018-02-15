/* Program to implement a shell minish
   Author: Sujit Kumar    BId: B00674954     mailId: skumar27@binghamton.edu 
   Date: 09/25/2017
*/
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <fcntl.h>
#include <sys/wait.h>
#include <signal.h>
#include <pwd.h>
#define _LENGTH_OF_INPUT 256
#define X NULL
static int listOfJobs[16][2];
static char *listOfCommands[16];
static int jobCounter=0;
static int statusOfProcess;
char *ptrPath;
char *ptrPathReturn;
long size;
//function definition to handle ctrl+c
void handle_sigkill_signal();
//function definition to list all the background processes
void listJobs(int listOfJobs[][2], char *listOfCommands[]); 

int main(int argc, char* argv[])
{
	//char choiceToContinue;
	for (int i=0; i<16; i++) {
		listOfJobs[i][0] = 0;
		listOfJobs[i][1] = 0;
	}
	
/* THIS IS START OF THE do-while loop that keeps minish alive after every new command is executed*/ 
do {
	char *inputBuffer;
    size_t len = 256;
    size_t sizeOfActualInput = 0;
	int status;
	//int counter;
	char *token;
	char splitInput[256] = " ";
	char concatInputCommand[16] = {};
	char *concatInputOption;
	//int exec_return_value;
	size_t tokenCounter= 1;
	char *xyzxyz[16];
	int backGroundProcessOption = 0;
	int changeParentProcessEnv = 0;
	struct sigaction actionSignal;
	actionSignal.sa_handler = &handle_sigkill_signal;
	/* structure sigaction defined insignal.h*/
	/*struct sigaction {
    void (*sa_handler)(int);
    void (*sa_sigaction)(int, siginfo_t *, void *);
    sigset_t sa_mask;
    int sa_flags;
    void (*sa_restorer)(void);
     }	*/
		//
	if (!(inputBuffer = malloc(_LENGTH_OF_INPUT * sizeof(char *))))
        return 1;
	do {		
		//printf ("\nplease enter the command you want this code to execute\n");
		printf("minish>");
		sizeOfActualInput = getline(&inputBuffer, &len, stdin);
		if (sizeOfActualInput >= _LENGTH_OF_INPUT) {
		printf ("The maximum length of the input command should be %d, you have etered %zu characters", _LENGTH_OF_INPUT, sizeOfActualInput);
		if (!(inputBuffer = realloc(inputBuffer, (2*_LENGTH_OF_INPUT))))
			return 1;
		} 
	   
	} while(inputBuffer[0] == '\n'); 
		

	
	if (!(concatInputOption = malloc(256 * sizeof(char))))
      	return 1;
	strcpy (splitInput, inputBuffer);
	splitInput[strcspn(splitInput, "\r\n")]=0;
	token = strtok(splitInput," ");
	//printf ("length of tokens is %zu\n", strlen(token));
	//printf ("COMMAND IS IN TOKEN %s\n", token);
			
	while (token != NULL) {
		xyzxyz[tokenCounter-1]= (char*)malloc(16 * sizeof(char *));
		if (tokenCounter == 1) {
			if (token[0] == '&') {
				printf ("\nYou have entered %s as command, please enter a valid command\n", token);
			} else {			
				strcpy (concatInputCommand, token);
				strcpy (xyzxyz[tokenCounter-1], token);
				//printf ("\nxyzxyz[tokenCounter-1] is: %s, tokenCounter is %zu\n", xyzxyz[tokenCounter-1], tokenCounter);
			}
		} else {
			if (token[0] == '&') {
				printf ("\nYou have requested the command to be run in background\n");
				backGroundProcessOption = 1;
			} else {
				strcat (concatInputOption, token);
				strcat (concatInputOption, " ");
				strcpy (xyzxyz[tokenCounter-1], token);
				//xyzxyz[tokenCounter-1][strcspn(xyzxyz[tokenCounter-1], " ")]=0;
				//printf ("xyzxyz[tokenCounter-1] is: %s, tokenCounter is %zu\n", xyzxyz[tokenCounter-1], tokenCounter);
			}
		}	
			tokenCounter++;
			token = strtok(NULL, " ");
    }
	//printf("SETTING THE FLAG TO DISTINGUISH SHELL BUILTIN\n");
	if ((strcmp(concatInputCommand, "joblist")==0) || (strcmp(concatInputCommand, "cd")==0) || (strcmp(concatInputCommand, "pwd")==0) || (strcmp(concatInputCommand, "fg")==0) || (strcmp(concatInputCommand, "exit")==0)) {	 
			changeParentProcessEnv = 1;
	}
	//printf("THE FLAG is %d", changeParentProcessEnv);
    if (changeParentProcessEnv == 0) {
        int rc = fork();
        if (rc < 0) {          // fork failed; exit
        fprintf(stderr, "fork failed\n");
        exit(1);
        } else if (rc == 0) { 
            //########################################
            //printf("\nChild process: pid=%d pgid=%d\n", getpid(), getpgrp());
            if(setpgrp() == 0){
                //printf("\nChild process after setting PGid: pid=%d pgid=%d\n", getpid(), getpgrp());
            }
            if (backGroundProcessOption == 0) {
                xyzxyz[tokenCounter-1]= (char*)malloc(16 * sizeof(char *));
                xyzxyz[tokenCounter-1] = (char*) NULL;
            } else {
                xyzxyz[tokenCounter-2]= (char*)malloc(16 * sizeof(char *));
                xyzxyz[tokenCounter-2] = (char*) NULL;		
            }	
            
            //if (changeParentProcessEnv == 0) {	
                concatInputOption[strcspn(concatInputOption, " ")]=0;
                //printf("Command to be run is %s\n", concatInputCommand);
                execvp(concatInputCommand, xyzxyz);
            //}
            if (sigaction(SIGKILL, &actionSignal, NULL) == -1) {
                    printf("\n\nSIGKILL cannot be handled, it just exits immediately...\n");
                }
        } else {              
                /* Here begins the parent process*/	
                /* THIS IS START OF THE do-while loop that keeps parent alive tillthe child completes */
                do {
                //printf ("\nI am shouting from parent code, process id is: %d\n", rc);
                //printf("\nParent process:   pid=%d pgid=%d\n", getpid(), getpgrp());
                    if (backGroundProcessOption == 1) {		
                        statusOfProcess = waitpid(rc, &status, WNOHANG);
                    } 
                    if (backGroundProcessOption == 0){
                        statusOfProcess= waitpid(rc, &status, WUNTRACED | WCONTINUED);
                    }
                    //if (changeParentProcessEnv == 0) {
                        
                        if (statusOfProcess== -1) {
                            perror("waitpid");
                            exit(EXIT_FAILURE);
                        }

                       if (WIFEXITED(status)) {
                            //printf("exited, status=%d\n", WEXITSTATUS(status));
                        } else if (WIFSIGNALED(status)) {
                            //printf("killed by signal %d\n", WTERMSIG(status));
                        } else if (WIFSTOPPED(status)) {
                            //printf("stopped by signal %d\n", WSTOPSIG(status));
                        } else if (WIFCONTINUED(status)) {
                            //printf("continued\n");
                        }
                    //}
                } while (!WIFEXITED(status) && !WIFSIGNALED(status));
                //printf("\n###backGroundProcessOption is %d\n", backGroundProcessOption);
                if (backGroundProcessOption == 1) {
                    listOfJobs[jobCounter][0] = rc;
                    listOfJobs[jobCounter][1] = statusOfProcess;
                    listOfCommands[jobCounter] = (char *)malloc(256 * sizeof(char));
                    strcpy(listOfCommands[jobCounter], concatInputCommand);
                    jobCounter++;
                    //#######################making the list of jobs contiguous###################
                    for (int i=1; i<16; i++) {
                        if(listOfJobs[i-1][0] == 0) {
                            listOfJobs[i-1][0]=listOfJobs[i][0];
                            listOfJobs[i-1][1]=listOfJobs[i][1];
                            listOfCommands[i-1] = listOfCommands[i];
                            listOfJobs[i][0]= 0;
                            listOfJobs[i][1]= 0;
                        }
                    }
                }
                //printf ("\nI am executing in parent: changeParentProcessEnv is %d", changeParentProcessEnv);
                /* THIS IS END OF THE do-while loop that keeps parent alive till the child completes */
            } 
        }
        if (changeParentProcessEnv == 1) {
            if (strcmp(concatInputCommand, "joblist")==0) {
                //printf ("\nI am executing joblist command:  \n");
                listJobs(listOfJobs, listOfCommands);
            } else if (strcmp(concatInputCommand, "cd")==0) {
                //printf ("\nI am executing cd command:    \n");
		char *homedir = getenv("HOME");
		//printf("home dir is %s, xyzxyz[1] is %s", homedir, xyzxyz[1]);
		    if (strcmp(xyzxyz[1],"cd")==0) {
			chdir(homedir);
		    } else {
                    	chdir(xyzxyz[1]); 
		    }
            } else if (strcmp(concatInputCommand, "pwd")==0) {
                //printf ("\nI am executing pwd command:   \n");
		//getting the path variable from env
                size = pathconf(".", _PC_PATH_MAX);
                if ((ptrPath = (char *)malloc((size_t)size)) != NULL)
                    ptrPathReturn = getcwd(ptrPath, (size_t)size);
                printf ("\nPresent Working Directory is:      %s    \n", ptrPathReturn);
                free(ptrPath);
            } else if(strcmp(concatInputCommand, "fg")==0) {
                printf ("\nProcess is being put in foreground now, pID is: %d\n", atoi(xyzxyz[1]));
		do {
			statusOfProcess= waitpid(atoi(xyzxyz[1]), &status, WUNTRACED | WCONTINUED);
		}while (!WIFEXITED(status) && !WIFSIGNALED(status));
		//Removing the foregrounded process from teh background process list
		for (int i=0; i<16; i++) {
			if(listOfJobs[i][0] == atoi(xyzxyz[1])) {
				listOfJobs[i][0]=0;
			}
		}
		//making the background process list contiguous
		for (int i=1; i<16; i++) {
                	if(listOfJobs[i-1][0] == 0) {
                    		listOfJobs[i-1][0]=listOfJobs[i][0];
                    		listOfJobs[i-1][1]=listOfJobs[i][1];
                    		listOfCommands[i-1] = listOfCommands[i];
                    		listOfJobs[i][0]= 0;
                    		listOfJobs[i][1]= 0;
                	}
            	}
            } else if(strcmp(concatInputCommand, "exit")==0) {
		printf("\n The user has issued EXIT command, all the background processes will be killed and shell will exit\n");	
		//killing all the background processes before proceeding to kill the shell	
		for (int i=0; i<16; i++) {
			if(listOfJobs[i][0] != 0) {
				kill(listOfJobs[i][0], SIGKILL);
				listOfJobs[i][0]=0;
			}
		 }
		////killing the shell 
			exit(EXIT_SUCCESS);
       	     	}
	}
        backGroundProcessOption = 0;
	for (int i=0;i<tokenCounter-1;i++) {
		free(xyzxyz[i]);
	}
	
	free(inputBuffer);
	free(concatInputOption);
    }while (1); 
	for (int i=0;i<jobCounter-1;i++) {
	free(listOfCommands[i]);
	}
/* THIS IS END OF THE do-while loop that keeps minish alive after each command is executed*/		
return 0;	
}

//function definition to handle ctrl+c
void handle_sigkill_signal() {
	    printf("Caught SIGKILL, exiting now\n");
            exit(0);
}

//function definition to list all the background processes
void listJobs(int listOfJobs[][2], char *listOfCommands[]) {
printf ("\nList of backgrounded processes:\n");
	for (int i=0; i<16; i++) {
		
		if(listOfJobs[i][0] != 0) {
			char *statusInterpret;
			statusInterpret = malloc(_LENGTH_OF_INPUT * sizeof(char *));
			if (listOfJobs[i][1] == 0) {
                         	strcpy(statusInterpret, "RUNNING");
                        } else if(listOfJobs[i][1] == listOfJobs[i][0]) {
				strcpy(statusInterpret, "FINISHED");
			} else {
				strcpy(statusInterpret, "ERROR STATE");
			}
			
			printf("\nCommand %s with PID %d Status:%s \n", listOfCommands[i], listOfJobs[i][0] , statusInterpret);
                        free(statusInterpret);
		}
				
	}
}
  

