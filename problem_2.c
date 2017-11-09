// Title:: main.c

#include <unistd.h>
#include <stdlib.h>

int main() {
  
  FILE* in_file = fopen("input.txt", "w");
  FILE* out_file = fopen("output.txt","w");
  
  pid_t pid = fork();
  if (pid == 0){
    // The output of the picodb function will be the input to this program
    dup2(fileno(in_file),1);
    // the input file to the picodb function is our output file from this program
    dup2(fileno(out_file),0);
    // Now that we have setup the files we execute the other program
    execl("./picodb","");
  }else if (pid > 0){
    // Parent process
    // To send data to the other function we just write to the in_file
    // To get data from the other function we read from the out_file
  }
  return 0;
}
