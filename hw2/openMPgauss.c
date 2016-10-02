#include<omp.h>
#include<stdio.h>
#include<stdlib.h>


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
  for(k=0; k<n-1; k++)
    {
      #pragma omp parallel num_threads(8) firstprivate(i)
      #pragma omp parallel for
      for(j=k+1; j<n-1; j++) 
        {
	  y[i] = b[i]/A[i][i];
	}	  for(k=i+1; k<n-1; k++)
	    {
	      A[j][k] = A[j][k] - (A[j][i] * A[i][k]);
	    }
	  b[j] = b[j] - (A[j][i] * y[i]);
	  A[j][i] = 0;
	}
    }
  
  
  //  printMatrix(A,n);

  // Backward Substitution
  for(i=n-1; i>=0; i--)
    {
      x[i] = y[i];
      for(j=i-1;j>=0;j--)
        {
	  y[j] = y[j] - A[j][i] * x[i];
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
