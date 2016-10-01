#include<stdio.h>
int main()
{
  int i,j,k,n;
 
  float A[20][20], c, b[10], sum=0.0, ratio;
  
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
	      b[j] = b[j] - (ratio * b[i]);
	    }
	}
    }
  
  
  // Backward Substitution
  for(i=n-1; i>=0; i--)
    {
      sum=0;
      for(j=i+1; j<n; j++)
        {
	  sum=sum+A[i][j]*b[j];
        }
      b[i]=(A[i][n+1]-sum)/A[i][i];
    }

  printf("\nThe solution is: \n");

  for(i=1; i<=n; i++)
    {
      printf("\nx%d=%f\t",i,x[i]);
    }

  return(0);
}
