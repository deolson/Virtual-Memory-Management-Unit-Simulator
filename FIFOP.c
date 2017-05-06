#include <stdlib.h>
#include <stdio.h>

#include "queue.h"
#include "table.h"

void fifoReplacement(char instruction, int pagenumber) {
  queueReplacement(instruction, pagenumber, 0);
}

void LRUReplacement(char instruction, int pagenumber) {
  queueReplacement(instruction, pagenumber, 1);
}

void queueReplacement(char instruction, int pagenumber, int LRU_flag) {

  if(!contains(pagenumber, fifoQ)) {
    enqueue(pagenumber, fifoQ);
  } else if (LRU_flag) {
    enqueue(removePage(pagenumber,fifoQ),fifoQ);
  }

  if(validBitshift(PTEs[pagenumber], MOST_SIG_BIT)) {
    printf("\tphys_addr=0x%08x\n", (PTEs[pagenumber] & PTE_MASK) << offset );
    return;
  }

  printf("\tPage fault: page=%d\n", pagenumber);

  int openframe = getFreeFrame();
  if(openframe != -1) {
    updateTables(openframe, pagenumber, instruction);
    return;
  }

  int evicted_page = dequeue(fifoQ);
  int evicted_frame = PTEs[evicted_page] & PTE_MASK;

  if( validBitshift( PTEs[evicted_page] , MOST_SIG_BIT-1) )
    printf("\tPage replacement: evicted_page=%d, writeout=true\n",evicted_page);
  else
    printf("\tPage replacement: evicted_page=%d, writeout=false\n",evicted_page);

  PTEs[ (FTEs[evicted_frame] & FTE_MASK) ] = 0x00000000;
  FTEs[evicted_frame] = 0x80000000;

  updateTables( evicted_frame , pagenumber, instruction);

}
