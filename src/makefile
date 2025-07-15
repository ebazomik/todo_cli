CC = gcc

CFLAGS = -Wall -Wextra -std=c11

LIBS = -lpthread -ldl

TARGET = todo-cli

SRCS = main.c sqlite/sqlite3.c

OBJS = $(SRCS:.c=.o)

.PHONY: all clean

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) -o $@ $^ $(LIBS)

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(OBJS) $(TARGET)
