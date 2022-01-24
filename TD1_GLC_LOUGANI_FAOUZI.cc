#include <stdio.h>
#include <mpi.h>
#include <unistd.h>

/* 
Le code suivant applique le principe de course ou chaque membre court l un après l autre en transmission de témoin 
chaque processus de rang inferieur prend le temoin (ici un entier=99) et le transmet a au process don le rang est supsuperieur 
*/

int main(int argc, char **argv) {

    int rk, temoin, tag1;
    MPI_Status sta;
    char str[MPI_MAX_PROCESSOR_NAME];
    int len,P;
    
    //initialisation de l environnement
    MPI_Init(&argc, &argv);
    
    //reccuperer le rang du processus
    MPI_Comm_rank(MPI_COMM_WORLD, &rk);
    
    //recuperer le nombre total de processus
    MPI_Comm_size(MPI_COMM_WORLD, &P);
    
    //recuperer le nom du processeur
    MPI_Get_processor_name(str, &len);
    
    
    tag1 = 1000;
    if (rk == 0)
    {
        temoin = 99;
        MPI_Ssend(&temoin, 1, MPI_INT, 1, tag1, MPI_COMM_WORLD);
        
        printf("Je suis le processus de rang=%d et j'ai recu le temoin= %d \n", rk, temoin);  
        printf("nom processeur =%s\n",str);
        printf("nombre de processus total=%d\n",P);
    }
    else if (rk == 1)
    {
        MPI_Recv(&temoin, 1, MPI_INT, 0, tag1, MPI_COMM_WORLD, &sta);
        printf("Je suis le processus de rang=%d et j'ai recu le temoin= %d \n", rk, temoin);   
        MPI_Ssend(&temoin, 1, MPI_INT, 2, tag1, MPI_COMM_WORLD);
    }
    else if (rk == 2)
    {
	MPI_Recv(&temoin, 1, MPI_INT, 1, tag1, MPI_COMM_WORLD, &sta);
	printf("Je suis le processus de rang=%d et j'ai recu le temoin= %d \n", rk, temoin); 
	MPI_Ssend(&temoin, 1, MPI_INT, 3, tag1, MPI_COMM_WORLD); 
    }
    else if (rk == 3)
    {
	MPI_Recv(&temoin, 1, MPI_INT, 2, tag1, MPI_COMM_WORLD, &sta);
	printf("Je suis le processus de rang=%d et j'ai recu le temoin= %d \n", rk, temoin); 
	
    }
   
    if(P!=4)
    {  
       //abort si on a pas utiliser strictement 4  processus 
       MPI_Abort(MPI_COMM_WORLD, 1);
    }
    
    //finalisation des processus MPI 
    MPI_Finalize();
    
     return 0;
}   
