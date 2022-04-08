#ifndef CL_LIB
#define CL_LIB

#include "CL_include"

int LireData(BUF *Tptr, int Voie);
void lecteur1(int Semid, int SemidClient, BUF *Tptr, int p1[2]);
void lecteur2(int Semid, int SemidClient, BUF *Tptr, int p2[2]);
void redacteur1(int p1[2]);
void redacteur2(int p1[2]);

#endif

