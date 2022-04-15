/* Fichier SV_mem.c */

#include "CL_mem.h"
#include "CL_include"

int getTampon(BUF **Ptr, char *cle) {
  BUF *MemTamponPtr;
  int CLTshmid;
  key_t key;

  if ((key = ftok(cle, 'T')) < 0)
    return CLEerr;
  if ((CLTshmid = shmget(key, 2 * sizeof(BUF), 0666)) < 0)
    return SHMerr;
  if ((MemTamponPtr = (BUF *)shmat(CLTshmid, NULL, 0)) == 0)
    return SHMerr;
  *Ptr = MemTamponPtr;
  return CLTshmid;
}

int RelacheMemoires(int Cshmid) {
  int ret = 0;

  if (Cshmid)
    ret += shmctl(Cshmid, IPC_RMID, NULL);
  return ret;
}
