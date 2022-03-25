/* Fichier SV_mem.c */

#include "CL_include"
#include "CL_mem.h"

int getTampon(BUF **Ptr, char *cle)
{
BUF *MemTamponPtr;
int CLTshmid;
key_t key;

if (( key = ftok(cle,'T')) < 0 )
   return CLEerr;
if ((CLTshmid = shmget(key,2*sizeof(BUF),0666)) <0)
   return SHMerr;
if ((MemTamponPtr = (BUF *)shmat(CLTshmid,NULL,0)) == 0 )
   return SHMerr;
*Ptr = MemTamponPtr ;
return CLTshmid;
}

/*********************  DESALLOCATION DES TAMPONS
*  ENTREE: Tshmid,Cshmid = ID des memoires partagee
*  SORTIE: neant
*  RETOUR: code erreur <0 ou  0 ;
*/
int RelacheMemoires(int Cshmid)
{
int ret = 0;

if (Cshmid)
   ret += shmctl(Cshmid,IPC_RMID,NULL);
return ret;
}

