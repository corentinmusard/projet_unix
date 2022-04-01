#include "CL_Lib.h"
#include "CL_include"
#include "CL_mem.h"
#include "CL_msg.h"
#include "CL_sem.h"

void LireData(BUF *Tptr, int Voie) {
  int n = (Tptr + Voie)->n; // + voies a verifier si utile
  printf("lire:%d\n", (Tptr + Voie)->tampon[n]);
}

void lecteur1(int Semid, int SemidClient, BUF *Tptr){
    while(1){
        printf("AVANT P CANAL 1\n");
        P(SemidClient,0);
        P(Semid,0);
        printf("APRES P CANAL 1\n");
        LireData(Tptr,0);
        V(Semid,0);
    }

}
void lecteur2(int Semid, int SemidClient, BUF *Tptr){
    while(1){
        printf("AVANT P CANAL 2\n");
        P(SemidClient,1);
        P(Semid,1);
        printf("APRES P CANAL 2\n");
        LireData(Tptr,1);
        V(Semid,1);
    }
    
}

