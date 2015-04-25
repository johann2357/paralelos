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
#include "graphics.h"

#define NI 500       /* array sizes */
#define NJ 500
#define NSTEPS 10    /* number of time steps */
#define MAXRAND 1111

void calculate_iteration(int*** old, int*** new);
void print_iteration(int*** old);

int main(int argc, char *argv[]) {

  /* Initialize random */
  srand(time(NULL));

  /* Initialize graphics */
  int gd = DETECT, gm;
  initgraph(&gd, &gm, "");
  setbkcolor(BLACK);
  cleardevice();

  int i, j, n, ni, nj;
  int **old, **new;
  int r;

  ni = NI + 2;  /* add 2 for left and right ghost cells */
  nj = NJ + 2;  /* add 2 for top and bottom ghost cells */

  old = malloc(ni * sizeof(int*));
  new = malloc(ni * sizeof(int*));

  for (i = 0; i < ni; i++){
    old[i] = malloc(nj * sizeof(int));
    new[i] = malloc(nj * sizeof(int));
  }

  /*  initialize elements of old to 0 or 1 */
  for (i = 1; i <= NI; i++){
    for (j = 1; j <= NJ; j++){
      r = rand() % MAXRAND;
      if (r < 1){
	old[i][j] = 1;
      } else {
	old[i][j] = 0;
      }
    }
  }


  /*  ITERATION */
  for (n = 0; n < NSTEPS; n++){

    /* corner boundary conditions */
    old[0][0] = old[NI][NJ];
    old[0][NJ + 1] = old[NI][1];
    old[NI + 1][NJ + 1] = old[1][1];
    old[NI + 1][0] = old[1][NJ];

    /* left-right boundary conditions */
    for(i = 1; i <= NI; i++){
      old[i][0] = old[i][NJ];
      old[i][NJ + 1] = old[i][1];
    }

    /* top-bottom boundary conditions */
    for(j = 1; j <= NJ; j++){
      old[0][j] = old[NI][j];
      old[NI + 1][j] = old[1][j];
    }

    /* calculate and print iteration */
    calculate_iteration(&old, &new);
    if (getch() == KEY_ESC) {
      return 0;
    }
    /* copy new state into old state */
    for(i=1; i<=NI; i++){
      for(j=1; j<=NJ; j++){
	old[i][j] = new[i][j];
      }
    }

  }
  closegraph();
  return 0;
}

void calculate_iteration(int*** old, int*** new) {
  int i, j, im, ip, jm, jp, nsum;
  for(i = 1; i <= NI; i++){
    for(j = 1; j <= NJ; j++){
      im = i - 1;
      ip = i + 1;
      jm = j - 1;
      jp = j + 1;

      nsum = (*old)[im][jp] +
             (*old)[i][jp] +
             (*old)[ip][jp] +
             (*old)[im][j] +
             (*old)[ip][j] +
             (*old)[im][jm] +
             (*old)[i][jm] +
             (*old)[ip][jm];

      switch(nsum){
        case 3 || 2:
          (*new)[i][j] = 1;
        break;
        default:
          (*new)[i][j] = 0;
      }
    }
  }
  print_iteration(old);
}


void print_iteration(int*** old){
  int i, j;
  for(i = 1; i <= NI; i++){
    for(j = 1; j <= NJ; j++){
      if ((*old)[i][j]) {
        putpixel(i, j, WHITE);
      } else {
        putpixel(i, j, BLACK);
      }
    }
  }
}
