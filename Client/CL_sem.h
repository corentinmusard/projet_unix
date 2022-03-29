#ifndef CL_SEM
#define CL_SEM

int CreationMutex(void);
int DestructionMutex(int);
int Init_Mutex(int, int);
int P(int, int);
int V(int, int);

#endif
