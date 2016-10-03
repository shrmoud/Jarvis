#Parallelism

Parallelism between iterations of the while loop is unlikely to be feasible since we cannot
choose the next pivot until the operations using the previous one have been applied. This
means we must seek parallelism within each iteration. This means that we want to parallelize
the operation of zeroing out the elements beneath the pivot in the current column. The
zeroing out of each element (and the associated updates to the row elements to right of it) are
all independent. Thus, when doing the first pivot (element A[1,1]) we will be able to operate
on all the remaining rows A[2..m,*] in parallel. Since each row contains n elements this
results in (m-1)*n independent operations to parallelize. Unfortunately, as more and more of
the elimination is performed, the number of concurrent operations decreases drastically. (At
the end, we will have a single operation.) Further, the operations performed on each element
are simple (a multiply and subtraction). This suggests that the speedup from parallelism will
likely be limited, especially for small matrices and/or many threads. For very large m and n
we may see reasonable speedups but since the operations are small, any synchronization
overhead will limit benefits. To address synchronization overhead we may want to group
potentially parallel operations into larger groups and execute them serially. Further, only the
first part of the elimination process is likely to benefit from parallelization. The latter parts
may actually experience slow down. Aside: Some of these problems might benefit from the
if clause on a parallel for and/or from dynamic scheduling but neither of these is expected in
the solution since we had not discussed them when the assignment was given.