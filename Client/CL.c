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
static int Semid_Driver;

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
    FILE *F;
    F = fopen(CleDriver,"w");
    if(F == NULL){
        perror("Fichier");
    }
    fclose(F);

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

    int fils1,fils2;//Fils lecteurs 1 et 2
    int r1,r2,d;
    int p1[2]; //Pipe redacteur 1
    int p2[2];//Pipe redacteur 2
    int p3[2];//Pipe driver
    
    //Creation des pipes
    if(pipe(p3)==-1){//Gestion d'erreur de creation de la pipe
        perror("Erreur pipe 3");
    }
    if(pipe(p1)==-1){
        perror("Erreur pipe 1");
    }
    if(pipe(p2)==-1){
        perror("Erreur pipe 2");
    }
    //Fils lecteur 1 :
    if((fils1=fork()) == -1){
        printf("ERREUR FORK FILS LECETEUR 1");
        exit(8);
    }
    if(fils1 == 0){//Code Lecteur 1
        lecteur1(Semid,SemidClient,Tptr,p1);
        exit(10);
    }
    //Fils lecteur 2 :
    if((fils2=fork()) == -1){
        printf("ERREUR FORK FILS LECETEUR 1");
        exit(8);
    }
    if(fils2 == 0){//Code Lecteur 2
        lecteur2(Semid,SemidClient,Tptr,p2);
        exit(10);
    }
    
    // Creation de la mutex partagee avec le driver
    if ((Semid_Driver = CreationMutexClient(CleDriver)) == -1) {
      perror("CreationMutex pour le driver");
      exit(0);
    }
    V(Semid_Driver,0);
    //Fils redacteur 1 :
    if((r1=fork()) == -1){
        printf("ERREUR FORK FILS REDACTEUR 1");
        exit(8);
    }
    if(r1 == 0){//Code redacteur 1
        redacteur1(p1,p3,Semid_Driver);
        exit(10);
    }
    //Fils redacteur 2 :
    if((r2=fork()) == -1){
        printf("ERREUR FORK FILS REDACTEUR 2");
        exit(8);
    }
    if(r2 == 0){//Code redacteur 2
        redacteur2(p2,p3,Semid_Driver);
        exit(10);
    }

    printf("Mutex Driver : %d\n",Semid_Driver);
    //Creation du troisieme fils de client :
    //Fils qui accede au driver
    if((d=fork()) == -1){
        printf("ERREUR FORK FILS POUR DRIVER");
        exit(8);
    }
    if(d == 0){
        char p3_zero[10];
        sprintf(p3_zero,"%d",p3[0]);
        char p3_un[10];
        sprintf(p3_un,"%d",p3[1]);
        char Semid_Driver_alpha[10];
        sprintf(Semid_Driver_alpha,"%d",Semid_Driver);
        printf("ARG : %s     %s    %s\n",p3_zero,p3_un,Semid_Driver_alpha);
        if (execlp("./Driver","./Driver",p3_zero,p3_un,Semid_Driver_alpha,NULL) == -1)
          {
            printf("Erreur de execlp\n");
          }
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
  return 0;
}
