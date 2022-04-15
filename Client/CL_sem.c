#include "CL_sem.h"
#include "CL_include"

int CreationMutex(void) {
  key_t key;
  int semid;
  if ((key = ftok(CleServeur, 'S')) < 0)
    return CLEerr;
  semid = semget(key, NVOIES, 0666);
  return semid;
}

int CreationMutexClient(char *cle) {
  key_t key;
  int semid;
  if ((key = ftok(cle, 'F')) < 0) // CrŽation d'une clŽe client
    return CLEerr;
  semid = semget(key, NVOIES, 0666 | IPC_CREAT);
  return semid;
}

int OuvertureMutexClient(char *cle) {
  key_t key;
  int semid;
  if ((key = ftok(cle, 'F')) < 0) // CrŽation d'une clŽe client
    return CLEerr;
  semid = semget(key, NVOIES, 0666);
  return semid;
}

int DestructionMutex(int semid) { return (semctl(semid, 0, IPC_RMID, 0)); }

int P(int semid, int voie) {
  struct sembuf semoper;
  semoper.sem_num = voie;
  semoper.sem_op = -1;
  semoper.sem_flg = 0;

  if (semop(semid, &semoper, 1) < 0) {
    perror("Erreur P sur le Mutex");
    return SEMerr;
  }

  return 0;
}

int V(int semid, int voie) {
  struct sembuf semoper;
  semoper.sem_num = voie;
  semoper.sem_op = 1;
  semoper.sem_flg = 0;

  if (semop(semid, &semoper, 1) < 0) {
    perror("Erreur V sur le Mutex");
    return SEMerr;
  }

  return 0;
}
