#include<stdio.h>
#include<stdlib.h>
#include<time.h>
#include<mpi.h>

int main(int argc, char **argv)
{
  MPI_Init(&argc, &argv);

  int i,j,k;
  int map[500];
  double A[500][500],b[500],c[500],x[500],sum=0.0;
  double range=1.0;
  int n=500;
  int rank, nprocs;
  //  clock_t begin1, end1, begin2, end2;
  double begin1, end1;
  double begin2, end2;
  MPI_Status status;

  MPI_Comm_rank(MPI_COMM_WORLD, &rank);   // get current process id 
  MPI_Comm_size(MPI_COMM_WORLD, &nprocs); // get number of processes

  if (rank==0)
    {
      for (i=0; i<n; i++)
        {
	  for (j=0; j<n; j++)
	    A[i][j]=range*(1.0-2.0*(double)rand()/RAND_MAX);
	  b[i]=range*(1.0-2.0*(double)rand()/RAND_MAX);
        }
      printf("\n Matrix A (generated randomly):\n");
      for (i=0; i<n; i++)
        {
	  for (j=0; j<n; j++)
	    printf("%9.6lf ",A[i][j]);
	  printf("\n");
        }
      printf("\n Vector b (generated randomly):\n");
      for (i=0; i<n; i++)
	printf("%9.6lf ",b[i]);
      printf("\n\n");
    }



  //begin1 =clock();
  begin1 = MPI_Wtime();

  MPI_Bcast (&A[0][0],500*500,MPI_DOUBLE,0,MPI_COMM_WORLD);
  MPI_Bcast (b,n,MPI_DOUBLE,0,MPI_COMM_WORLD);    

  for(i=0; i<n; i++)
    {
      map[i]= i % nprocs;
    } 

  for(k=0;k<n;k++)
    {
      //printf("%d : i=%d map[i]= %d\n",rank,i,map[i]);
      MPI_Bcast (&A[k][k],n-k,MPI_DOUBLE,map[k],MPI_COMM_WORLD);
      MPI_Bcast (&b[k],1,MPI_DOUBLE,map[k],MPI_COMM_WORLD);
      for(i= k+1; i<n; i++) 
        {
	  if(map[i] == rank)
            {
	      c[i]=A[i][k]/A[k][k];
            }
        }               
      for(i= k+1; i<n; i++) 
        {       
	  if(map[i] == rank)
            {
	      for(j=0;j<n;j++)
                {
		  A[i][j]=A[i][j]-( c[i]*A[k][j] );
                }
	      b[i]=b[i]-(c[i]*b[k]);
            }
        }
    }
  //  end1 = clock();
  end1 = MPI_Wtime();

  //  begin2 =clock();
  begin2 = MPI_Wtime();

  if (rank==0)
    { 
      x[n-1]=b[n-1]/A[n-1][n-1];
      for(i=n-2;i>=0;i--)
        {
	  sum=0;

	  for(j=i+1;j<n;j++)
            {
	      sum=sum+A[i][j]*x[j];
            }
	  x[i]=(b[i]-sum)/A[i][i];
        }

      //      end2 = clock();
      end2 = MPI_Wtime();
    }

  if (rank==0)
    { 
      printf("\nThe solution is:");
      for(i=0;i<n;i++)
        {
	  printf("\nx%d=%f\t",i,x[i]);

        }

      printf("\n\nLU decomposition time: %f", (double)(end1 - begin1));
      printf("\n");
      //printf("\nBack substitution time: %f\n", (double)(end2 - begin2));
    }
  MPI_Finalize();
  return(0);
}
