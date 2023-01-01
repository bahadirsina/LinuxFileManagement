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

void *listenme(){

	int fd;
    	char * myfifo = "/tmp/myfifo"; // FIFO file path
    	mkfifo(myfifo, 0666);
    	char arr1[80], arr2[80];
    	
    	while (1) {
		fd = open(myfifo, O_RDONLY);
		read(fd, arr1, sizeof(arr1));
		printf("User2: %s\n", arr1);
		close(fd);
		fd = open(myfifo, O_WRONLY); // Open FIFO for write only
		fgets(arr2, 80, stdin);
		write(fd, arr2, strlen(arr2)+1);
		close(fd);
    	}
return NULL;

}

int main(int argc, char *argv[]) {
	char File_List[20][20];
	pthread_t fmt;
	void *status;
	int t_suc = pthread_create(&fmt, NULL,listenme,NULL);
	if(t_suc != 0){
		perror("Thread is not create!!");
		exit(1);
	}
	int tj_suc = pthread_join(fmt, &status);
	if(tj_suc != 0){
		perror("Thread is not join!!");
		exit(1);
	}

    return 0;
    
}
