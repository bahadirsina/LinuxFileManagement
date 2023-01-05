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

pthread_mutex_t lock;

int strcompare(char *s1,char *s2){  //  A function I created that correctly compares two strings
	int i,c = 0;
	if((strlen(s1)-1) == (strlen(s2))){
	
		for(i = 0; s2[i]!='\0'; i++){
			if(s1[i] == s2[i]){
				c++;
			}
		}
		if(c == i){
			return 0;
		}
	}
return -1;
}
void *listenme(){
	pthread_mutex_lock(&lock); // mutex locked prevent critical region
char File_List[20][20];  //create file_list for system file_names
	int flidx;
	int flidx2 = 0;
	int fd;
    	char * file_manager_named_pipe = "/tmp/file_manager_named_pipe"; // FIFO file path
    	mkfifo(file_manager_named_pipe, 0666); // create named_pipe thanks to mkfifo
    	char arr1[80];
    	char *arr2;
    	char arr3[80];
    	char arr4[80];
    	char *cp7 = "Exit\n"; //For compare string commands
 	char *cp1 = "Create";
 	char *cp2 = "Delete";
 	char *cp3 = "Read";
 	char *cp4 = "Write";
    	while (1) {
		fd = open(file_manager_named_pipe, O_RDONLY); // Open FIFO for read only
		read(fd, arr1, sizeof(arr1)); // take a response from file_manager
		printf("User2: %s\n", arr1);
		close(fd);
		char tmp[10][10]; //A temporary matrix array is created.
     			int i,j,count;
    			j=0; count=0;
    			for(i=0; i <= (strlen(arr1)); i++) {  //All entries entered console then entries
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
        	//char tarr[20];
        	//tarr[20] = tmp[1];
        	//strcat(tarr,"\0");
       		char *cp0 = tmp[1]; // Filename
       		char *cpp1 = tmp[2];  // Data for write file
		char *cp7 = "Exit\n";
		if (strcmp(arr1, cp7) == 0){ //exit 
			arr2= "Exit\n";
			fd = open(file_manager_named_pipe, O_WRONLY); // Open FIFO for write only
			//fgets(arr2, 80, stdin);
			write(fd, arr2, strlen(arr2)+1);
			close(fd);	
		} 

		else if (strcmp(cp, cp1) == 0){ // Compare commands
			for(flidx = 0; flidx < 20; flidx++) { 
				if(strcmp(File_List[flidx] , cp0) == 0){// search file_name in File_list 
					arr2 = "This file already exists in the File_list."; // Response to file_client
					fd = open(file_manager_named_pipe, O_WRONLY); // Open FIFO for write only
					//fgets(arr2, 80, stdin);
					write(fd, arr2, strlen(arr2)+1); // write a response in named_pipe file
					close(fd);
					break;
				}
			}
			if(flidx == 20){
				strcpy(File_List[flidx2],cp0); // Add file_name to File_list
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
				fd = open(file_manager_named_pipe, O_WRONLY); // Open FIFO for write only
				//fgets(arr2, 80, stdin);
				write(fd, arr2, strlen(arr2)+1); // write a response in named_pipe file
				close(fd);
			}
		} 
		else if (strcmp(cp, cp2) == 0){  // Compare commands
			for(flidx = 0; flidx < 20; flidx++) { 
				if(strcmp(File_List[flidx] , cp0) == 0){ // search file_name in File_list 
					strcpy(File_List[flidx]," "); // remove file_name in File_list
					char* fName = cp0; 
					int result = remove(fName) ; // delete files in system
	  				arr2 = "The file name is removed from file_list and the file is deleted in the system.";
					fd = open(file_manager_named_pipe, O_WRONLY); // Open FIFO for write only
					//fgets(arr2, 80, stdin);
					write(fd, arr2, strlen(arr2)+1); // write a response in named_pipe file
					close(fd);
					break;
  					
				}
			}
			if(flidx == 20){
	       			arr2 = "The file to be deleted was not found.";
				fd = open(file_manager_named_pipe, O_WRONLY); // Open FIFO for write only
				//fgets(arr2, 80, stdin);
				write(fd, arr2, strlen(arr2)+1); // write a response in named_pipe file
				close(fd);
			}
		}else if (strcmp(cp, cp3) == 0){  // Compare commands
			for(flidx = 0; flidx < 20; flidx++) { 
				if(strcmp(File_List[flidx] , cp0) == 0){ // search file_name in File_list 
					FILE *fp;
					char* fName = cp0; 
					strcpy(arr4," "); // provide empty array for read a file
					strcpy(arr3," "); //provide empty array for read a file
					if ((fp = fopen(fName, "r"))) { // File is opened and check if there is a file with "r". 
						while(!feof(fp)){  // All lines in the file traversed.
							strcat(arr4,arr3); // array içinde okunan data tutuldu.
							fgets(arr3,80,fp); // file okundu
							
						}
	  					fclose(fp);  // File is closed.
  					} else{
      						printf("Error: File is not open!\n");
      						exit(1);
  					}
					fd = open(file_manager_named_pipe, O_WRONLY); // Open FIFO for write only
					//fgets(arr2, 80, stdin);
					write(fd, arr4, strlen(arr4)+1); // write a response in named_pipe file
					close(fd);
					break;
				}
			}
			if(flidx == 20){
	       			arr2 = "The requested file was not found.";
				fd = open(file_manager_named_pipe, O_WRONLY); // Open FIFO for write only
				//fgets(arr2, 80, stdin);
				write(fd, arr2, strlen(arr2)+1); // write a response in named_pipe file
				close(fd);
			}
		} else if (strcmp(cp, cp4) == 0){  // Compare commands
			for(flidx = 0; flidx < 20; flidx++) { 
			       	//strcat(cp0,"\0");
			       	//printf("%s", cp0);
			       	//printf("%s",File_List[flidx]);
				if(strcompare(File_List[flidx] , cp0) == 0){ // search file_name in File_list 
					FILE *fp;
					char* fName = File_List[flidx] ; 
					char* rDatas = cpp1;
					if ((fp = fopen(fName, "a"))) { // File is opened and check if there is a file with "a". 
						fprintf(fp,"%s",rDatas);  // Write data inside file.
	  					fclose(fp);  // File is closed.
  					} else{
      						printf("Error: File is not open!\n");
      						exit(1);
  					}
  					arr2 = "The entered data is written to the file.";
					fd = open(file_manager_named_pipe, O_WRONLY); // Open FIFO for write only
					//fgets(arr2, 80, stdin);
					write(fd, arr2, strlen(arr2)+1); // write a response in named_pipe file
					close(fd);
					break;
				}
			}
			if(flidx == 20){
				
	       			arr2 = "The requested file was not found.";
				fd = open(file_manager_named_pipe, O_WRONLY); // Open FIFO for write only
				//fgets(arr2, 80, stdin);
				write(fd, arr2, strlen(arr2)+1); // write a response in named_pipe file
				close(fd);
			}
		} else {
			arr2 = "Wrong command entered! Enter the correct command.\n";
			fd = open(file_manager_named_pipe, O_WRONLY); // Open FIFO for write only
			//fgets(arr2, 80, stdin);
			write(fd, arr2, strlen(arr2)+1); // write a response in named_pipe file
			close(fd);
		}
		
		
    	}
    	pthread_mutex_unlock(&lock); // mutex unlocked prevent critical region
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
		pthread_create(fcts+i, NULL,listenme,NULL); // Create 5 thread prevent critical region.
	}
	for(int i = 0; i < threadCount; ++i){
		pthread_join(fcts[i], &status); // Join 5 thread prevent critical region.
	}
	pthread_mutex_destroy(&lock);
    return 0;
    
}
