// Title :: Problem 4 Searcher Implementation
// Author:: Nicholas Hemstreet



#include <sys/types.h>
#include <sys/param.h>
#include <sys/stat.h>
#include <dirent.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>



int main(int argc, char** argv){
  // Setup the file descriptors and output to 
  // We exit if we don't get a directory name
  if (argc < 1)
    return 0;
  DIR*    search_d; 
  struct dirent *di;
  char* dir_name = argv[0];
  search_d = opendir(dir_name);
  if (search_d==NULL){
    return 1;
  }
  while ( ( dir = readdir(search_d)) ) {
    // Loop through each file inside of this file
    if (dir->d_type == DT_DIR ){
      // if this is a directory we need to spawn a new thread and
      // create a new process
    }else {
      // Otherwise it's a boring ass file so we pipe it out to the filecounter
      
    }
  }
  
  
  return 0;
}

