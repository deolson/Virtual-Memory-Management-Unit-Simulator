#include <stdlib.h>
#include <stdio.h>
#include "queue.h"

//print the fifo/lru queue used for debugging
void	printqueue(struct queue *q) {

	if(isempty(q)) {
    printf("[ ]\n");
		return;
	}

  struct qentry *currNode = q->head;

  printf("[ ");
  while(currNode != NULL) {
    printf("%d,", currNode->page);
    currNode = currNode->next;
  }
  printf("]\n");

}

int contains(int pagenumber, struct queue *q) {

  	if(isempty(q)) {
  		return 0;
  	}

    struct qentry *currNode = q->head;

    while(currNode != NULL) {
      if(currNode->page == pagenumber){
        return 1;
      }
      currNode = currNode->next;
    }
    return 0;
}

int	isempty(struct queue *q)
{
	if (q->size == 0){
		return 1;
	}
	return 0;
}


void enqueue(int enqPage, struct queue *q)
{
  struct qentry *toAdd = malloc(sizeof(struct qentry));
  toAdd->page = enqPage;
  toAdd->next = NULL;
  toAdd->prev = NULL;

  if(isempty(q)) {
    q->head = toAdd;
    q->tail = toAdd;
  } else {
    q->tail->next = toAdd;
    toAdd->prev = q->tail;
    q->tail = toAdd;
  }
  q->size++;
}


int dequeue(struct queue *q)
{
  if(isempty(q)) {
    return -1;
  }

  struct qentry *headNode = q->head;
  int headFrame = headNode->page;

  if(q->size == 1) {
    q->head = NULL;
    q->tail = NULL;
  } else {
    q->head = headNode->next;
  }

  q->size--;
  free(headNode);
  return headFrame;
}

int	getlast(struct queue *q)
{
	struct qentry *old_tail = q->tail;
	int pagenumber = old_tail->page;

	if (q->size == 1){
		q->tail = NULL;
		q->head = NULL;
	} else {
		q->tail = old_tail->prev;
		q->tail->next = NULL;
	}

	free(old_tail);
	q->size--;
	return pagenumber;
}

int removePage(int pagenumber, struct queue *q) {
  if(q->head->page == pagenumber){
		return dequeue(q);
	}
	if(q->tail->page == pagenumber){
		return getlast(q);
	}
	if(q->size > 2){

		struct qentry *checking = q->head->next;
		while(checking != NULL){
			if(checking->page == pagenumber){
				checking->prev->next = checking->next;
				checking->next->prev = checking->prev;
				free(checking);
				q->size--;
				return pagenumber;
			}
			checking = checking->next;
		}
	}
  return -1;
}

void initFIFO() {
  fifoQ = malloc(sizeof(struct queue));
  fifoQ->size = 0;
}

void freeQueue(struct queue *q) {
  int i;
  for( i = 0; i < q->size; i++) {
    dequeue(q);
  }
  free(q);
}
