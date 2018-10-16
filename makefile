all: clean compile run
clean:
	rm -f omp
compile :omp.c
	echo "compiling"
	gcc -g -Wall -fopenmp omp.c -o omp
run : omp
	chmod u+x omp
	./omp