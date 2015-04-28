/**
 *  You need to isntall libXbgi in order to run this program
 *  http://libxbgi.sourceforge.net/
 *  See Makefile for more information
**/

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include <math.h>
#include <mpi.h>
#include "graphics.h"
#include <X11/Xlib.h>

#define NI 640       /* array sizes */
#define NJ 480
#define NSTEPS 100    /* number of time steps */
#define NPROCS 4     /* number of time steps */
#define NTHREADS 4
#define MAXRAND 1000


void calculate_iteration();
void* print_iteration(void*);

struct Game {
  int start;
  int end;
  int old[NI + 2][NJ + 2];
  int new[NI + 2][NJ + 2];
} game;

int main(int argc, char *argv[]) {
  XInitThreads();

  pthread_t thread_id[NTHREADS];
  int* from_to;

  int i, j, n, r;

  int q, comm_sz, my_rank;
  my_rank = 0;

  MPI_Init(NULL, NULL);
  MPI_Comm_size(MPI_COMM_WORLD, &comm_sz);
  MPI_Comm_rank(MPI_COMM_WORLD, &my_rank);

  if (my_rank == 0) {

    /* Initialize graphics */
    int gd, gm;
    gd = X11;
    gm = X11_640x480;
    initgraph (&gd, &gm, "");
    setbkcolor(BLACK);
    cleardevice();

    /* Initialize random */
    srand(time(NULL));

    /*  initialize elements of old to 0 or 1 */
    for (i = 1; i <= NI; i++){
      for (j = 1; j <= NJ; j++){
        r = rand() % MAXRAND;
        if (r < 100){
          game.old[i][j] = 1;
        } else {
          game.old[i][j] = 0;
        }
      }
    }

    /*  ITERATION */
    for (n = 0; n < NSTEPS; n++){

      /* corner boundary conditions */
      game.old[0][0] = game.old[NI][NJ];
      game.old[0][NJ + 1] = game.old[NI][1];
      game.old[NI + 1][NJ + 1] = game.old[1][1];
      game.old[NI + 1][0] = game.old[1][NJ];

      /* left-right boundary conditions */
      for(i = 1; i <= NI; i++){
        game.old[i][0] = game.old[i][NJ];
        game.old[i][NJ + 1] = game.old[i][1];
      }

      /* top-bottom boundary conditions */
      for(j = 1; j <= NJ; j++){
        game.old[0][j] = game.old[NI][j];
        game.old[NI + 1][j] = game.old[1][j];
      }

      /* calculate iteration -- ONLY SLAVES */
      //for (q = 1; q < comm_sz; q++) {
      //  MPI_Send(&game,1,game_type,...)
      //}
      calculate_iteration(1, NI);

      // THESE SHOULD BE AWESOME WITH GPU
      for(i = 0; i < NTHREADS; i++) {
        from_to = malloc(2 * sizeof(int));
        from_to[0] = (i * (NI / NTHREADS)) + 1;
        from_to[1] = (i + 1) * (NI / NTHREADS);
        pthread_create(
          &thread_id[i],
          NULL,
          print_iteration,
          (void *) from_to
        );
      }
      for(j = 0; j < NTHREADS; ++j)
        pthread_join(thread_id[j], NULL);

      /* copy new state into old state */
      for(i=1; i<=NI; i++){
        for(j=1; j<=NJ; j++){
          game.old[i][j] = game.new[i][j];
        }
      }

      // struct timespec tim, tim2;
      // tim.tv_sec = 1;
      // tim.tv_nsec = 100000000L;
      // nanosleep(&tim , &tim2);
    }
    if (getch() == KEY_ESC) {
      closegraph();
      return 0;
    }
  } else {
    //MPI_Recv(&game,1,game_type,...)
    //calculate_iteration(game.start, game.end);
  }

  MPI_Finalize();
  return 0;
}

void calculate_iteration(int from, int to) {
  int i, j, im, ip, jm, jp, nsum;
  for(i = from; i <= to; i++){
    for(j = 1; j <= NJ; j++){
      im = i - 1;
      ip = i + 1;
      jm = j - 1;
      jp = j + 1;

      nsum = game.old[im][jp] +
             game.old[i][jp] +
             game.old[ip][jp] +
             game.old[im][j] +
             game.old[ip][j] +
             game.old[im][jm] +
             game.old[i][jm] +
             game.old[ip][jm];

      switch(nsum){
        case 3:
          game.new[i][j] = 1;
        break;
        case 2:
          game.new[i][j] = game.old[i][j];
        break;
        default:
          game.new[i][j] = 0;
      }
    }
  }
}


void* print_iteration(void* data){
  int* from_to = (int*) data;
  int i, j;
  for(j = 1; j <= NJ; j++){
    for(i = from_to[0] + 1; i <= from_to[1] + 1; i++){
      if (game.old[i][j]) {
        putpixel(i, j, WHITE);
      } else {
        putpixel(i, j, BLACK);
      }
    }
  }
}
