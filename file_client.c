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
int v = 0;
void *listenme(){
	pthread_mutex_lock(&lock);
	int fd1;
	char * myfifo = "/tmp/myfifo"; // FIFO file path
	mkfifo(myfifo, 0666);
	char str1[80], str2[80];
	printf("Create  -> This command: If there is no file name in File_List, \n");
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
		
		fd1 = open(myfifo,O_WRONLY);
		fgets(str2, 80, stdin);
		write(fd1, str2, strlen(str2)+1);
		close(fd1);
		fd1 = open(myfifo,O_RDONLY); // Open FIFO for read only
		read(fd1, str1, 80);
		printf("User1: %s\n", str1);
		close(fd1);
		char *cp7 = "Exit\n";
		if (strcmp(str1, cp7) == 0){ //exit 
		printf("Successfully thread is finished...\n");
		break;
		
		} 
	}
	pthread_mutex_lock(&lock);
return NULL;

}
int main(int argc, char *argv[]) {
	int threadCount = 5;
	pthread_t fcts[threadCount];
	void *status;
	if(pthread_mutex_init(&lock,NULL) != 0){
		printf("Error: Mutex init!");
		return 1;
	
	}
	for(int i = 0; i < threadCount; ++i){
		pthread_create(fcts+i, NULL,listenme,NULL);
	}
	for(int i = 0; i < threadCount; ++i){
		pthread_join(fcts[i], &status);
	}
	pthread_mutex_destroy(&lock);

    return 0;
}

