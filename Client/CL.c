#include "CL_include"

#include "CL_mem.h"
#include "CL_msg.h"
#include "CL_sem.h"
#include "CL_Lib.h"

//Variables globales
static volatile sig_atomic_t flag;
static int msqid;
static int Tshmid;
static int SemidClient;

void fonction1(/*int sig*/) {
  printf("ICI USR 1\n");
    flag = 1;
}

void fonction2(/*int sig*/) {
  printf("ICI USR 2\n");
    flag = 2;
}

void end(){
    printf("\nRelache Mutex\nFin de Fils\n");
    DestructionMutex(SemidClient);
    while(wait(NULL)!=-1);
    kill(0,SIGKILL);
}
int main() {
  signal(SIGUSR1, fonction1);
  signal(SIGUSR2, fonction2);

  msqid = CreationMessagerie();

  dmsgbuf cle = connect(msqid);

  int Semid;
  if ((Semid = CreationMutex()) == -1) {
    perror("CreationMutex");
    exit(0);
  }
    BUF *Tptr;
    Tshmid = getTampon(&Tptr, cle.txt);
    //Creation de la mutex client
    
    if ((SemidClient = CreationMutexClient(cle.txt)) == -1) {
      perror("CreationMutex");
      exit(0);
    }

    int fils1,fils2;
    
    if((fils1=fork()) == -1){
        printf("ERREUR FORK FILS1");
        exit(8);
    }
    if(fils1 == 0){
        lecteur1(Semid,SemidClient,Tptr);
        exit(10);
    }
    if((fils2=fork()) == -1){
        printf("ERREUR FORK FILS2");
        exit(8);
    }
    if(fils2 == 0){
        lecteur2(Semid,SemidClient,Tptr);
        exit(10);
    }
    signal(SIGTERM, end);
    signal(SIGINT, end);
    while(1){
        pause();
        if (flag == 1){
            V(SemidClient,0);
            flag = 0;
        }
        if(flag == 2){
            V(SemidClient,1);
            flag = 0;
        }
    }

  // pour chaque canaux
  // fork
  // si lecteur

  // sinon

  return 0;
}
