#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "table.h"

void rReplacement(char instruction, int pagenumber) {

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
  
  int evicted_frame = rand() % num_frames;
  int evicted_page = FTEs[evicted_frame] & FTE_MASK;

  if( validBitshift( PTEs[evicted_page] , MOST_SIG_BIT-1) )
    printf("\tPage replacement: evicted_page=%d, writeout=true\n",evicted_page);
  else
    printf("\tPage replacement: evicted_page=%d, writeout=false\n",evicted_page);

  PTEs[ (FTEs[evicted_frame] & FTE_MASK) ] = 0x00000000;
  FTEs[evicted_frame] = 0x80000000;

  updateTables( evicted_frame , pagenumber, instruction);
}
