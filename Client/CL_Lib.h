#ifndef CL_LIB
#define CL_LIB

#include "CL_include"

void LireData(BUF *Tptr, int Voie);
void lecteur1(int Semid, int SemidClient, BUF *Tptr);
void lecteur2(int Semid, int SemidClient, BUF *Tptr);

#endif

