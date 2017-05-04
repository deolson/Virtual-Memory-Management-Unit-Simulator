#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <math.h>
#include "table.h"

unsigned int *PTEs;
unsigned int *FTEs;
int num_pages;
int num_frames;
int offset;

int main(int argc, char * const argv[]) {

  num_pages = 8; //(optional) must be a multiple of 2 (defaults to 8, if not given)
  num_frames = 4; //optional) must be a multiple of 2 (defaults to 4, if not given)
  int block_size = 1024; //(optional) must be a multiple of 2 and is the number of bytes per page/frame (defaults to 1024)
  char* rep_policy = "random"; //(optional) is the page replacement policy.(defaults to random, if not given)
  char* inst_file = argv[argc-1]; //(required) is a path to a file containing instructions

  int c;
  while( (c = getopt(argc, argv, "p:f:b:r:")) != -1 ){
    if(optarg == inst_file) {
      printf("Option %c requires an argument\n",optopt);
      exit(-1);
    }
    switch (c) {
      case 'p':
        num_pages = *optarg;
        if((num_pages % 2) != 0 || num_pages<=0) {
          printf("Pages must be a multiple of 2\n");
          exit(-1);
        }
        break;
      case 'f':
        num_frames = *optarg;
        if((num_frames % 2) != 0 || num_frames<=0) {
          printf("num_frames must be a multiple of 2\n");
          exit(-1);
        }
        break;
      case 'b':
        block_size = *optarg;
        if((block_size % 2) != 0 || block_size<=0) {
          printf("block_size must be a multiple of 2\n");
          exit(-1);
        }
        break;
      case 'r':
        rep_policy = optarg;
        break;
      default:
        exit(-1);
    }
  }

  if( ( (log(num_pages)/log(2) ) + (log(block_size)/log(2)) ) > MAX_BIT_SIZE ) {
    printf("We are working with 32-bit instructions, input does not match\n");
    exit(-1);
  }
  printf("pages:%d | frames:%d  | block:%d | policy:%s | file:%s\n", num_pages, num_frames, block_size, rep_policy, inst_file);

  initPTE();

  offset = (int)(log(block_size) / log(2));
  printf("%d\n",PTEs[0] >> offset);

  printPTEs();
  printf("\n");
  printFTEs();


  free(PTEs);
  free(FTEs);
  return 0;
}
