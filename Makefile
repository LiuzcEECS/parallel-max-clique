serial: serial.cpp
	g++ serial.cpp -O3 -fopenmp -o serial

debug: serial.cpp
	g++ serial.cpp -D DEBUG -o debug-serial 

gen: gendata.c
	gcc gendata.c -o gendata
	
clean:
	rm serial gendata debug-serial
