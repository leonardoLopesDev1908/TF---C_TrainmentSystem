CC = gcc

CFLAGS = -Iinclude

SRC = $(wildcard src/*.c)

OBJ = $(SRC:src/%.c=build/%.o)

TARGET = app

all: $(TARGET)

$(TARGET): $(OBJ)
	$(CC) $(OBJ) -o $(TARGET)

build/%.o: src/%.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f build/*.o $(TARGET)