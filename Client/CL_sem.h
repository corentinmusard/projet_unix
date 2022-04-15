#ifndef CL_SEM
#define CL_SEM

#include "CL_include"

/**
 * Connection de la semaphore serveur de taille NVOIES
 * @return l'id de la semaphore
 */
int CreationMutex(void);

/**
 * Création d'une semaphore
 * @param cle argument donné à ftok
 * @return l'id de la semaphore
 */
int CreationMutexClient(char *cle);

/**
 * Connection de la semaphore par l'identifiant `cle`
 * @return l'id de la semaphore
 */
int OuvertureMutexClient(char *cle);

/**
 * Detruit la semaphore `semid`
 * @param semid de la semaphore
 * @return -1 en cas d'erreur
 */
int DestructionMutex(int semid);

/**
 * Applique l'opération P sur la mutex
 * @param semid id de la semaphore
 * @param voie numéro de la voie
 * @return SEMerr en cas d'erreur
 */
int P(int semid, int voie);

/**
 * Applique l'opération V sur la mutex
 * @param semid id de la semaphore
 * @param voie numéro de la voie
 * @return SEMerr en cas d'erreur
 */
int V(int semid, int voie);

#endif
