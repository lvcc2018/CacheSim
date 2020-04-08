main:main.o cache.o
	g++ -o main cache.o main.o
main.o:main.cpp
	g++ -c main.cpp
cache.o:cache.cpp
	g++ -c cache.cpp
.PHONY:clean
clean:
	rm -f *.o
	rm main
