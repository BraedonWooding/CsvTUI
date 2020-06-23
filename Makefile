CC=gcc
CFLAGS=-std=c11 -gdwarf

csv: color.o csv.o csv_reader.o ll.o main.o defs.o
	$(CC) $(CFLAGS) color.o csv.o csv_reader.o main.o defs.o ll.o -o csv

color.o: color.c

csv.o: csv.c

csv_reader.o: csv_reader.c

ll.o: ll.c

main.o: main.c

defs.o: defs.c

.PHONY : clean
clean :
	-rm *.o csv
