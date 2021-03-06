CC = gcc -g
TARGET = prover
OBJ =  prover.o PriorityQueue.o
LIBS =
INC = -I./
DEF =
CFLAGS = $(INC) $(DEF)

all: $(TARGET)

$(TARGET): $(OBJ)
	$(CC) $(OBJ) -o $(TARGET) $(LIBS)
clean:
	rm *.o
	rm $(TARGET)

