#include <mpi.h>
#include <stdio.h>
#include <malloc.h>
#include <stdlib.h>
#include <string.h>
 
#define HAYSTACKSIZE 12
#define WORDSIZE 10
 
int main(argc, argv)
int argc;
char *argv[];
{
    int size, rank;
    char haystack[HAYSTACKSIZE][WORDSIZE], needle[WORDSIZE], (*dataChunk)[WORDSIZE];
    int i, *sendcounts, *offsets, chunkSize, zvysok, *retbuf, *matches;
    MPI_Datatype word;
 
    /* Initialize MPI  */
    MPI_Init(&argc, &argv);
    MPI_Type_contiguous(WORDSIZE, MPI_CHAR, &word);  //vymedzenie noveho typu
    MPI_Type_commit(&word);       //zaznamenanie noveho typu
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
 
    chunkSize = HAYSTACKSIZE / size;     //pocet slov na procesor
    zvysok = HAYSTACKSIZE % size;
 
    if (rank == 0)
    {
             //tabulka slov
              
        strcpy(haystack[0], "januar");    strcpy(haystack[1], "jun");       strcpy(haystack[10], "november");
        strcpy(haystack[2], "februar");   strcpy(haystack[3], "jul");       strcpy(haystack[11], "december");
        strcpy(haystack[4], "marec");     strcpy(haystack[5], "august");
        strcpy(haystack[6], "april");     strcpy(haystack[7], "september");
        strcpy(haystack[8], "maj");       strcpy(haystack[9], "oktober");
 
        printf("\nTabulka slov:\n\n");
         
        for(i = 0; i < HAYSTACKSIZE; i++)
        {
             printf("[%d] %s\n", i, haystack[i]);
        }
 
        printf("\n");
 
        strcpy(needle, "ma");    //hladany retazec
 
        matches = (int*) malloc(HAYSTACKSIZE * sizeof(int));
 
        sendcounts = (int*) malloc(size*sizeof(int));
        offsets = (int*) malloc(size*sizeof(int));
         
        for(i = 0; i < size; i++)
        {
            if(i < zvysok)
            {
                sendcounts[i] = chunkSize + 1;  // pocet slov pre procesor
                offsets[i] = i * (chunkSize + 1); 
            }
            else
            {
                sendcounts[i] = chunkSize;
                offsets[i] = (zvysok*(chunkSize+1)) + ((i-zvysok) * chunkSize);
            }
        }
    }
 
    MPI_Bcast(needle,WORDSIZE,MPI_CHAR,0,MPI_COMM_WORLD);
 
    chunkSize = rank < zvysok ? chunkSize+1:chunkSize;
    dataChunk = (char(*)[WORDSIZE]) malloc(sizeof(char) * WORDSIZE * chunkSize);
     
    /*
    int MPI_Scatterv(void* sendbuf, int *sendcounts,
          int *displs, MPI_Datatype sendtype, void* recvbuf,
          int recvcount, MPI_Datatype recvtype, int root,
          MPI_Comm comm);
          */
           
    MPI_Scatterv(haystack, sendcounts, offsets, word, dataChunk, chunkSize, word, 0, MPI_COMM_WORLD);
 
    retbuf = (int*) malloc(chunkSize*sizeof(int));
     
    for(i = 0; i < chunkSize; i++)
    {
        printf("rank %d: %s\n", rank, dataChunk[i]);
        retbuf[i] = 0;
        if(strstr(dataChunk[i], needle) != NULL)
            retbuf[i] = 1;
    }
 
    /*
    int MPI_Gatherv(void* sendbuf, int sendcount, MPI_Datatype sendtype,
          void* recvbuf, int recvcounts, int *displs, MPI_Datatype recvtype,
          int root, MPI_Comm comm);
          */
           
    MPI_Gatherv(retbuf, chunkSize, MPI_INT, matches, sendcounts, offsets, MPI_INT, 0, MPI_COMM_WORLD);
 
    if(rank == 0)
    {
        printf("\nVysledok vyhladavania pre retazec '%s':\n", needle);
         
        for(i = 0; i < HAYSTACKSIZE; i++)
        {
            if(matches[i] == 1)
                printf("[%d] %s\n", i, haystack[i]);
        }
    }
 
    MPI_Finalize();
    return (EXIT_SUCCESS);
}
