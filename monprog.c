#include <stdio.h>
#include <mpi.h>
#include <unistd.h>
#include <sys/time.h>
#include <stdlib.h>
#include <time.h>  

#define iter 10
// la variable iter varie de 10 100 1000 10000

long get_time_us()  // returner le temps en unite us
{
    struct timeval my_time;  
    gettimeofday(&my_time, NULL);
    long runtime_us = my_time.tv_sec+ my_time.tv_usec; 
    return runtime_us;
}



int dot_prod(int *tab, int nb_elements,int size,int rank)
{
  // calcul des plages d'iterations de chaque neoud
  int seuil=(nb_elements/size);
  int plage_debut = seuil*rank;
  int plage_fin = nb_elements-rank*seuil-1;
  int prod = 0;
  
  for (int i = 0; i < 10; i++)
  {
    prod +=tab[i+plage_debut]*tab[plage_fin-i];
  }

  return prod;
}


int main(int argc, char **argv) {
     
    
     int min_run, max_run;
     int start, end;
     int local_prod =0;
     int *tab =NULL;
     FILE *f =fopen("faouzi.txt", "a+");
     
     tab=(int*)malloc(sizeof(int)*50);
     
     srand(time(NULL));
     //initialisation aléatoire du tableau 
     for (int i = 0; i < 50; i++)
     {
         tab[i] = rand()%50;
         printf("\t tab[%d]=%d \t",i,tab[i]);
             
     }
      
     /* begin traitement long*/     
     int rank, size;
     MPI_Init ( &argc, &argv );
     MPI_Comm_rank(MPI_COMM_WORLD, &rank);
     MPI_Comm_size(MPI_COMM_WORLD, &size);
     start=(int)get_time_us();
     
      for (int i = 0; i < iter; i++)
      {  rewind(f);
        
      for (int j = 0; j < size; j++)
      {
        if(rank == j)
        { // calcul des produits scalaires locaux 
         local_prod = dot_prod(tab, 50,size,rank);
         printf("local_prod=%d\n",local_prod);
         fprintf(f,"%d\n",local_prod);
        }

      MPI_Barrier(MPI_COMM_WORLD);
      }
     }

     /*fin traitement long */ 
     end= (int)get_time_us();
     
     // les temps de traitements min et max des noeuds
     MPI_Allreduce(&end,&min_run,1,MPI_INT,MPI_MIN,MPI_COMM_WORLD);
     MPI_Allreduce(&end,&max_run,1,MPI_INT,MPI_MAX,MPI_COMM_WORLD);
 
     printf("Min time: %d  Max time: %d  \n", min_run, max_run);
     MPI_Finalize();
     fclose(f); 
   
     return 0;
}   
     
