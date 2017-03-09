CC = clang
LD = $(CC)
CFLAGS = -std=c99 -Wextra -Wall -g
LDFLAGS =
EXEC = testpatch

all: $(EXEC)

testpatch: testpatch.o patchwork.o image.o ast.o
	$(CC) -o $@ $^ $(LDFLAGS)

%.o: %.c
	$(CC) -o $@ -c $< $(CFLAGS)

clean:
	rm -f *.o *~ $(EXEC) *.ppm
