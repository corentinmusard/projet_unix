#include "CL_Lib.h"
#include "CL_include"
#include "CL_mem.h"
#include "CL_msg.h"
#include "CL_sem.h"

int LireData(BUF *Tptr, int Voie) {
  int n = (Tptr + Voie)->n; // + voies a verifier si utile
  int data = (Tptr + Voie)->tampon[n];
  return data;
}

void lecteur1(int Semid, int SemidClient, BUF *Tptr, int p1[2]) {
  while (1) {
    int data[1];
    P(SemidClient, 0);
    P(Semid, 0);
    data[0] = LireData(Tptr, 0);
    V(Semid, 0);

    close(p1[0]);
    write(p1[1], data, sizeof(data));
  }
}
void lecteur2(int Semid, int SemidClient, BUF *Tptr, int p2[2]) {
  while (1) {
    int data[1];
    P(SemidClient, 1);
    P(Semid, 1);
    data[0] = LireData(Tptr, 1);
    V(Semid, 1);
    close(p2[0]);
    write(p2[1], data, sizeof(data));
  }
}

void redacteur1(int p1[2], int p3[2], int Semid_Driver) {
  int i = 0;
  int buf[5] = {0};
  while (1) {
    close(p1[1]);
    read(p1[0], &buf[i], sizeof(int));
    i++;
    if (i == 5) {
      P(Semid_Driver, 0);
      close(p3[0]);
      write(p3[1], buf, sizeof(buf));
      V(Semid_Driver, 0);
      i = 0;
    }
  }
}

void redacteur2(int p1[2], int p3[2], int Semid_Driver) {
  int i = 0;
  int buf[5] = {0};
  while (1) {
    close(p1[1]);
    read(p1[0], &buf[i], sizeof(int));
    i++;
    if (i == 5) {
      P(Semid_Driver, 0);
      close(p3[0]);
      write(p3[1], buf, sizeof(buf));
      V(Semid_Driver, 0);
      i = 0;
    }
  }
}
