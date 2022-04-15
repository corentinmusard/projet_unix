#ifndef CL_LIB
#define CL_LIB

#include "CL_include"
/**
 *Lecture des donnees de la memoire partagee
 *@param Tptr buffer contenant les informations a lire dans la memoire partagee
 *@param Voie voie de lecture (USR1 ou USR2)
 *@return valeur du tampon Tprtr
 */
int LireData(BUF *Tptr, int Voie);
/**
 *Lecture des donnees sur la voie 1
 *@param Semid semaphore du serveur
 *@param SemidClient semaphore SemidClient
 *@param Tptr buffer contenant les informations a lire dans la memoire partagee
 *@param p1 pipe communicant avec la fonction redacteur1 (sur la voie 1)
 */
void lecteur1(int Semid, int SemidClient, BUF *Tptr, int p1[2]);
/**
 *Lecture des donnees sur la voie 2
 *@param Semid semaphore du serveur
 *@param SemidClient semaphore SemidClient
 *@param Tptr buffer contenant les informations a lire dans la memoire partagee
 *@param p2 pipe communicant avec la fonction redacteur2 (sur la voie 2)
 */
void lecteur2(int Semid, int SemidClient, BUF *Tptr, int p2[2]);
/**
 *Le redacteur1 recupere les donnees une a une du pipe 1 remplis 5 a 5 les donnes dans un buffer et l'envoie dan la pipe 3 pour le driver
 *@param p1 pipe de lien avec le lecteur 1
 *@param p3 pipe de transmission au driver
 *@param Semid_Driver mutex d'exclusion mutuelle driver client
 */
void redacteur1(int p1[2], int p3[2], int Semid_Driver);
/**
 *Le redacteur2 recupere les donnees une a une du pipe 2 remplis 5 a 5 les donnes dans un buffer et l'envoie dan la pipe 3 pour le driver
 *@param p1 pipe de lien avec le lecteur 2
 *@param p3 pipe de transmission au driver
 *@param Semid_Driver mutex d'exclusion mutuelle driver client
 */
void redacteur2(int p1[2], int p3[2], int Semid_Driver);

#endif
