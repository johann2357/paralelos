all: example
run: run_example
example:
	mpicc -o example.out example.c
run_example:
	mpirun -np 4 ./example.out
clean:
	rm *.out

