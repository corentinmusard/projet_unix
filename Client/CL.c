#include "CL_include"

#include "CL_mem.h"
#include "CL_msg.h"
#include "CL_sem.h"

static volatile sig_atomic_t flag;

void fonction1(/*int sig*/) {
  printf("ICI 1\n");
  flag = 1;
}

void fonction2(/*int sig*/) {
  printf("ICI 2\n");
  flag = 2;
}

void LireData(BUF *Tptr, int Voie) {
  int n = (Tptr + Voie)->n;
  printf("lire:%d\n", (Tptr + Voie)->tampon[n]);
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
