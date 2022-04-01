#ifndef CL_SEM
#define CL_SEM

#include "CL_include"

int CreationMutex(void);
int CreationMutexClient(char *cle);
int DestructionMutex(int);
int Init_Mutex(int, int);
int P(int, int);
int V(int, int);

#endif
