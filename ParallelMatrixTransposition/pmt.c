#include <stdio.h>
#include <mpi.h>
 
void Swap(int a[4][1], int b[1][4])
{
     int i, j;
      
    for (j=0;j<1;++j)
          {
           for (i=0;i<4;++i)
           {
               a[i][j] = b[j][i]; 
            }
          }
}
void PrintMatrix(int ar[4][1])
{
    int i = 0;
    int j = 0;
    for(i = 0; i < 4; i++)
    {
        for( j = 0; j < 1; j++)
        {   
            printf("%4d", ar[i][j]);
        }
             
        putchar('\n'); 
    }
}
void PrintMatrix_N(int ar[][4])
{
    int i = 0;
    int j = 0;
    for(i = 0; i < 4; i++)
    {
        for( j = 0; j < 4; j++)
        {   
            printf("%4d", ar[i][j]);
        }
        putchar('\n'); 
    }
}
 
int main(int argc, char *argv[])
{
   
  int size, rank;
  double t1, t2;
   
  int A1[1][4] = {
                    {1, 2, 3, 4}
                    },
      A2[1][4] = {
                    {5, 6, 7, 8}
                    },
      A3[1][4] = {
                    {9, 10, 11, 12}
                    },                            
      A4[1][4] = {
                    {13, 14, 15, 16}
                    }, 
   
  A[4][4] = {
                  {1, 2, 3, 4},
                  {5, 6, 7, 8},
                  {9, 10, 11, 12},
                  {13, 14, 15, 16}
                  },
  C[4][1] = { 
            {0},
            {0},
            {0},
            {0}
   },
                   
       A_vzor[4][4] = {
                  {1, 5, 9, 13},
                  {2, 6, 10, 14},
                  {3, 7, 11, 15},
                  {4, 8, 12, 16}
                  };            
   
  MPI_Status status;
  MPI_Request request;
   
  MPI_Init(&argc, &argv);
  MPI_Comm_size(MPI_COMM_WORLD, &size);
   
   
  if(size == 5)
  {
           
     MPI_Comm_rank(MPI_COMM_WORLD, &rank);  
     MPI_Bcast(C, 4, MPI_INT, 0, MPI_COMM_WORLD);
             
              
              
     if(rank == 0) 
     { 
             t1 = MPI_Wtime();
              
         printf("\n\nMatica A:\n");
         PrintMatrix_N(A);   
          
         printf("\n\nMatica A* - vzorova:\n");    
         PrintMatrix_N(A_vzor);    
          
         printf("\n\n\nVysledok:\n"); 
          
         MPI_Send(A1, 4, MPI_INT, 1, 01, MPI_COMM_WORLD);
         MPI_Send(A2, 4, MPI_INT, 2, 02, MPI_COMM_WORLD);
         MPI_Send(A3, 4, MPI_INT, 3, 03, MPI_COMM_WORLD);
         MPI_Send(A4, 4, MPI_INT, 4, 04, MPI_COMM_WORLD);
          
         t2 = MPI_Wtime();
         
          printf("\nvypocet casu (rank 0): %f\n", t2-t1);
          fflush(stdout);
              
         
     }
     else if(rank == 1)
     {
          t1 = MPI_Wtime();
           
          MPI_Recv(A1, 4, MPI_INT, 0, 01, MPI_COMM_WORLD, &status);
           
          Swap(C, A1);
          printf("\n1.Stlpec\n");
          PrintMatrix(C);  
           
          t2 = MPI_Wtime();
         
          printf("\nvypocet casu (rank 1): %f\n", t2-t1);
          fflush(stdout);
     }
     else if(rank == 2)
     {
          t1 = MPI_Wtime();
           
          MPI_Recv(A2, 4, MPI_INT, 0, 02, MPI_COMM_WORLD, &status);
          Swap(C, A2);
          printf("\n2.Stlpec\n");
          PrintMatrix(C);  
           
          t2 = MPI_Wtime();
         
          printf("\nvypocet casu (rank 2): %f\n", t2-t1);
          fflush(stdout);
     }
     else if(rank == 3)
     {
          t1 = MPI_Wtime();
           
          MPI_Recv(A3, 4, MPI_INT, 0, 03, MPI_COMM_WORLD, &status);
          Swap(C, A3);
          printf("\n3.Stlpec\n");
          PrintMatrix(C);  
           
          t2 = MPI_Wtime();
         
          printf("\nvypocet casu (rank 3): %f\n", t2-t1);
          fflush(stdout);
     }
     else if(rank == 4)
     {
          t1 = MPI_Wtime();
           
          MPI_Recv(A4, 4, MPI_INT, 0, 04, MPI_COMM_WORLD, &status);
          Swap(C, A4);
          printf("\n4.Stlpec\n");
          PrintMatrix(C);  
           
          t2 = MPI_Wtime();
         
          printf("\nvypocet casu (rank 4): %f\n", t2-t1);
          fflush(stdout);
     }
}   
   
  MPI_Finalize();
  return(0);
}
