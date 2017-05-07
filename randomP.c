#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "table.h"

void rReplacement(char instruction, int pagenumber) {

  //if the queue already contains the instruction we don't want to add the instruction back into the queue
  if(validBitshift(PTEs[pagenumber], MOST_SIG_BIT)) {
    printf("\tphys_addr=0x%08x\n", (PTEs[pagenumber] & PTE_MASK) << offset );
    return;
  }

  //if the page was not valid there is a page fault and we need to update the frame table
  printf("\tPage fault: page=%d\n", pagenumber);
  faults++;

  //check to see if there is a free frame and assign
  int openframe = getFreeFrame();
  if(openframe != -1) {
    updateTables(openframe, pagenumber, instruction);
    return;
  }

  replacements++;

  //chose a random frame to evict
  int evicted_frame = rand() % num_frames;
  int evicted_page = FTEs[evicted_frame] & FTE_MASK;

  if( validBitshift( PTEs[evicted_page] , MOST_SIG_BIT-1) )
    printf("\tPage replacement: evicted_page=%d, writeout=true\n",evicted_page);
  else
    printf("\tPage replacement: evicted_page=%d, writeout=false\n",evicted_page);

  //reset the page and frame tables of the choosen page/frame
  PTEs[ (FTEs[evicted_frame] & FTE_MASK) ] = 0x00000000;
  FTEs[evicted_frame] = 0x80000000;

  //we have cleared the page and frame table at this point, now we update these spots with the current instruction
  updateTables( evicted_frame , pagenumber, instruction);
}
