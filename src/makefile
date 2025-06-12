# Compilatore
CC = gcc
# FLags di compilazione
CFLAGS = -Wall -Wextra -std=c11
# Librerie
LIBS = -lpthread -ldl

#Nome eseguibile
TARGET = todo-cli

#File sorgenti
SRCS = main.c sqlite/sqlite3.c

#File oggetto
OBJS = $(SRCS:.c=.o)

.PHONY: all clean

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) -o $@ $^ $(LIBS)

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(OBJS) $(TARGET)
