.PHONY: all clean

CC = clang

CFLAGS = -Wall
CFLAGS += -Wextra
CFLAGS += -O0

SRCS = main.c

TARGET = tup0

all: $(TARGET)

$(TARGET): $(SRCS)
	$(CC) $(CFLAGS) -o $@ $^

clean:
	rm -f $(TARGET)
