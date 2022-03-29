/* Ficier SV_msg.c */

#include "CL_msg.h"
#include "CL_include"

/****************   CREATION DE LA MESSAGERIE
 *  ENTREE: Neant
 *  SORTIE: la messagerie est cree
 *  RETOUR: code erreur <0 ou identifcateur de la messagerie
 */
int CreationMessagerie(void) {
  key_t key;
  int msqid;
  if ((key = ftok(CleServeur, 'M')) < 0)
    return CLEerr;
  msqid = msgget(key, 0666);
  return msqid;
}

/*********************  RELACHE LA MESSAGERIE
 *  ENTREE: msqid = identificateur de la messagerie
 *  SORTIE: neant
 *  RETOUR: code erreur -1  ou 0
 */
int RelacheMessagerie(int msqid) { return (msgctl(msqid, IPC_RMID, NULL)); }

dmsgbuf connect(int msqid) {
  dmsgbuf msg;
  msg.type = CONNECT;
  int cpid = getpid();
  sprintf(msg.txt, "%d", cpid);

  if (msgsnd(msqid, &msg, strlen(msg.txt) + 1, 0) < 0) {
    perror("msgsnd CONNECT");
    exit(1);
  }

  if (msgrcv(msqid, &msg, L_MSG, cpid, 0) < 0) {
    perror("msgrecv cle");
    exit(1);
  }
  printf("txt: %s\n", msg.txt);

  msg.type = ACK;
  if (msgsnd(msqid, &msg, strlen(msg.txt) + 1, 0) < 0) {
    perror("msgsnd ACK");
    exit(1);
  }
  return msg;
}
