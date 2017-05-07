#include <stdio.h>

#include "table.h"

int clock_placement = 0;

int evictFrame();

void clockReplacement(char instruction, int pagenumber) {

  if(validBitshift(PTEs[pagenumber], MOST_SIG_BIT)) {
    printf("\tphys_addr=0x%08x\n", (PTEs[pagenumber] & PTE_MASK) << offset );
    return;
  }

  printf("\tPage fault: page=%d\n", pagenumber);

  faults++;

  int openframe = getFreeFrame();
  if(openframe != -1) {
    updateTables(openframe, pagenumber, instruction);
    return;
  }

  replacements++;

  int evicted_frame = evictFrame();
  printf("evicted frame %d\n",evicted_frame);

  int evicted_page = FTEs[evicted_frame] & FTE_MASK;

  if( validBitshift( PTEs[evicted_page] , MOST_SIG_BIT-1) )
    printf("\tPage replacement: evicted_page=%d, writeout=true\n",evicted_page);
  else
    printf("\tPage replacement: evicted_page=%d, writeout=false\n",evicted_page);

  PTEs[ (FTEs[evicted_frame] & FTE_MASK) ] = 0x00000000;
  FTEs[evicted_frame] = 0x80000000;

  updateTables( evicted_frame , pagenumber, instruction);
}

int evictFrame() {
  while(1) {
    if( validBitshift(PTEs[(FTEs[clock_placement] & FTE_MASK)], MOST_SIG_BIT-2) ) {
      PTEs[(FTEs[clock_placement] & FTE_MASK)] = flipBit(PTEs[(FTEs[clock_placement] & FTE_MASK)], (MOST_SIG_BIT-2) );
    } else {
      return clock_placement;
    }
    clock_placement = (clock_placement+1) % num_frames;
  }
}
