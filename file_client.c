#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <time.h>
#include <pthread.h>
#include <signal.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <semaphore.h>

pthread_mutex_t lock;
void *listenme(){
	pthread_mutex_lock(&lock);  // mutex locked prevent critical region
	int fd1;
	char * file_manager_named_pipe = "/tmp/file_manager_named_pipe"; // FIFO file path
	mkfifo(file_manager_named_pipe, 0666); // create named_pipe thanks to mkfifo
	char str1[80], str2[80];
	printf("Create  -> This command: If there is no file name in File_List, \n");  //Commands to be used in the program and their explanations.
	printf("it adds the file name to the empty line \n");
	printf("and creates the file in the system.\n");
	printf("Delete  -> This command: If there is a file name in File_List, \n");
	printf("it deletes the file from the system and File_List(index).\n");
	printf("Read 	-> This command: If there is a file name in File_List,\n");
	printf("it reads the line in the relevant index from the system \n");
	printf("and sends it to the relevant File_Client.\n");
	printf("Write   -> This command: If there is a file name in File_List, \n");
	printf("it writes the data from File_client to the file.\n");
	printf("Exit  	-> This command: Ends the corresponding file_client thread, \n");
	printf("communication is broken.\n");
	while (1){
		
		fd1 = open(file_manager_named_pipe,O_WRONLY);  // Open FIFO for write only
		fgets(str2, 80, stdin);				// make a listen command 
		write(fd1, str2, strlen(str2)+1);		// write a command in named_pipe file
		close(fd1);
		fd1 = open(file_manager_named_pipe,O_RDONLY); // Open FIFO for read only
		read(fd1, str1, 80);			      // take a response from file_manager	
		printf("User1: %s\n", str1);
		close(fd1);
		char *cp7 = "Exit\n";
		if (strcmp(str1, cp7) == 0){ 		//If exit command is received, the thread is terminated.
		printf("Successfully thread is finished...\n");
		break;
		} 
	}
	pthread_mutex_unlock(&lock);  // mutex unlocked prevent critical region
return NULL;

}
int main(int argc, char *argv[]) {
	pthread_t fct;
	void *status;  
	int t_suc2 = pthread_create(&fct, NULL,listenme,NULL); // create thread and give listenme function as a parameter
	if(t_suc2 != 0){
		perror("Thread is not create!!");
		exit(1);
	}
	int tj_suc2 = pthread_join(fct, &status);   // join thread
	if(tj_suc2 != 0){
		perror("Thread is not join!!");
		exit(1);
	}
	int pipes[10];			// create 5 pipe for communicate client server 
	for(int y = 0; y < 5; y++){
		if(pipe(pipes + y*2) < 0){
			perror("Error: pipe");
			exit(1);
		}
	}

    return 0;
}
