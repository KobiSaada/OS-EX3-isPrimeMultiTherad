
solution: solution.c
	gcc solution.c -o solution -std=gnu11 -lm -lpthread

.PHONY: clean all
clean:
	rm -f *.o *.a *.so solution
