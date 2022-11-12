OBJS = duplicates.o hashing.o filesize.o listfiles.o 
SOURCE = duplicates.c hashing.c filesize.c listfiles.c 
HEADER = 
OUT = a.out 
CC = gcc 
FLAGS = -g -c -Wall -Werror -pedantic
LFLAGS = 
all: $(OBJS) 
				$(CC) -g $(OBJS) -o $(OUT) $(LFLAGS) 
duplicates.o: duplicates.c 
				$(CC) $(FLAGS) duplicates.c -std=c11 
hashing.o: hashing.c 
				$(CC) $(FLAGS) hashing.c -std=c11 
filesize.o: filesize.c 
				$(CC) $(FLAGS) filesize.c -std=c11 
listfiles.o: listfiles.c 
				$(CC) $(FLAGS) listfiles.c -std=c11 
clean: 
				rm -f $(OBJS) $(OUT)