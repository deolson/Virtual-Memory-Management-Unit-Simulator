#include <stdlib.h>
#include <stdio.h>

#include "queue.h"
#include "table.h"

//fifo and lru are similar -- I built my lru on top of my fifo queue.
//when lru policy is in place I set the lru flag that if the queue contains the current instruction I move it back to the end of the queue
//In fifo replacement it will keep it's place in the queue even if it's called again

void fifoReplacement(char instruction, int pagenumber) {
  queueReplacement(instruction, pagenumber, 0);
}

void LRUReplacement(char instruction, int pagenumber) {
  queueReplacement(instruction, pagenumber, 1);
}

void queueReplacement(char instruction, int pagenumber, int LRU_flag) {

  //if the queue already contains the instruction we don't want to add the instruction back into the queue
  if(!contains(pagenumber, fifoQ)) {
    enqueue(pagenumber, fifoQ);
  } else if (LRU_flag) {
    //We reach this statement if the queue conatins the instruction, we then will reset the instructions place in the queue if lru flag is on
    enqueue(removePage(pagenumber,fifoQ),fifoQ);
  }

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

  //in both fifo and lru we can just dequeue for our evicted_page
  int evicted_page = dequeue(fifoQ);
  int evicted_frame = PTEs[evicted_page] & PTE_MASK;

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
