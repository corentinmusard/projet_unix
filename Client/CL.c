#include "CL_include"

#include "CL_Lib.h"
#include "CL_mem.h"
#include "CL_msg.h"
#include "CL_sem.h"

// Variables globales
static volatile sig_atomic_t flag;
static int msqid;
static int Tshmid;
static int SemidClient;
static int Semid_Driver;

/**
 *fonction d'interruprion pour l'USER 1
 *Modification du flag a 1 quand on est dans l'USER 1
 */
void Handler_USR1() {
  printf("ICI USR 1\n");
  flag = 1;
}
/**
 *fonction d'interruprion pour l'USER 2
 *Modification du flag a 2 quand on est dans l'USER 2
 */
void Handler_USR2() {
  printf("ICI USR 2\n");
  flag = 2;
}

void end() {
  printf("\nRelache Mutex\nFin de Fils\n");
  DestructionMutex(SemidClient);
  while (wait(NULL) != -1)
    ;
  kill(0, SIGKILL);
}
int main() {
  signal(SIGUSR1, Handler_USR1);
  signal(SIGUSR2, Handler_USR2);

  msqid = CreationMessagerie();

  dmsgbuf cle = connect(msqid);
    //Creation du ficher associe à la CleDriver
  FILE *F;
  F = fopen(CleDriver, "w");
  if (F == NULL) {
    perror("Fichier");
    return 1;
  }
  fclose(F);

  int Semid;
    //Recuperation de la mutex seveur (deja cree dans le serveur)
  if ((Semid = CreationMutex()) == -1) {
    perror("CreationMutex");
    return 1;
  }
  BUF *Tptr;
  Tshmid = getTampon(&Tptr, cle.txt);
    
  // Creation de la mutex client
  if ((SemidClient = CreationMutexClient(cle.txt)) == -1) {
    perror("CreationMutex");
    return 1;
  }

  int fils1, fils2; // Fils lecteurs 1 et 2
  int r1, r2, d;
  int p1[2]; // Pipe redacteur 1
  int p2[2]; // Pipe redacteur 2
  int p3[2]; // Pipe driver

  // Creation des pipes
  if (pipe(p3) == -1) { // Gestion d'erreur de creation de la pipe
    perror("Erreur pipe 3");
  }
  if (pipe(p1) == -1) {
    perror("Erreur pipe 1");
  }
  if (pipe(p2) == -1) {
    perror("Erreur pipe 2");
  }
  // Fils lecteur 1 :
  if ((fils1 = fork()) == -1) {
    perror("ERREUR FORK FILS LECETEUR 1");
    return 8;
  }
  if (fils1 == 0) { // Code Lecteur 1
    lecteur1(Semid, SemidClient, Tptr, p1);
    return 10;
  }
  // Fils lecteur 2 :
  if ((fils2 = fork()) == -1) {
    perror("ERREUR FORK FILS LECETEUR 1");
    return 8;
  }
  if (fils2 == 0) { // Code Lecteur 2
    lecteur2(Semid, SemidClient, Tptr, p2);
    return 10;
  }

  // Creation de la mutex partagee avec le driver
  if ((Semid_Driver = CreationMutexClient(CleDriver)) == -1) {
    perror("CreationMutex pour le driver");
    return 0;
  }
  V(Semid_Driver, 0);
  // Fils redacteur 1 :
  if ((r1 = fork()) == -1) {
    perror("ERREUR FORK FILS REDACTEUR 1");
    return 8;
  }
  if (r1 == 0) { // Code redacteur 1
    redacteur1(p1, p3, Semid_Driver);
    return 10;
  }
  // Fils redacteur 2 :
  if ((r2 = fork()) == -1) {
    perror("ERREUR FORK FILS REDACTEUR 2");
    return 8;
  }
  if (r2 == 0) { // Code redacteur 2
    redacteur2(p2, p3, Semid_Driver);
    return 10;
  }

  // Creation du troisieme fils de client :
  // Fils qui accede au driver
  if ((d = fork()) == -1) {
    printf("ERREUR FORK FILS POUR DRIVER");
    return 8;
  }
  if (d == 0) {
    char p3_zero[10];
    sprintf(p3_zero, "%d", p3[0]);
    char p3_un[10];
    sprintf(p3_un, "%d", p3[1]);
    if (execlp("./Driver", "./Driver", p3_zero, p3_un,
               NULL) == -1) {
      printf("Erreur de execlp\n");
    }
    return 10;
  }

  signal(SIGTERM, end);
  signal(SIGINT, end);
  while (1) {
    pause();
    if (flag == 1) {
      V(SemidClient, 0);
      flag = 0;
    }
    if (flag == 2) {
      V(SemidClient, 1);
      flag = 0;
    }
  }
  return 0;
}
