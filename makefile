CC = gcc
CFLAGS = -Wall -Wextra -std=c11
TARGET = main
OBJS = main.o commandshell.o

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) -o $(TARGET) $(OBJS)

main.o: main.c commandshell.h
	$(CC) $(CFLAGS) -c main.c

commandshell.o: commandshell.c commandshell.h
	$(CC) $(CFLAGS) -c commandshell.c

clean:
	rm -f $(TARGET) $(OBJS)

.PHONY: all clean