#include <stdio.h>
#include <stdlib.h>
#include "table.h"

void printPTEs() {

  printf("Page\t| V,M,R,Frame\n");

  int i;
  for ( i = 0; i < num_pages; i++) {
    printf("%d\t| ", i);
    if( (PTEs[i] >> 31) == 0) {
      printf("0,-,-,-\n");
    } else {
      printBitShift(PTEs[i], MOST_SIG_BIT );
      printBitShift(PTEs[i], MOST_SIG_BIT-1 );
      printBitShift(PTEs[i], MOST_SIG_BIT-2 );
      printf("%u\n", (PTEs[i] >> offset) );
    }
  }
}

void printBitShift(unsigned int value, int shift) {
  if( (value >> shift) == 0){
    printf("0,");
    return;
  }
  printf("1,");
}

void printFTEs() {
  printf("Frame\t| F,Page\n");

  int i;
  for ( i = 0; i < num_frames; i++) {
    printf("%d\t|", i);
    printBitShift( FTEs[i], MOST_SIG_BIT );
  }
}





void initFTE() {
  FTEs = malloc(num_frames * sizeof(unsigned int));

  int i;
  for( i = 0; i<num_frames; i++) {
    FTEs[i] = 0x00000000;
  }
}

void initPTE() {
  PTEs = malloc(num_pages * sizeof(unsigned int));

  int i;
  for( i = 0; i<num_pages; i++ ) {
    PTEs[i] = 0x00000000;
  }
}
