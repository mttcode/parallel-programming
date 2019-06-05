#include <stdio.h>
#include <mpi.h>
#define POCET 40
 
int main(int argc, char *argv[])
{
  int cisla[POCET];
  int cisla_1[POCET/10];
  int cisla_2[POCET/10];
  int cisla_3[POCET/10];
  int cisla_4[POCET/10];
   
  double t1, t2;
   
  int i, size, rank;
  int n, value_1, value_2;
  float rval;
  int max[4];
  int rozsah = 10;
   
  MPI_Status status;
  MPI_Request request;
   
  MPI_Init(&argc, &argv);
  MPI_Comm_size(MPI_COMM_WORLD, &size);
   
   
  if(size == 8)
  {
           
     MPI_Comm_rank(MPI_COMM_WORLD, &rank);   
      
     MPI_Bcast(&rozsah, 1, MPI_INT, 0, MPI_COMM_WORLD);                   
              
              
     if (rank == 0) 
     { 
        cisla[0]=2;cisla[1]=3;cisla[2]=8;cisla[3]=9;cisla[4]=45;cisla[5]=12;
        cisla[6]=66;cisla[7]=45;cisla[8]=58;cisla[9]=47;cisla[10]=32;cisla[11]=11;
        cisla[12]=12;cisla[13]=13;cisla[14]=17;cisla[15]=3;cisla[16]=1;cisla[17]=0;
        cisla[18]=19;cisla[19]=23;cisla[20]=32;cisla[21]=37;cisla[22]=68;cisla[23]=86;
        cisla[24]=96;cisla[25]=97;cisla[26]=91;cisla[27]=78;cisla[28]=56;cisla[29]=55;
        cisla[30]=52;cisla[31]=57;cisla[32]=41;cisla[33]=36;cisla[34]=27;cisla[35]=72;
        cisla[36]=1;cisla[37]=7;cisla[38]=99;cisla[39]=84;
         
        printf("\n\n\n Posielanie cisel");
        printf("\n -----------------------------");
        printf("\n 10 cisel z 0 -> 1");
        printf("\n 10 cisel z 0 -> 2");
        printf("\n 10 cisel z 0 -> 3");
        printf("\n 10 cisel z 0 -> 4");
        printf("\n 40 cisel z 0 -> 5,6,7");
     }
   
     MPI_Bcast(cisla, 40, MPI_INT, 0, MPI_COMM_WORLD);  
   
   
     if (rank == 0) 
     {
              t1 = MPI_Wtime();
            
        if(POCET %4 == 0)
        {  
             for(i = 0; i < 10; i++)
             {
                   cisla_1[i] = cisla[i];    
             }
             MPI_Send(cisla_1, 10, MPI_INT, 1, 01, MPI_COMM_WORLD);
              
              
             for(i = 10; i < 20; i++)
             {
                   cisla_2[i-10] = cisla[i]; 
             }
             MPI_Send(cisla_2, 10, MPI_INT, 2, 02, MPI_COMM_WORLD);
              
             for(i = 20; i < 30; i++)
             {
                   cisla_3[i-20] = cisla[i];
             }
             MPI_Send(cisla_3, 10, MPI_INT, 3, 03, MPI_COMM_WORLD);
              
              
             for(i = 30; i < 40; i++)
             {
                   cisla_4[i-30] = cisla[i];
             }
             MPI_Send(cisla_4, 10, MPI_INT, 4, 04, MPI_COMM_WORLD);
                       
        }
         
         
        printf("\n\n\n Vysledky");
        printf("\n -----------------------------");
         
         
         
        MPI_Recv(&value_1, 1, MPI_INT, 1, 10, MPI_COMM_WORLD, &status);
        max[0] = value_1;
        MPI_Recv(&value_1, 1, MPI_INT, 2, 20, MPI_COMM_WORLD, &status);
        max[1] = value_1;
        MPI_Recv(&value_1, 1, MPI_INT, 3, 30, MPI_COMM_WORLD, &status);
        max[2] = value_1;
        MPI_Recv(&value_1, 1, MPI_INT, 4, 40, MPI_COMM_WORLD, &status);
        max[3] = value_1;
         
        value_1 = 0; 
        for(n = 0; n < 4; n++) 
        {
          if(value_1 < max[n]) 
          {
            value_1 = max[n];
          }
        }  
                 
        printf("\n Maximum: %4d (4 casti)", value_1);
        MPI_Recv(&value_2, 1, MPI_INT, 5, 50, MPI_COMM_WORLD, &status);
        printf("\n Minimum: %4d", value_2);
        MPI_Recv(&value_2, 1, MPI_INT, 6, 60, MPI_COMM_WORLD, &status);
        printf("\n Sucet: %4d", value_2);
        MPI_Recv(&rval, 1, MPI_FLOAT, 7, 70, MPI_COMM_WORLD, &status);
        printf("\n Priemer: %f", rval);  
         
        t2 = MPI_Wtime();
         
          printf("\nvypocet casu (rank 0): %f\n", t2-t1);
          fflush(stdout);
           
     } 
      
     else if(rank == 1)
     {
          t1 = MPI_Wtime();
           
          MPI_Recv(cisla_1, 10, MPI_INT, 0, 01, MPI_COMM_WORLD, &status);
           
        value_1 = 0; 
        for(n = 0; n < rozsah; n++) 
        {
          if(value_1 < cisla_1[n]) 
          {
            value_1 = cisla_1[n];
          }
        } 
        MPI_Send(&value_1, 1, MPI_INT, 0, 10, MPI_COMM_WORLD); 
         
        t2 = MPI_Wtime();
         
          printf("vypocet casu (rank 1): %f\n", t2-t1);
          fflush(stdout);
     }
      
     else if(rank == 2)
     {
          t1 = MPI_Wtime();
           
          MPI_Recv(cisla_2, 10, MPI_INT, 0, 02, MPI_COMM_WORLD, &status);
           
        value_1 = 0; 
        for(n = 0; n < rozsah; n++) 
        {
          if(value_1 < cisla_2[n]) 
          {
            value_1 = cisla_2[n];
          }
        } 
        MPI_Send(&value_1, 1, MPI_INT, 0, 20, MPI_COMM_WORLD); 
         
        t2 = MPI_Wtime();
         
          printf("vypocet casu (rank 2): %f\n", t2-t1);
          fflush(stdout);
     }
      
     else if(rank == 3)
     {
          t1 = MPI_Wtime();
           
         MPI_Recv(cisla_3, 10, MPI_INT, 0, 03, MPI_COMM_WORLD, &status);
           
        value_1 = 0; 
        for(n = 0; n < rozsah; n++) 
        {
          if (value_1 < cisla_3[n]) 
          {
            value_1 = cisla_3[n];
          }
        } 
        MPI_Send(&value_1, 1, MPI_INT, 0, 30, MPI_COMM_WORLD); 
         
        t2 = MPI_Wtime();
         
          printf("vypocet casu (rank 3): %f\n", t2-t1);
          fflush(stdout);
               
     }
      
     else if(rank == 4)
     {
          t1 = MPI_Wtime();
           
        MPI_Recv(cisla_4, 10, MPI_INT, 0, 04, MPI_COMM_WORLD, &status);
           
        value_1 = 0; 
        for(n = 0; n < rozsah; n++) 
        {
          if(value_1 < cisla_4[n]) 
          {
            value_1 = cisla_4[n];
          }
        } 
        MPI_Send(&value_1, 1, MPI_INT, 0, 40, MPI_COMM_WORLD);  
         
        t2 = MPI_Wtime();
         
          printf("vypocet casu (rank 4): %f\n", t2-t1);
          fflush(stdout); 
     }
      
     else if(rank == 5)
     {
          t1 = MPI_Wtime();
           
        value_2 = 100; 
        for(n = 0; n < rozsah*4; n++) 
        {
          if(value_2 > cisla[n]) 
          {
            value_2 = cisla[n];
          }
        } 
        MPI_Send(&value_2, 1, MPI_INT, 0, 50, MPI_COMM_WORLD);   
         
        t2 = MPI_Wtime();
         
          printf("vypocet casu (rank 5): %f\n", t2-t1);
          fflush(stdout);  
     }
      
     else if(rank == 6)
     {
          t1 = MPI_Wtime();
           
        value_2 = 0; 
        for(n = 0; n < rozsah*4; n++) 
        {
          value_2 = value_2 + cisla[n]; 
        }  
        MPI_Send(&value_2, 1, MPI_INT, 0, 60, MPI_COMM_WORLD);     
         
        t2 = MPI_Wtime();
         
          printf("vypocet casu (rank 6): %f\n", t2-t1);
          fflush(stdout);
     }
      
     else if(rank == 7)
     {
          t1 = MPI_Wtime();
           
        value_2 = 0; 
        for(n = 0; n < rozsah*4; n++) 
        {
          value_2 = value_2 + cisla[n]; 
        }  
         
        rval = (float) value_2 / 40;
          
        MPI_Send(&rval, 1, MPI_FLOAT, 0, 70, MPI_COMM_WORLD);     
         
        t2 = MPI_Wtime();
         
          printf("vypocet casu (rank 7): %f\n", t2-t1);
          fflush(stdout);
     }
         
                 
  }
   
   
  MPI_Finalize();
  return(0);
}
