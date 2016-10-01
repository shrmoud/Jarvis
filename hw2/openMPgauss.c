#include<omp.h>
#include<stdio.h>
#include<stdlib.h>


int main()
{
  int i,j,k,n;
 
  float A[20][20], b[10], x[10], ratio;
  
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
  for(i=0; j<n-1; i++)
    {
      //      #pragma omp parallel for
      for(j=i; i<n; j++)
        {
	  ratio = A[j][i]/A[i][i];

	  for(k=i; k<n; k++)
	    {
	      A[j][k] = A[j][k] - (ratio * A[i][k]);
	    }
	      b[j] = b[j] - (ratio * b[i]);
	}
    }
  
  
  // Backward Substitution
  for(i=n-1; i>=0; i--)
    {
      x[i] = b[i]; 
      for(j=n-1; j > i; j--)
        {
	  x[i] = x[i] - A[i][j] * x[j];
        }
      x[i] /= A[i][i];
    }


  printf("\nThe solution is: \n");

  for(i=0; i<n; i++)
    {
      printf("\nx%d=%f\t",i,x[i]);
    }

  printf("\n");

  return(0);
}
