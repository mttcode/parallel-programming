#include <mpi.h>
#include <stdio.h>
#include <malloc.h>
#include <stdlib.h>
#include <string.h>
#define HASHTABLESIZE 10
#define WORDSIZE 6
#define STEP 3;

int doHash(char* word)
{
    int i,hash = 0;
    for(i = 0; i < strlen(word); i++)
        hash += (int)word[i];
    return hash % HASHTABLESIZE;
}

int main(argc, argv)
int argc;
char *argv[];
{
    int size, rank;
    int hashtable[HASHTABLESIZE], *dataChunk;
    char word[WORDSIZE];
    int i, full,placed,runs, hash, position, *sendcounts, *offsets, 
    chunkSize, zvysok, bounds[2], retbuf[2];
    MPI_Status status;
    /* Initialize MPI  */
    MPI_Init(&argc, &argv);
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    chunkSize = HASHTABLESIZE / size;
    zvysok = HASHTABLESIZE % size;
    if (rank == 0)
    {
             //hashtabulka, 0=volne,1=obsadene
              
        hashtable[0] = 1;   hashtable[1] = 1; 
        hashtable[2] = 1;   hashtable[3] = 1;
        hashtable[4] = 0;   hashtable[5] = 1;
        hashtable[6] = 0;   hashtable[7] = 0;
        hashtable[8] = 0;   hashtable[9] = 1;
        strcpy(word, "hasho"); //hashovany retazec
        for(i = 0; i < HASHTABLESIZE; i++)
        {
            if(hashtable[i] == 0)
                printf("%d: -\n", i);
            else
                printf("%d: obsadene\n", i);
        }
        sendcounts = (int*) malloc(size*sizeof(int));
        offsets = (int*) malloc(size*sizeof(int));
         
        for(i = 0; i < size; i++)
        {
            if(i < zvysok)
            {
                sendcounts[i] = chunkSize + 1;
                offsets[i] = i * (chunkSize + 1);
            }
            else
            {
                sendcounts[i] = chunkSize;
                offsets[i] = (zvysok*(chunkSize+1)) + ((i-zvysok) * chunkSize);
            }
        }
    }
     
    MPI_Bcast(word,WORDSIZE,MPI_CHAR,0,MPI_COMM_WORLD);
    chunkSize = rank < zvysok ? chunkSize+1 : chunkSize;
    dataChunk = (int*) malloc(sizeof(int)* chunkSize);
     
    /*
    int MPI_Scatterv(void* sendbuf, int *sendcounts,
          int *displs, MPI_Datatype sendtype, void* recvbuf,
          int recvcount, MPI_Datatype recvtype, int root,
          MPI_Comm comm);
          */
           
           
    MPI_Scatterv(hashtable, sendcounts, offsets, MPI_INT, dataChunk, chunkSize, MPI_INT, 0, MPI_COMM_WORLD);
    if(rank < zvysok)
        bounds[0] = rank * (chunkSize); //chunksize je uz o jedna vacsi
    else
        bounds[0] = (zvysok*(chunkSize+1)) + ((rank-zvysok) * chunkSize);
         
    bounds[1] = bounds[0] + chunkSize;
    //check ci nema svoj segment plny
    full = 1;    //na zac. akoze plny, ak najdem neobsadene - set 0(=neplny)
     
    for(i = 0; i < chunkSize; i++)
    {
        if(dataChunk[i] == 0)
        {
            full = 0;
            break;
        }
    }
    if(full == 0) //nie je plny - najdi miesto pre novy prvok
    {
        hash = doHash(word);
        runs = 0;             //pocet prechodov hashtabulkou
        placed = 0;           //umiestnene do tab?
         
        while(placed == 0 && runs < chunkSize)
        {
            if(hash >= bounds[0] && hash < bounds[1])
            {
                i = hash-bounds[0]; //local index
                if(dataChunk[i] == 0)
                    placed = 1;
            }
             
            if(placed == 0)//nema miesto
            {
                do
                {
                    hash += STEP;
                    if(hash >= HASHTABLESIZE)
                    {
                        runs++;
                        hash -= HASHTABLESIZE;
                    }
                     
                } while (hash<bounds[0] || hash>=bounds[1]);
                 
            }
        }
        retbuf[0] = runs;  //pocet prechodov - da/neda sa ulozit ?
        retbuf[1] = hash; //pozicia
    }
    else //full
    {
        retbuf[0] = -1; //neda sa ulozit
        retbuf[1] = -1;
    }
    if(rank == 0)
    {
        i=STEP;
        printf("\nhash: %d, step: %d\n", doHash(word), i);
        printf("master %d: runs=%d pos=%d\n",rank,retbuf[0],retbuf[1]);
         
        for(i = 1; i < size; i++)
        {
            MPI_Recv(bounds, 2, MPI_INT, i, 1, MPI_COMM_WORLD, &status);
            printf("slave %d: runs=%d pos=%d\n", i, bounds[0], bounds[1]);
             
            if(bounds[0] != -1 && (bounds[0]<retbuf[0] || (bounds[0]==retbuf[0] && 
            bounds[1] < retbuf[1]) || retbuf[0] == -1)) //compare runs
            {
                retbuf[0] = bounds[0];
                retbuf[1] = bounds[1];
            }
        }
         
        //final result
        if(retbuf[0] == -1)
            printf("neda sa ulozit\n");
        else
            printf("ulozene na: %d\n", retbuf[1]);
    }
    else
    {
        MPI_Send(retbuf,2,MPI_INT,0,1,MPI_COMM_WORLD);
    }
    MPI_Finalize();
    return (EXIT_SUCCESS);
}
