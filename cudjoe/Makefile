all: main

main: main.o
	g++ -o main  -I/usr/local/include -I/usr/local/include/cppconn   -Wl,-Bdynamic main.cpp -lmysqlcppconn

main.o: main.cpp
	g++ -c main.cpp

clean:
	rm main.o main
