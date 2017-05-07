all: main.c table.o randomP.o FIFOP.o queue.o clock.o
	gcc -Wall -o vmm main.c randomP.o table.o FIFOP.o queue.o clock.o -lm

table.o: table.h table.c
	gcc -Wall -c table.c

randomP.o: randomP.c
	gcc -Wall -c randomP.c

FIFOP.o: FIFOP.c
	gcc -Wall -c FIFOP.c

queue.o: queue.h queue.c
	gcc -Wall -c queue.c

clock.o: clock.c
	gcc -Wall -c clock.c

# all: thebar.c bartender.o customer.o
# 	gcc -Wall -o graph thebar.c bartender.o customer.o -lpthread


# customer.o: customer.h customer.c
# 	gcc -Wall -c customer.c

clean:
	rm -f vmm *.o
