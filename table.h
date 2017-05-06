extern unsigned int *PTEs;
extern unsigned int *FTEs;
extern int num_pages;
extern int num_frames;
extern int offset;

static const int MAX_BIT_SIZE = 32;
static const int MOST_SIG_BIT = 31;

static const int SIG_BIT_MASK = 0xFFFFFFFF;

static const int PTE_READ = 0xA0000000;
static const int PTE_WRITE = 0xE0000000;
static const int PTE_MASK = 0x1FFFFFFF;
static const int FTE_MASK = 0x7FFFFFFF;

void printPTEs();
void printFTEs();
void initPTE();
void initFTE();
void printBitshift(unsigned int value, int shift);
int validBitshift(unsigned int value, int shift);
int getFreeFrame();
unsigned int flipBit(unsigned int binary, int bit);
void updateTables(int openframe, int pagenumber, char instruction);
