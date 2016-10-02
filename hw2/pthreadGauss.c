#include<omp.h>
#include<stdio.h>
#include<stdlib.h>


void thdfunc(float* A[], int* k, int* n)
{

  int slice = (int)s;
  int start = s * (n/p);
  int end = (s+1) * (n/p);

  for(j=k+1; j<=n-1; j++)
    {
      A[k][j] = A[k][j] / A[k][k];
    }
}


void elimination(float* A[], int* k, int* n, float* b)
{
  for(i=k+1; i<=n-1; i++)
    {
      for(j=k+1; j<=n-1; j++)
	{
	  A[i][j] = A[i][j] - (A[i][k] * A[k][j]);
	}
      b[i] = b[i] - (A[i][k] * y[k]);
      A[i][k] = 0;
    }
}




int main()
{
  int i,j,k,n, tn;

  float A[20][20], b[10], x[10], y[10];
  
  int p = 16; //number of threads
  pthread_t threads[p];

  printf("\nEnter the order of matrix: ");
  scanf("%d",&n);
  
  printf("\nEnter the elements of augmented matrix row-wise:\n\n");
  for(i=0; i<n; i++)
    {
      for(j=0; j<n; j++)
        {
	  printf("A[%d][%d] : ", i,j);
	  scanf("%f",&A[i][j]);
        }
    }


  printf("\nEnter the elements of b matrix row-wise:\n\n");
  for(i=0; i<n; i++)
    {
      printf("b[%d]: ", i);
      scanf("%f",&b[i]);
    }

  //Generate Upper Triangular Matrix
  for(k=0; k<=n-1; k++)
    {

      for(tn=0; tn<p;tn++)
	{
	  pthread_create(&threads[tn],NULL, division, (float **)A , &k, &n );
	}

      for(tn=0; tn<p; tn++)
	{
	  pthread_join(threads[tn],NULL);
	}


      y[k] = b[k]/A[k][k];
      A[k][k] = 1;

      for(tn=0; tn<p;tn++)
	{
	  pthread_create(&threads[tn],NULL, elimination, (float **)A, &k, &n, b);
	}

      for(tn=0; tn<p; tn++)
	{
	  pthread_join(threads[tn],NULL);
	}
      

    }
  
 

  // Backward Substitution
  for(k=n-1; k>=0; k--)
    {
      x[k] = y[k];
      for(i=k-1;i>=0;i--)
        {
	  y[i] = y[i] - A[i][k] * x[k];
        }
    }


  printf("\nThe solution is: \n");

  for(i=0; i<n; i++)
    {
      printf("\nx%d=%f\t",i,x[i]);
    }

  printf("\n");

  return(0);
}
