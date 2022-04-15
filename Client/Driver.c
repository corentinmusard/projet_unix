#include "CL_include"

#include "CL_mem.h"
#include "CL_msg.h"
#include "CL_sem.h"
#include "CL_Lib.h"

/*Processus Lourd : Driver qui recupere les donnees des redacteur un a un*/
void read_from_pipe(int fd){
    FILE *stream;
    int c;
    stream = fdopen(fd,"r");
    fscanf(stream,"%d",&c);
    fclose(stream);
}

int main(int argc, char *argv[]){
    
    int p3[2];
    int Semid_Driver;
    int buf[5];
    if(argc != 4){
        perror("Mauvais nombre d'arguments");
    }
    if((Semid_Driver = OuvertureMutexClient(CleDriver))==-1){
        perror("Ouverture Mutex Driver");
    }
    p3[0]=atoi(argv[1]);
    //printf("Affichage de l'argument 1 : %d\n",p3[0]);
    p3[1]=atoi(argv[2]);
    //printf("Affichage de l'argument 2 : %d\n",p3[1]);
    //printf("Affichage de l'argument 3 : %d\n",Semid_Driver);*/
    //Attente d'autorisation pour lire
    time_t date;
    while(1){
        //P(Semid_Driver,0);
        close(p3[1]);
        read(p3[0],buf,sizeof(buf));
        date = time(NULL);
        printf("*****************\nDATE : %sDATA : %d %d %d %d %d\n*****************\n",ctime(&date),buf[0],buf[1],buf[2],buf[3],buf[4]);
        //V(Semid_Driver,0);
    }
    
}
