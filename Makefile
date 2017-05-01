all: main.c main.h
	gcc -Wall -o vmm main.c

# all: thebar.c bartender.o customer.o
# 	gcc -Wall -o graph thebar.c bartender.o customer.o -lpthread

# bartender.o: bartender.h bartender.c
# 	gcc -Wall -c bartender.c
#
# customer.o: customer.h customer.c
# 	gcc -Wall -c customer.c

clean:
	rm -f vmm *.o
