euler153: euler153.c imaginary.o
	gcc -o euler153 euler153.c imaginary.o -lprime -lm -I../prime -L../prime -I.

euler153_d: euler153.c imaginary_d.o
	gcc -g -o euler153_d euler153.c imaginary_d.o -lprime_d -lm -I../prime -L../prime -I.

imaginary.o: imaginary.c imaginary.h
	gcc -c o imaginary.o imaginary.c -I.

imaginary_d.o: imaginary.c imaginary.h
	gcc -c -g -o imaginary_d.o imaginary.c -I.