CFLAGS = -Wall -pedantic -g

.PHONY: all
all: my-malloc.so test-malloc

my-malloc.so: my-malloc.c
	gcc $(CFLAGS) -rdynamic -shared -fPIC -o $@ $^ 

test-malloc: test-malloc.c
	gcc $(CFLAGS) -o $@ $^ 

.PHONY: link_ls 
link_ls:
	LD_PRELOAD=./my-malloc.so ls

.PHONY: link_test
link_test:
	LD_PRELOAD=./my-malloc.so ./test-malloc

.PHONY: gdb
gdb: 
	gdb --args env LD_PRELOAD=./my-malloc.so ls -l

.PHONY: clean
clean:
	rm -f my-malloc.so my-malloc test-malloc

