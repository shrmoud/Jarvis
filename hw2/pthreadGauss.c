#include<pthread.h>
#include<stdio.h>
#include<stdlib.h>

#define NUM_THREADS 16;
int num;


pthread_barrier_t phase_barrier, row_barrier;

// following are function declarations

void* thrdfunc(void* p);

// function declaration

void* thrdfunc(void* p)
{

  float* A = p;
  int thread_number = pthread_self();
  int k,j,i;

  for(k=0; k<=num-1;k++)
    {

      pthread_barrier_wait(&row_barrier);

      for(j = (k+1+thread_number); j<=num-1 ; j=(j+NUM_THREADS))
	{
	  A[k][j] = (A[k][j] / A[k][k]);
	 
	  for(i=k+1; i<=num-1; i++)
	    {
	      A[i][j] = (A[i][j] - (A[i][k] * A[k][j]));
	    }
	}
    }
  
  pthread_barrier_wait(&phase_barrier);
}





int main()
{
  int i,j,k,n, tn;

  float A[20][20], b[10], x[10], y[10];
  
  int p = NUM_THREADS; //number of threads
  pthread_t threads[p];

  pthread_barrier_init(&phase_barrier, NULL, p); 
  pthread_barrier_init(&row_barrier,NULL,p);


  printf("\nEnter the order of matrix: ");
  scanf("%d",&n);
  
  num = n;

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

  for(tn=0; tn<p;tn++)
    {
      pthread_create(&threads[tn],NULL,thrdfunc, (void* )A);
    }
  
  for(tn=0; tn<p; tn++)
    {
      pthread_join(threads[tn],NULL);
    }
  
  pthread_barrier_wait(&phase_barrier);
  
  for(k=0; k<=n-1;k++)
    {
      
      y[k] = b[k]/A[k][k];
      A[k][k] = 1;
    
      for(i=k+1; i<=n-1;i++)
	{
	  b[i] = b[i] - A[i][k] * y[k];
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
