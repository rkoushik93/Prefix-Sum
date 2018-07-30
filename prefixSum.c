#include<mpi.h>
#include<stdio.h>
#include<stdlib.h>
#include<time.h>

int main(int argc,char** argv){

        int numElem,destproc=0;
        numElem=atoi(argv[1]);

        int x[numElem],sum[numElem];
        int i,j,numprocs,myid,subSum;
        //clock_t startTime,endTime;
        double timeTaken,t1,t2,compStart,compEnd;

        MPI_Init(&argc,&argv);
        t1=MPI_Wtime();
        MPI_Comm_size(MPI_COMM_WORLD,&numprocs);
        MPI_Comm_rank(MPI_COMM_WORLD,&myid);

        /*startTime=clock();*/

        if(myid==0){
                printf("Number of elements = %d\n\n",numElem);
                for(i=0;i<numElem;i++){
                        x[i]=0;
                }
                for(i=0;i<numElem;i++){
                        x[i]=rand()%100;
                }
                printf("The randomly generated input sequence is : \n");
                for(i=0;i<numElem;i++){
                        printf("x[%d]=%d\n",i,x[i]);
                }

                compStart=MPI_Wtime();
                for(i=1;i<numprocs;i++){
                        MPI_Send(&x[0],(i+1)*numElem/numprocs,MPI_INT,i,0,MPI_COMM_WORLD);
                }
        }

        else{
                MPI_Recv(&x[0],(myid+1)*numElem/numprocs,MPI_INT,0,0,MPI_COMM_WORLD,MPI_STATUS_IGNORE);
        }

        for(i=0;i<numElem/numprocs;i++){
                subSum=0;
                for(j=0;j<((myid*numElem/numprocs)+i);j++){
                        subSum=subSum+x[j];
                }
                sum[i]=subSum+x[(myid*numElem/numprocs)+i];
        }

        if(myid!=0){
                MPI_Send(&sum[0],numElem/numprocs,MPI_INT,0,0,MPI_COMM_WORLD);
        }

        else if(myid==0){
                for(i=1;i<numprocs;i++){
                        MPI_Recv(&sum[i*numElem/numprocs],numElem/numprocs,MPI_INT,i,0,MPI_COMM_WORLD,MPI_STATUS_IGNORE);
                }

                printf("\n\nPrefix sum :\n");
                for(i=0;i<numElem;i++){
                        printf("sum[%d]=%d\n",i,sum[i]);
                }
                t2=MPI_Wtime();
                compEnd=MPI_Wtime();
                printf("\n\nTime taken for the computation = %f sec\n",compEnd-compStart);
                printf("\nTotal time taken (computed using MPI_Wtime() = %f sec\n",t2-t1);
        }

        MPI_Finalize();

        return 0;
}