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

printf("Thread is running succesfully.\n");
return NULL;

}

int main(int argc, char *argv[]) {
	char File_List[20][20];
	int flidx;
	int flidx2 = 0;
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
	int fd;
    	char * myfifo = "/tmp/myfifo"; // FIFO file path
    	mkfifo(myfifo, 0666);
    	char arr1[80];
    	char *arr2;
    	char *cp7 = "Exit\n";
 	char *cp1 = "Create";
 	char *cp2 = "Delete";
    	while (1) {
		fd = open(myfifo, O_RDONLY);
		read(fd, arr1, sizeof(arr1));
		printf("User2: %s\n", arr1);
		close(fd);
		char tmp[10][10]; //A temporary matrix array is created.
     			int i,j,count;
    			j=0; count=0;
    			for(i=0; i <= (strlen(arr1)); i++) {  //All entries entered by the user after myshell 
        			if(arr1[i] == ' '|| arr1[i] == '\0'){ //are separated by spaces and stored in a temporary array.
            				tmp[count][j]='\0';
            				count++;  
            				j=0;  
        			}else {	
            				tmp[count][j]=arr1[i];
            				j++;
        			}
        		}
        	char *cp = tmp[0]; // Values were stored for comparison.
       		char *cp0 = tmp[1]; // Filename
		char *cp7 = "Exit\n";
		if (strcmp(arr1, cp7) == 0){ //exit 
			arr2= "Exit\n";
			fd = open(myfifo, O_WRONLY); // Open FIFO for write only
			//fgets(arr2, 80, stdin);
			write(fd, arr2, strlen(arr2)+1);
			close(fd);	
		} else if (strcmp(cp, cp1) == 0){
			for(flidx = 0; flidx < 20; flidx++) { 
				if(strcmp(File_List[flidx] , cp0) == 0){
					arr2 = "This file already exists in the File_list.";
					fd = open(myfifo, O_WRONLY); // Open FIFO for write only
					//fgets(arr2, 80, stdin);
					write(fd, arr2, strlen(arr2)+1);
					close(fd);
					break;
				}
			}
			if(flidx == 20){
				strcpy(File_List[flidx2],cp0);
				flidx2++;
				FILE *fp;
				char* fName = cp0; 
				if ((fp = fopen(fName, "a+"))) {   // File is opened and check if there is a file with "a+". 
	  				fclose(fp);  // File is closed.
  				} else{
      					printf("Error: File is not open!\n");
      					exit(1);
  				}
	       			arr2 = "The file name is added to the file_list and the file is created in the system.";
				fd = open(myfifo, O_WRONLY); // Open FIFO for write only
				//fgets(arr2, 80, stdin);
				write(fd, arr2, strlen(arr2)+1);
				close(fd);
			}
			
		} else if (strcmp(cp, cp2) == 0){
			for(flidx = 0; flidx < 20; flidx++) { 
				if(strcmp(File_List[flidx] , cp0) == 0){
					strcpy(File_List[flidx]," ");
					char* fName = cp0; 
					int result = remove(fName) ;
	  				arr2 = "The file name is removed from file_list and the file is deleted in the system.";
					fd = open(myfifo, O_WRONLY); // Open FIFO for write only
					//fgets(arr2, 80, stdin);
					write(fd, arr2, strlen(arr2)+1);
					close(fd);
					break;
  					
				}
			}
			if(flidx == 20){
	       			arr2 = "The file to be deleted was not found.";
				fd = open(myfifo, O_WRONLY); // Open FIFO for write only
				//fgets(arr2, 80, stdin);
				write(fd, arr2, strlen(arr2)+1);
				close(fd);
			}
		}else {
			arr2 = "Wrong command entered! Enter the correct command.\n";
			fd = open(myfifo, O_WRONLY); // Open FIFO for write only
			//fgets(arr2, 80, stdin);
			write(fd, arr2, strlen(arr2)+1);
			close(fd);
		}
		
    	}
    return 0;
    
}
