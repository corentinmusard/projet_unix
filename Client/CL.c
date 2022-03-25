#define _POSIX_C_SOURCE 1

#include "CL_include"

#include "CL_msg.h"
#include "CL_mem.h"
#include "CL_sem.h"

void fonction() {
  printf("ICI 1\n");
}

void fonction2() {
  printf("ICI 2\n");
}

void LireData(BUF *Tptr, int Voie) {
  int n = (Tptr + Voie)->n;
  printf("lire:%d,", (Tptr+Voie)->tampon[n]);
}

int main() {
  signal(SIGUSR1, fonction);
  signal(SIGUSR2, fonction2);

  int msqid = CreationMessagerie();

  dmsgbuf cle = connect(msqid);

  int Semid;
  if ((Semid = CreationMutex()) == -1)
    {
      perror("CreationMutex");
      exit(0);
    }

  BUF *Tptr;
  int Tshmid = getTampon(&Tptr, cle.txt);

  int Voie = 1;
  int i=0;
  while(i < 10) {
  V(Semid, Voie);
  LireData(Tptr, Voie);
  P(Semid, Voie);
  i++;
}

  //while (1);
  //pour chaque canaux
    //fork
    //si lecteur


    //sinon

  RelacheMemoires(Tshmid);
  RelacheMessagerie(msqid);
  return 0;
}