//Matrix multiplication kernel - thread specification

//we used only 1 block, execute on one SM
__global__ void MatrixMulKernel(float* Md, fload* Nd, fload* Pd, int Width)
{
  //2D thread ID
  int tx = threadIdx.x;
  int ty = threadIdex.y;
  //Pvalues stores the Pd elements that is computed by the thread
  fload Pvalue = 0;
  for(int k=0; k< Width; k++)
    {
      float Mdelement = Md[ty*Width+k];
      float Ndelement = Nd[k*Width+tx];
      Pvalue += Mdelement * Ndelement;
    }

  //Write the matrix to device memory each thread writes one element
  Pd[ty*Width+tx] = Pvalue;
}

/*
dim3 threadsPerBlock(8,8);
dim3 numBlocks(imageWidth/threadsPerBlock.x, imageheight/threadsPerBlock.y);
kernelFunc<<<numBlocks, threadsPerBlock>>>();
*/


//Break-up Pd into tiles
//Each Block calculates on tile
//Each thread calculates one element
//Block size equals tile size

//Setup the execution configuration

dim3 dimGrid(Width/TILE_WIDTH,Width/TILE_WIDTH );
dim3 dimBlock(TILE_WIDTH, TILE_WIDTH);

//Launch the device compassion threads
MatrixMulKerne<<<dimGrid, dimBlock>>>l( Md, Nd, Pd Width);

__global__ void MatrixMulKernel(float* Md, fload* Nd, fload* Pd, int Width)
{
  //Calculate the row index of the Pd element and M;
  int Row = blockIdx.y*TILE_WIDTH + threadIdx.y;
  //Calculate the column index of Pd and N
  int Col = blockIdex.x**TILE_WIDTH + threadIdx.x;
  fload Pvalue = 0;
  //Each thread computes one element of the block sub-matrix
  for(int k=0; k< Width; k++)
    Pvalue += Md[Row*Width+k] + Nd[k*Width+Col];
  Pd[Row*Width+Col] = Pvalue;
}





///////////////////////////////////////////////////////////////////////
//Using Shared Memory

//Setup the execution configuration
dim3 dimGrid(Width/TILE_WIDTH,Width/TILE_WIDTH );
dim3 dimBlock(TILE_WIDTH, TILE_WIDTH);
//Launch the device compassion threads
MatrixMulKerne<<<dimGrid, dimBlock>>>l( Md, Nd, Pd Width);
__global__ void MatrixMulKernel(float* Md, fload* Nd, fload* Pd, int Width){
  int Row = blockIdx.y*TILE_WIDTH + threadIdx.y;
  int Col = blockIdex.x*TILE_WIDTH + threadIdx.x;
  int tx = threadIdx.x, ty = threadIdx.y;
  __shared__ float a[TILE_WIDTH][TILE_WIDTH], b[TILE_WIDTH][TILE_WIDTH];
  fload Pvalue = 0;
  //Each thread computes one element of the block sub-matrix
  for(int k=0; k< gridDim.y; k++){
    a[tx][ty] = Md[Row*Width+k*TILE_WIDTH+ty];
    b[ty][tx] = Nd[Col+Width*(k*TILE_WIDTH + tx)];
    __syncthreads(); //sync all threads in a block;
    for(int kk=0; kk<TILE_WIDTH; kk++)
      Pvalue += a[kk][tx]*b[kk][ty];
    __syncthreads(); //avoid memory hazards;
  }
  Pd[Row*Width+Col] = Pvalue;
}


