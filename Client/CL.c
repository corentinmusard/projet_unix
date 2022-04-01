#include "CL_include"

#include "CL_mem.h"
#include "CL_msg.h"
#include "CL_sem.h"

static volatile sig_atomic_t flag;

void LireData(BUF *Tptr, int Voie) {
  int n = (Tptr + Voie)->n; // + voies a verifier si utile
  printf("lire:%d\n", (Tptr + Voie)->tampon[n]);
}

void lecteur1(int Semid, int SemidClient, BUF *Tptr){
    printf("AVANT P CANAL 1\n");
    P(SemidClient,0);
    printf("APRES P CANAL 1\n");
    LireData(Tptr,0);
    V(SemidClient,0);
}
void lecteur2(int Semid, int SemidClient, BUF *Tptr){
    printf("AVANT P CANAL 2\n");
    P(SemidClient,1);
    printf("APRES P CANAL 2\n");
    LireData(Tptr,1);
    V(SemidClient,1);
}
void fonction1(/*int sig*/) {
  printf("ICI 1\n");
    flag = 1;
}

void fonction2(/*int sig*/) {
  printf("ICI 2\n");
    flag = 2;
}

int main() {
  signal(SIGUSR1, fonction1);
  signal(SIGUSR2, fonction2);

  int msqid = CreationMessagerie();

  dmsgbuf cle = connect(msqid);

  int Semid;
  if ((Semid = CreationMutex()) == -1) {
    perror("CreationMutex");
    exit(0);
  }
    BUF *Tptr;
    int Tshmid = getTampon(&Tptr, cle.txt);
    //Creation de la mutex client
    int SemidClient;
    if ((SemidClient = CreationMutexClient()) == -1) {
      perror("CreationMutex");
      exit(0);
    }
    if(Init_Mutex(SemidClient,1)==SEMerr){
        printf("Error Init Mutex");
        exit(3);
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
    if(fils1 == 0){
        lecteur2(Semid,SemidClient,Tptr);
        exit(10);
    }
    //P(SemidClient,1);
    //P(SemidClient,2);
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

  RelacheMemoires(Tshmid);
  RelacheMessagerie(msqid);
  return 0;
}
