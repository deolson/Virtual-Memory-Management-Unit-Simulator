#include <stdio.h>
#include "table.h"

int clock_placement = 0;

int evictFrame();

//given an instruction and pagenumber we access the page table and run the replacement policy if needed
void clockReplacement(char instruction, int pagenumber) {

  //check if the page is valid and if it is we can just return the phys location in the page table
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

  //there was not a free frame and we now need to replace a frame
  int evicted_frame = evictFrame();
  printf("evicted frame %d\n",evicted_frame);

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

//clock_placement is where the current clock is, we loop threw the frame and if
//the refrenced bit was 1 we flip it to 0, and if it was 0 we choose it to be the evictFrame
int evictFrame() {
  while(1) {
    if( validBitshift(PTEs[(FTEs[clock_placement] & FTE_MASK)], MOST_SIG_BIT-2) ) { //check the refrenced bit
      //referenced bit was 1 so we flip it to 0
      PTEs[(FTEs[clock_placement] & FTE_MASK)] = flipBit(PTEs[(FTEs[clock_placement] & FTE_MASK)], (MOST_SIG_BIT-2) );
    } else {
      //referenced bit was 0 so we evict the frame
      return clock_placement;
    }
    clock_placement = (clock_placement+1) % num_frames; //incerment the clock
  }
}
