#include<omp.h>
#include<stdio.h>
#include<stdlib.h>

#define NUM_THREADS 16

int main()
{
  int i,j,k,n;
 
  float A[20][20], b[10], x[10], y[10];
  
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

      //      #pragma omp parallel num_threads(NUM_THREADS)
      #pragma omp parallel for
      for(j=k+1; j<=n-1; j++) 
        {
	  A[k][j] = A[k][j] / A[k][k];
	}
      y[k] = b[k]/A[k][k];
      A[k][k] = 1;

      //#pragma omp parallel num_threads(NUM_THREADS)
      #pragma omp parallel for
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
