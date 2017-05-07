#include <string.h>
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <math.h>
#include <time.h>
#include "policies.h"
#include "table.h"
#include "queue.h"

//globals
unsigned int *PTEs;
unsigned int *FTEs;
int num_pages;
int num_frames;
void (*policy_fun)(char instruction, int pagenumber);
int offset;
struct queue *fifoQ;

double reads;
double writes;
double faults;
double replacements;

//prototypes
void readInput(char *inputFile);
void procInstruct(char *instruction, unsigned int addr);
void checkPageTable(char instruction, int pagenumber);
void decidePolicy(char rep_policy);
void updateTables(int openframe, int pagenumber, char instruction);


int main(int argc, char * const argv[]) {

  num_pages = 8; //(optional) must be a multiple of 2 (defaults to 8, if not given)
  num_frames = 4; //optional) must be a multiple of 2 (defaults to 4, if not given)
  int block_size = 1024; //(optional) must be a multiple of 2 and is the number of bytes per page/frame (defaults to 1024)
  char *rep_policy = "random"; //(optional) is the page replacement policy.(defaults to random, if not given)
  srand(time(NULL));
  char* inst_file = argv[argc-1]; //(required) is a path to a file containing instructions

  reads = 0;
  writes = 0;
  faults = 0;
  replacements = 0;

  //using getopt function -- told some people about this -- really useful
  int c;
  while( (c = getopt(argc, argv, ":p:f:b:r:")) != -1 ){ //specifies option flags p,f,b,r
    if(optarg == inst_file) {
      printf("Option %c requires an argument\n",optopt);
      exit(-1);
    }
    switch (c) {
      case 'p'://page flag
        num_pages = atoi(optarg);
        if((num_pages % 2) != 0 || num_pages<=0) {
          printf("Pages must be a multiple of 2\n");
          exit(-1);
        }
        break;
      case 'f'://frame flag
        num_frames = atoi(optarg); //instructions state needs to be a multiple of 2, but examples are not
        printf("num frames2 %d\n",num_frames );
        break;
      case 'b'://block flag
        block_size = atoi(optarg);
        if((block_size % 2) != 0 || block_size<=0) {
          printf("block_size must be a multiple of 2\n");
          exit(-1);
        }
        break;
      case 'r'://policy flag -- bad input here will result in random default
        rep_policy = optarg;
        break;
      default:
        exit(-1);
    }
  }

  //assuming 32 bit instructions, if arguments would require 32>bits exit
  if( ( (log(num_pages)/log(2) ) + (log(block_size)/log(2)) ) > MAX_BIT_SIZE ) {
    printf("We are working with 32-bit instructions, input does not match\n");
    exit(-1);
  }

  printf("pages:%d | frames:%d  | block:%d | policy:%s | file:%s\n", num_pages, num_frames, block_size, rep_policy, inst_file);

  //malloc and initalize the frame and page table
  initPTE();
  initFTE();

  //grab the offset bits log2(block_size)
  offset = (int)(log(block_size) / log(2));

  char policyChar = rep_policy[0];
  policyChar = tolower(policyChar);

  //function pointer to the policy we are using
  decidePolicy(policyChar);

  //loops through the input file and parses each instruction by line
  readInput(inst_file);

  double total = reads+writes;
  double readPercent = (reads / total) * 100;
  double writePercent = (writes / total) * 100;
  double faultPercent = (faults / total) * 100;
  double replacementPercent = (replacements/total) * 100;
  printf("inst_count=%.0f (reads=%.2f%%, writes=%.2f%%), page_faults=%.0f (%.2f%%), replacements=%.0f (%.2f%%)\n",total, readPercent, writePercent, faults, faultPercent, replacements, replacementPercent);

  //frees our mallocs
  if (policyChar == 'f' || policyChar == 'l')
    freeQueue(fifoQ);
  free(PTEs);
  free(FTEs);
  return 0;
}

//loops throguh instruction file and gets < op > and optional [ addr ]
//allowed to assume good input here
void readInput(char *inputFile) {
  FILE *file = fopen(inputFile,"r");

  char line[14];
  char *token;
  int count;

  char *instruction;
  unsigned int addr;

  while(fgets(line, sizeof(line), file)) {

    token = strtok(line, " \n");// delimit by new line and by space
    count = 0;

    while (token!=NULL) {

      if(count == 0) { //the first token will always be the instruction
        instruction = token;
      } else { //we haven't reset count so the token will be the addr
        addr = (unsigned int)strtoul(token, NULL, 0);
      }

      count++;
      token = strtok(NULL, " \n");
    }
    // update the frame and page table given we know the instruction and addr now
    procInstruct(instruction, addr);
  }
}

//processes the instruction, this basically filters out the print instructions and gives initial print statments
void procInstruct(char *instruction, unsigned int addr) {

  unsigned int pagenumber = addr >> offset;

  char firstLetter = instruction[0];

  switch(firstLetter) {
    case 'W': //write instruction

      printf("START_WRITE\n");
      printf("\tvirt_addr=0x%08x\n", addr);

      if( pagenumber >= num_pages ) {
        printf("\tSegmentation fault: illegal_page=%u\n", pagenumber);
        printf("END_WRITE\n\n");
        return;
      }

      writes++;

      policy_fun('w', pagenumber);
      printf("END_WRITE\n\n");
      break;

    case 'R': //read instruction

      printf("START_READ\n");
      printf("\tvirt_addr=0x%08x\n", addr);

      if( pagenumber >= num_pages ) {
        printf("\tSegmentation fault: illegal_page=%u\n", pagenumber);
        printf("END_READ\n\n");
        return;
      }

      reads++;

      policy_fun('r', pagenumber);
      printf("END_READ\n\n");
      break;

    case 'P': //print instruction
      if(instruction[1] == 'P') { //2nd letter is P then print page table
        printPTEs();
        printf("\n");
      } else { //print frame table
        printFTEs();
        printf("\n");
      }
      break;
  }
}

void decidePolicy(char policyChar) {

  switch (policyChar) {
    case 'f':
      initFIFO();
      policy_fun = fifoReplacement;
      break;
    case 'l':
      initFIFO();
      policy_fun = LRUReplacement;
      break;
    case 'c':
      policy_fun = clockReplacement;
      break;
    default:
       policy_fun = rReplacement;
      break;
  }
}
