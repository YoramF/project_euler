libsets: sets.o crc.o
	ar rcs libsets.a sets.o crc.o

sets.o: sets.c sets.h crc.h
	gcc -c sets.c -o sets.o

crc.o: crc.c crc.h
	gcc -c crc.c -o crc.o

libsets_d: sets_d.o crc_d.o
	ar rcs libsets_d.a sets_d.o	crc_d.o

sets_d.o: sets.c sets.h  crc.h
	gcc -g -c sets.c -o sets_d.o

crc_d.o: crc.c crc.h
	gcc -g -c crc.c -o crc_d.o

tst_d: tst.c libsets_d.a
	gcc -g tst.c -o tst_d -lsets_d -L .