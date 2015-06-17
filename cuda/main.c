#include <cuda.h>

#define N  512

int main( void ) {
  int *a , *b , *c ;                   
  // host copies of a, b, c
  int * dev_a , * dev_b , * dev_c ;       
  // device copies of a, b, c
  int size = N * sizeof ( int);     
  // we need space for 512 integers
  // allocate device copies of a, b, c
  cudaMalloc ( ( void **)& dev_a , size );
  cudaMalloc ( ( void **)& dev_b , size );
  cudaMalloc ( ( void **)& dev_c , size );
  a = ( int *) malloc ( size ); 
  b = ( int *) malloc ( size );
  c = ( int *) malloc ( size );
  random_ints ( a, N ); 
  random_ints ( b, N );
  // copy inputs to device
  cudaMemcpy ( dev_a , a, size, cudaMemcpyHostToDevice);
  cudaMemcpy ( dev_b , b, size, cudaMemcpyHostToDevice);
  // launch add() kernel with N parallel blocks
  add<<< N , 1 >>>( dev_a , dev_b , dev_c);
  // copy device result back to host copy of c
  cudaMemcpy ( c, dev_c , size, cudaMemcpyDeviceToHost);
  free( a ); free( b ); free( c );
  cudaFree ( dev_a);
  cudaFree ( dev_b);
  cudaFree ( dev_c);
  return 0;
}
