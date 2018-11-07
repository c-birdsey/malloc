CFLAGS = -Wall -pedantic -g

.PHONY: all
all: my-malloc.so test-malloc

my-malloc.so: my-malloc.c
	gcc $(CFLAGS) -rdynamic -shared -fPIC -o $@ $^ 

test-malloc: test-malloc.c
	gcc $(CFLAGS) -o $@ $^ 

.PHONY: link
link:
	LD_PRELOAD=./my-malloc.so ls

.PHONY: gdb
gdb: 
	gdb --args env LD_PRELOAD=./my-malloc.so ./test-malloc

.PHONY: clean
clean:
	rm -f my-malloc.so
	rm -f my-malloc
	rm -f test-malloc

