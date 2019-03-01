EXEC=robot
SRC= $(wildcard *.c)
OBJ= $(SRC:.c=.o)
LDFLAGS=-lm -lSDL2
CFLAGS=-Wall

all: $(EXEC)

$(EXEC): $(OBJ)
	$(CC) -o $@ $^ $(LDFLAGS)

%.o: %.c
	$(CC) -o $@ -c $< $(CFLAGS)

.PHONY: clean

clean:
	@rm -rf *.o
	@rm -rf $(EXEC)
