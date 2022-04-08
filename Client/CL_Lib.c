#include "CL_Lib.h"
#include "CL_include"
#include "CL_mem.h"
#include "CL_msg.h"
#include "CL_sem.h"

int LireData(BUF *Tptr, int Voie) {
  int n = (Tptr + Voie)->n; // + voies a verifier si utile
  int data = (Tptr + Voie)->tampon[n];
  printf("lire:%d\n", data);
    return data;
}

void lecteur1(int Semid, int SemidClient, BUF *Tptr, int p1[2]){
    while(1){
        int data[1];
        printf("AVANT P CANAL 1\n");
        P(SemidClient,0);
        P(Semid,0);
        printf("APRES P CANAL 1\n");
        data[0] = LireData(Tptr,0);
        V(Semid,0);
        
        close(p1[0]);
        write(p1[1],data,sizeof(data));
    }

}
void lecteur2(int Semid, int SemidClient, BUF *Tptr, int p2[2]){
    while(1){
        int data[1];
        printf("AVANT P CANAL 2\n");
        P(SemidClient,1);
        P(Semid,1);
        printf("APRES P CANAL 2\n");
        data[0] = LireData(Tptr,1);
        V(Semid,1);
        close(p2[0]);
        write(p2[1],data,sizeof(data));
    }
    
}

void redacteur1(int p1[2]){
    int i=0;
    int buf[5] = {0};
    while(1){
        close(p1[1]);
        read(p1[0],&buf[i],sizeof(int));
        i++;
        if(i==5){
            printf("DATA 1 : %d %d %d %d %d\n",buf[0],buf[1],buf[2],buf[3],buf[4]);
            i=0;
        }
    }
}

void redacteur2(int p1[2]){
    int i=0;
    int buf[5] = {0};
    while(1){
        close(p1[1]);
        read(p1[0],&buf[i],sizeof(int));
        i++;
        if(i==5){
            printf("DATA 2 : %d %d %d %d %d\n",buf[0],buf[1],buf[2],buf[3],buf[4]);
            i=0;
        }
    }
}

