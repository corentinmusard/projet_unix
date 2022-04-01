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
    P(SemidClient,1);
    LireData(Tptr,1);
    V(SemidClient,1);
}
void lecteur2(int Semid, int SemidClient, BUF *Tptr){
    P(SemidClient,2);
    LireData(Tptr,2);
    V(SemidClient,2);
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

  int i = 0;
  while (i < 10) {
    while (flag == 0) {
      pause();
    }
    printf("flag=%d\n", flag);
    P(Semid, flag);
    LireData(Tptr, flag - 1);
    V(Semid, flag);
    flag = 0;
    i++;
  }

  while (1)
    ;
  // pour chaque canaux
  // fork
  // si lecteur

  // sinon

  RelacheMemoires(Tshmid);
  RelacheMessagerie(msqid);
  return 0;
}
