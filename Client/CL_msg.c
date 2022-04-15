#include "CL_msg.h"
#include "CL_include"

int CreationMessagerie(void) {
  key_t key;
  int msqid;
  if ((key = ftok(CleServeur, 'M')) < 0)
    return CLEerr;
  msqid = msgget(key, 0666);
  return msqid;
}

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

  msg.type = ACK;
  if (msgsnd(msqid, &msg, strlen(msg.txt) + 1, 0) < 0) {
    perror("msgsnd ACK");
    exit(1);
  }
  return msg;
}
