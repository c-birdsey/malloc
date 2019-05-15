# Implementing malloc

Fall 2018: This assignment created replacements for the malloc(3) and free(3) calls and also the variants of the former: calloc(3) and realloc(3). This implementation of malloc moderately and intelligently grows the size of the heap to accommodate for new requests, minimizing the calls to brk(2) and sbrk(2) to maintain efficiency. 

## Getting Started

In order to test the functionality of the recreation of the malloc and malloc-related calls, the Makefile default target builds a dynamically-linkable library that contains these implementations. Testing requires the use the LD_PRELOAD mechanism to cause this implementation to be loaded and linked before libc, which contains the official implementations of malloc(3) and free(3).


## Authors

* **Calder Birdsey** - *Initial work* - [c-birdsey](https://github.com/c-birdsey)

See also the list of [contributors](https://github.com/c-birdsey/io-syscalls/contributors) who participated in this project.

## Acknowledgments

* Professor Pete C. Johnson

## Sources: 
https://www.geeksforgeeks.org/linked-list-set-1-introduction/
https://www.mathworks.com/help/matlab/ref/break.html
https://www.rapidtables.com/convert/number/hex-to-binary.html
