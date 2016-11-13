Thread - Smallest Unit of Task that CUDA handles

Major Design Decision - How to split all tasks into threads?

Block = Group of threads that can communicate and sync with each other.
A Block runs on a single multiprocessor.

If you have a bunch of threads that periodically need data from other threads, they have to be put into the same block.

Only way one block can talk to another block is to finish the kernel execution at one point so that all the blocks write out their data to global memory, and at the next kernel invocation all the blocks can read data from all other blocks. 

different GPUs have differing amounts of multi-processors in them but CUDA programs need to run smoothly on any and all of them.
The only way to do that is to introduce the concept of a block so that on GPUs with only a few multi-processors, multiple blocks are run serially (that is, one after another) whereas on a powerful GPU, many blocks can be run in parallel. 

The other reason is probably due to scale of circuitry. Threads in a block can do a number of things threads in different blocks can't do. 
For example, threads in a block can access very fast shared memory simultaneously. In order for multiple threads to access same shared memory simultanously, there mustbe a certain circuitry overhead which becomes too large if the block becomes too large.

Grid = collection of blocks. It enables multiple blocks to execute in one kernel invocation.
So if you have a big parallel problem, you break it into blocks and arrange them into a grid.

Taking your 5x5 matrix multiply problem, if I were you, I would assign a thread to multiplying one row of the left matrix with one column of the right matrix. There will be 25 threads because there are 25 combinations of rows and columns. Your block will contain 25 threads.
If you only want to calculate one 5x5 matrix, you will need only one block. If you want to multiply a 100 pairs of 5x5 matrices,
you should make a grid consisting of 100 blocks, each block containing 25 threads.
