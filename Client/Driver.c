#include "CL_include"

#include "CL_mem.h"
#include "CL_msg.h"
#include "CL_sem.h"
#include "CL_Lib.h"

/*Processus Lourd : Driver qui recupere les donnees des redacteur un a un*/
void read_from_pipe(int fd){
    FILE *stream;
    int c;
    stream = fdopen(fd,"r");
    fscanf(stream,"%d",&c);
    fclose(stream);
}

int main(int argc, char *argv[]) {
    int Semid_Driver;
    if ((Semid_Driver = CreationMutexClient(CleDriver)) == -1) {
      perror("CreationMutex");
      exit(0);
    }
    return 0;
}
