#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main(int argc, char * const argv[]) {

  int num_pages = 8; //(optional) must be a multiple of 2 (defaults to 8, if not given)
  int num_frames = 4; //optional) must be a multiple of 2 (defaults to 4, if not given)
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
        break;
      case 'f':
        num_frames = *optarg;
        break;
      case 'b':
        block_size = *optarg;
        break;
      case 'r':
        rep_policy = optarg;
        break;
      case '?':
        printf("how now borwn cow\n");
      default:
        exit(-1);
    }
  }
  printf("pages:%d | frames:%d  | block:%d | policy:%s | file:%s\n", num_pages, num_frames, block_size, rep_policy, inst_file);



  return 0;
}
