#ifndef CL_MSG
#define CL_MSG

#include "CL_include"

/**
 * Création de la messagerie
 * @return l'id de la messagerie
 */
int CreationMessagerie(void);

/**
 * Fermeture de la liaison avec la messagerie
 * @param msqid l'id de messagerie
 * @return -1 en cas d'erreur
 */
int RelacheMessagerie(int msqid);

/**
 * Connection à la messagerie
 * @param msqid l'id de messagerie
 * @return la cle de la semaphore
 */
dmsgbuf connect(int msqid);

#endif
