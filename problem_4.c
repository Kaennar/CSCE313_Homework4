// Title :: Problem 4 Implementation
// Author:: Nicholas Hemstreet


#include <dirent.h>
#include <sys/types.h>
#include <sys/param.h>
#include <sys/ioctl.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>

// Global variables
int fd[2], nbytes;

int search(char* s_dir,int level);


// fd[0] == read from this channel
// fd[1] == write to this channel


int main(int argc, char** argv){
  // Setup the file descriptors and output to 
  // We exit if we don't get a directory name
  char* s_dir;
  if (argc < 2){
    s_dir = (char*)malloc(sizeof(char[2]));
    s_dir[0]= '.';
    s_dir[1]= '/';
  }else{
    s_dir = argv[1];
  }
  
  // Pipe File
  pipe(fd);
  pid_t pid = 0; 
  pid = fork();
  if (pid == 0){
    close(fd[0]);
    // Child process, Search the past directory
    search(s_dir,1);
    // Reached the end of the file time to close up shop
    close(fd[0]);
    close(fd[1]);
    // we need to
    printf("Done Searching\n");
    exit(0);
  }else if (pid > 0){
    // Parent Process 
    close(fd[1]);
    int total_count = 0;
    int bytes_to_read = 0;
    // Wait for the child process to complete it's stuff 
    wait(NULL); 
    ioctl(fd[0],FIONREAD,&bytes_to_read);
    char readbuffer[bytes_to_read];
    memset((void*)readbuffer,0,sizeof(char[bytes_to_read]));
     
    nbytes = read(fd[0], readbuffer,sizeof(readbuffer));
    // need to count the number of \ns in the readbuffer
    for (int i=0; i < nbytes; i++){
      if (readbuffer[i] == '\n'){
        total_count++;
      }
    }
    if (nbytes != bytes_to_read){
      printf("Tom Follery is afoot\n");
    }
    printf("Total Files and Directories = %d\n",total_count);
    close(fd[0]);
    close(fd[1]);
  }else{
    // Do nothing because it failed
  }

  return 0;
}

int search(char* s_dir, int level){
  DIR*    search_d; 
  struct dirent *dir;
  search_d = opendir(s_dir);
  if (search_d==NULL){
    return 1;
  }
  char wr_val[strlen(s_dir)+2];
  memcpy ((void*)wr_val,s_dir,strlen(s_dir));
  wr_val[strlen(s_dir)] = '\n';
  wr_val[strlen(s_dir)+1] = '\x00';
  write(fd[1],s_dir,strlen(s_dir));
  printf("%s",s_dir);
  printf(" (directory)\n");
  while ( ( dir = readdir(search_d)) ) {
    // Loop through each file inside of this file
    if (strcmp(dir->d_name,"..") != 0 && strcmp(dir->d_name,".") != 0){
      if (dir->d_type == DT_DIR ){
        // if this is a directory we need to spawn a new thread and
        // create a new process
        // We need to create the new path to this directory
        char new_address[strlen(s_dir) + strlen(dir->d_name) + 2];
        memcpy((void*)new_address,(void*)s_dir,strlen(s_dir));
        // Now we need to build the new pointer for new_address
        char* d_addr = (char*)(&new_address[0] + strlen(s_dir));
        memcpy((void*)d_addr, (void*) dir->d_name, strlen(dir->d_name));
        new_address[strlen(s_dir) + strlen(dir->d_name)] = '/'; 
        new_address[strlen(s_dir) + strlen(dir->d_name)+1] = '\x00';
        // Make sure that there is a backslash to search recursively
        search((char*)new_address,level+1);
        // We should also count the directory
      }else {
        char output[strlen(dir->d_name)+1];
        memcpy((void*)output,(void*)dir->d_name,strlen(dir->d_name));
        output[strlen(dir->d_name)] = '\n';
        // Otherwise it's a boring ass file so we pipe it out to the filecounter
        write(fd[1],output,(strlen(output)));
        printf("|");
        for (int i=0; i < level; i++){
          printf("-");
        }
        printf(">");
        // Read out the file
        printf("%s \n",dir->d_name);
      }
    }
  }
  return 0;
}
