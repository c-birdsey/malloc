CFLAGS = -Wall -pedantic -g

.PHONY: all
all: my-malloc.so test-malloc

my-malloc.so: my-malloc.c
	gcc $(CFLAGS) -rdynamic -shared -fPIC -o $@ $^ 

test-malloc: test-malloc.c
	gcc $(CFLAGS) -o $@ $^ 

.PHONY: ls 
ls:

	LD_PRELOAD=./my-malloc.so ls

.PHONY: ls-l
ls-l:
	LD_PRELOAD=./my-malloc.so ls -l

.PHONY: test
test:
	LD_PRELOAD=./my-malloc.so ./test-malloc

.PHONY: gdb
gdb: 
	gdb --args env LD_PRELOAD=./my-malloc.so ls -l

.PHONY: gdb-test
gdb-test: 
	gdb --args env LD_PRELOAD=./my-malloc.so ./test-malloc

.PHONY: clean
clean:
	rm -f my-malloc.so my-malloc test-malloc


