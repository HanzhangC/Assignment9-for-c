CC = gcc
CFLAGS = -Wall -Wextra -pedantic -g

LDFLAGS =

TARGET = fetchmail
OBJ = main.o 1.logging_on.o 2.retrieve.o 3.parse.o 4.mime.o 5.list.o

all: $(TARGET)

$(TARGET): $(OBJ)
	$(CC) $(CFLAGS) -o $(TARGET) $(OBJ) $(LDFLAGS)

main.o: main.c functions.h
	$(CC) $(CFLAGS) -c main.c

1.logging_on.o: 1.logging_on.c functions.h
	$(CC) $(CFLAGS) -c 1.logging_on.c

2.retrieve.o: 2.retrieve.c functions.h
	$(CC) $(CFLAGS) -c 2.retrieve.c

3.parse.o: 3.parse.c functions.h
	$(CC) $(CFLAGS) -c 3.parse.c

4.mime.o: 4.mime.c functions.h
	$(CC) $(CFLAGS) -c 4.mime.c

5.list.o: 5.list.c functions.h
	$(CC) $(CFLAGS) -c 5.list.c

clean:
	rm -f $(TARGET) $(OBJ)