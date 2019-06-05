#include <stdio.h>
#include <mpi.h>
 
void Matrix_Mult(int a1[][4], int a2[][4], int a3[][4])
{
   int i = 0;
   int j = 0;
   int k = 0;
    
       for(i = 0; i < 1; i++) 
       {
           for( j = 0; j < 4; j++)
           {
               for( k = 0; k < 4; k++) 
               {
                   a3[i][j] +=  a1[i][k] * a2[k][j];
               }
           }
       }
}
void PrintMatrix(int ar[][4])
{
    int i = 0;
    int j = 0;
    for(i = 0; i < 1; i++)
    {
        for( j = 0; j < 4; j++)
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
                    {5, 7, 8, 9}
                    },
      A2[1][4] = {
                    {1, 2, 3, 4}
                    },
      A3[1][4] = {
                    {9, 8, 8, 7}
                    },                            
      A4[1][4] = {
                    {1, 1, 2, 1}
                    },            
       B[4][4] = {
                  {1, 1, 1, 2},
                  {2, 3, 4, 5},
                  {7, 7, 8, 1},
                  {0, 4, 5, 0}
                  },
                   
       C[1][4] = {
                  {0, 0, 0, 0}
                  },
                   
       C_vzor[4][4] = {
                  {75, 118, 142, 53},
                  {26, 44, 53, 15},
                  {81, 117, 140, 66},
                  {17, 22, 26, 9}
                  };           
   
  MPI_Status status;
  MPI_Request request;
   
  MPI_Init(&argc, &argv);
  MPI_Comm_size(MPI_COMM_WORLD, &size);
   
   
  if(size == 5)
  {
           
     MPI_Comm_rank(MPI_COMM_WORLD, &rank);  
     MPI_Bcast(B, 16, MPI_INT, 0, MPI_COMM_WORLD);  
     MPI_Bcast(C, 4, MPI_INT, 0, MPI_COMM_WORLD);               
              
              
     if(rank == 0) 
     { 
             t1 = MPI_Wtime();
              
         printf("\n\nMatica A:\n");
         PrintMatrix(A1);PrintMatrix(A2);PrintMatrix(A3);PrintMatrix(A4);    
              
         printf("\n\nMatica B:\n");    
         PrintMatrix_N(B);
          
         printf("\n\nMatica C - vzorova:\n");    
         PrintMatrix_N(C_vzor);    
          
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
          Matrix_Mult(A1, B, C);
          printf("\n\n1.riadok:\n");
          PrintMatrix(C);
           
          t2 = MPI_Wtime();
         
          printf("\nvypocet casu (rank 1): %f\n", t2-t1);
          fflush(stdout);
     }
     else if(rank == 2)
     {
          t1 = MPI_Wtime();
           
          MPI_Recv(A2, 4, MPI_INT, 0, 02, MPI_COMM_WORLD, &status);
          Matrix_Mult(A2, B, C);
          printf("\n\n2.riadok:\n");
          PrintMatrix(C);
           
          t2 = MPI_Wtime();
         
          printf("\nvypocet casu (rank 2): %f\n", t2-t1);
          fflush(stdout);
     }
     else if(rank == 3)
     {
          t1 = MPI_Wtime();
           
          MPI_Recv(A3, 4, MPI_INT, 0, 03, MPI_COMM_WORLD, &status);
          Matrix_Mult(A3, B, C);
          printf("\n\n3.riadok:\n");
          PrintMatrix(C);
           
          t2 = MPI_Wtime();
         
          printf("\nvypocet casu (rank 3): %f\n", t2-t1);
          fflush(stdout);
     }
     else if(rank == 4)
     {
          t1 = MPI_Wtime();
           
          MPI_Recv(A4, 4, MPI_INT, 0, 04, MPI_COMM_WORLD, &status);
          Matrix_Mult(A4, B, C);
          printf("\n\n4.riadok:\n");
          PrintMatrix(C);
           
          t2 = MPI_Wtime();
         
          printf("\nvypocet casu (rank 4): %f\n", t2-t1);
          fflush(stdout);
     }
     
}
   
  MPI_Finalize();
  return(0);
}
