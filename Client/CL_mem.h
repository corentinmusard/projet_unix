#ifndef CL_MEM
#define CL_MEM

#include "CL_include"

/**
 * Relache la mémoire de `Cshmid`
 * @param Ptr pointeur vers le tampon
 * @param cle chaine de caractère utilisé pour ftok
 * @return SHMerr en cas d'erreur
 */
int getTampon(BUF **Ptr, char *cle);

/**
 * Relache la mémoire du tampon `Cshmid`
 * @param Cshmid Shared memory id
 * @return -1 en cas d'erreur
 */
int RelacheMemoires(int Cshmid);

#endif
