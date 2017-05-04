extern unsigned int *PTEs;
extern unsigned int *FTEs;
extern int num_pages;
extern int num_frames;
extern int offset;

#define MAX_BIT_SIZE 32;
#define MOST_SIG_BIT 31;

void printPTEs();
void printFTEs();
void initPTE();
void initFTE();
void printBitShift(unsigned int value, int shift);
