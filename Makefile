serial: serial.cpp
	g++ serial.cpp -O3 -o serial

parallel: parallel.cpp
	mpicxx parallel.cpp -O3 -fopenmp -o parallel

gen: gendata.c
	gcc gendata.c -o gendata
	
clean:
	rm serial gendata parallel

test: serial parallel
	time ./serial
	time mpirun -np 4 -hosts ceca19,ceca23,ceca24 ./parallel
