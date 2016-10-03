#include<pthread.h>
#include<stdio.h>
#include<stdlib.h>

#define NUM_THREADS 16
int n;
//int p = NUM_THREADS;

volatile float A[20][20], b[10], x[10];

struct s1 {
  int norm;
  int id;
};

void* thrdfunc(void *p);


int main()
{
  int i,j,k;
 
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



  for (k = 0; k < n-1; k++) 
    {
      pthread_t threads[n];

      struct s1* para = malloc(2 * sizeof(struct s1));
      for (i = 0; i < p; i++) 
	{
	  para[i].norm = k;
	  para[i].id = i;
	  
	  pthread_create(&threads[i], NULL, thrdfunc, (void*) &para[i]);
	}

      for (j = 0; j < p; j++) 
	{
	  pthread_join(threads[j], NULL);
	}

      free(para);
    }

  int row, col;


  /* Back substitution */
  for (row = n - 1; row >= 0; row--) {
    x[row] = b[row];
    for (col = n-1; col > row; col--) {
      x[row] -= A[row][col] * x[col];
    }
    x[row] /= A[row][row];
  }


  printf("\nThe solution is: \n");

  for(i=0; i<n; i++)
    {
      printf("\nx%d=%f\t",i,x[i]);
    }

  printf("\n");

  return(0);
}



void* thrdfunc(void *s)
{ 
  struct s1* myStruct = (struct s1*) s;
  int norm = myStruct->norm;
  int id = myStruct->id;
  int i;
  for(i=norm+id;i<n;i=i+NUM_THREADS) {
    int col; 
    float multiplier;
    multiplier = A[i][norm] / A[norm][norm];
    for (col = norm; col < n; col++) {
      A[i][col] -= A[norm][col] * multiplier;
    }
    b[i] -= b[norm] * multiplier;
  }
  pthread_exit(0);
}
