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
	time mpirun -np 3 -hosts ceca19,ceca23,ceca24 ./parallel

suite: serial parallel gendata
	./gendata 256 16500
	time serial
	time mpirun -np 3 -hosts ceca19,ceca23,ceca24 ./parallel
	./gendata 256 16000
	time serial
	time mpirun -np 3 -hosts ceca19,ceca23,ceca24 ./parallel
	./gendata 256 15000
	time serial
	time mpirun -np 3 -hosts ceca19,ceca23,ceca24 ./parallel

	
