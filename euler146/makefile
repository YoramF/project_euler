LIBDIR = -L../../async -L../prime
LIB = -lprime -lasync -lm
LIB_D = -lprime_d -lasync_d -lm
INC = -I../../async -I../prime

euler146: euler146.o
	gcc -o euler146 euler146.o $(LIB) $(LIBDIR)

euler146_d: euler146_d.o
	gcc -o euler146_d euler146_d.o $(LIB_D) $(LIBDIR)

euler146.o: euler146.c 
	gcc -c euler146.c -o euler146.o $(INC)

euler146_d.o: euler146.c 
	gcc -g -c euler146.c -o euler146_d.o $(INC)