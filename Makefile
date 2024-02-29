CC = gcc

CFLAGS = -Wall -O2

# No linker flags I thnk
LFLAGS =

SRC = parse.c

OBJS = $(SRC:.c=.o)

MAIN = parser

all: $(MAIN)

$(MAIN): $(OBJS)
	$(CC) $(CFLAGS) -o $(MAIN) $(OBJS)

.c.o:
	$(CC) $(CFLAGS) -c $< -o $@

debug:
	$(CC) $(DEPENDENCIES) -o $(EXECUTABLE) -g

clean:
	rm *.o  $(MAIN)
