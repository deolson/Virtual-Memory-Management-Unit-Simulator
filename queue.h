struct queue
{
	struct qentry *head;
	struct qentry *tail;
	int size;
};

struct qentry
{
	int page;
	struct qentry *prev;
	struct qentry *next;
};

extern struct queue *fifoQ;

void queueReplacement(char instruction, int pagenumber, int LRU_flag);
int removePage(int pagenumber, struct queue *q);
void	printqueue(struct queue *q);
int	isempty(struct queue *q);
void enqueue(int enqframe, struct queue *q);
int dequeue(struct queue *q);
void freeQueue(struct queue *q);
void initFIFO();
int contains(int pagenumber, struct queue *q);
