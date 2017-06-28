serial: serial.cpp
	g++ serial.cpp -O3 -o serial

debug: serial.cpp
	g++ serial.cpp -D DEBUG -o serial 

gen: gendata.c
	gcc gendata.c -o gendata
	
clean:
	rm serial gendata