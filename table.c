#include <stdio.h>
#include <stdlib.h>
#include "table.h"

void printPTEs() {

  printf("\nPage\t| V,M,R,Frame\n");

  int i;
  for ( i = 0; i < num_pages; i++) {
    printf("%d\t| ", i);
    if( (PTEs[i] >> 31) == 0) {
      printf("0,-,-,-\n");
    } else {
      printBitshift(PTEs[i], MOST_SIG_BIT );
      printBitshift(PTEs[i], MOST_SIG_BIT-1 );
      printBitshift(PTEs[i], MOST_SIG_BIT-2 );
      printf("%u\n", (PTEs[i] & PTE_MASK) );
    }
  }
}

void printFTEs() {
  printf("\nFrame\t| F,Page\n");
  int i;
  for ( i = 0; i < num_frames; i++) {
    printf("%d\t| ", i);
    printBitshift( FTEs[i], MOST_SIG_BIT );
    if(validBitshift(FTEs[i], MOST_SIG_BIT)) {
      printf("0\n");
    } else {
      printf("%u\n", FTEs[i] & SIG_BIT_MASK);
    }
  }
}

void printBitshift(unsigned int value, int shift) {
  if( validBitshift(value, shift) )
    printf("1,");
  else
    printf("0,");
}

int validBitshift(unsigned int value, int shift) {
  if( value & ( 1 << shift) )
    return 1;
  return 0;
}


int getFreeFrame() {
  int i;
  for(i = 0; i < num_frames; i++) {
    if( validBitshift(FTEs[i], MOST_SIG_BIT) ) {
      return i;
    }
  }
  return -1;
}

unsigned int flipBit(unsigned int binary, int bit) {
   return (binary ^ (1<<bit));
}

void updateTables(int openframe, int pagenumber, char instruction) {
  FTEs[openframe] = flipBit(FTEs[openframe], MOST_SIG_BIT);
  FTEs[openframe] += pagenumber;

  if(instruction == 'w')
    PTEs[pagenumber] = PTE_WRITE + openframe;
  if(instruction == 'r')
    PTEs[pagenumber] = PTE_READ + openframe;

  printf("\tphys_addr=0x%08x\n",(PTEs[pagenumber] & PTE_MASK) << offset );
}


void initPTE() {
  PTEs = malloc(num_pages * sizeof(unsigned int));

  int i;
  for( i = 0; i<num_pages; i++ ) {
    PTEs[i] = 0x00000000;
  }
}

void initFTE() {
  FTEs = malloc(num_frames * sizeof(unsigned int));

  int i;
  for( i = 0; i<num_frames; i++) {
    FTEs[i] = 0x80000000;
  }
}
