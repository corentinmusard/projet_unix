#include "CL_include"

#include "CL_Lib.h"
#include "CL_mem.h"
#include "CL_msg.h"
#include "CL_sem.h"

/*Processus Lourd : Driver qui recupere les donnees des redacteur un a un*/

int main(int argc, char *argv[]) {
  int p3[2];
  int Semid_Driver;
  int buf[5];
  if (argc != 3) {
    fprintf(stderr, "Mauvais nombre d'arguments\n");
    return 1;
  }
    //Ouverture de la mutex Driver cree dans le client
  if ((Semid_Driver = OuvertureMutexClient(CleDriver)) == -1) {
    perror("Ouverture Mutex Driver");
    return 1;
  }
    // Recuperation des arguments passes par le client
  p3[0] = atoi(argv[1]);
  p3[1] = atoi(argv[2]);

  
  time_t date;
  while (1) {
    P(Semid_Driver, 0);//Attente du droit d'acces
    close(p3[1]);//On ferme l'entree du pipe
    read(p3[0], buf, sizeof(buf));// On lit les donnees de la pipe et on ecrit dans buff
    date = time(NULL);
      //Affichage des donnees recus par les redacteurs
    printf("*****************\nDATE : %sDATA : %d %d %d %d "
           "%d\n*****************\n",
           ctime(&date), buf[0], buf[1], buf[2], buf[3], buf[4]);
    V(Semid_Driver, 0); //Rend l'acces
  }
}
