all: main.c table.o
	gcc -Wall -o vmm main.c table.o -lm

table.o: table.h table.c
	gcc -Wall -c table.c

# all: thebar.c bartender.o customer.o
# 	gcc -Wall -o graph thebar.c bartender.o customer.o -lpthread


# customer.o: customer.h customer.c
# 	gcc -Wall -c customer.c

clean:
	rm -f vmm *.o
