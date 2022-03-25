# Fichier Makefile

CC = gcc
CFLAGS = -g -Wall -Wextra -D _GNU_SOURCE 
CFLAG2 = -c -Wall -Wextra -D _GNU_SOURCE 
OBJETS = CL.o CL_msg.o CL_mem.o CL_sem.o

all: cl

cl : $(OBJETS) CL.c
	$(CC) $(CFLAGS) $(OBJETS) -o CL

CL.o : CL.c CL_include CL_msg.h CL_mem.h
	$(CC) $(CFLAG2) CL.c -o CL.o

CL_msg.o : CL_msg.c CL_include CL_msg.h
	$(CC) $(CFLAG2) CL_msg.c -o CL_msg.o

CL_mem.o : CL_mem.c CL_include CL_mem.h
	$(CC) $(CFLAG2) CL_mem.c -o CL_mem.o

CL_sem.o : CL_sem.c CL_include CL_sem.h
	$(CC) $(CFLAG2) CL_sem.c -o CL_sem.o

clean:
	@rm -f *~ *.o CL 




