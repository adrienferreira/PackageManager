.PHONY:clean

DEBUG_FLAG=-Wno-unused-variable
CC=g++
FLAGS=-g -std=c++11 -Wall -pedantic-errors $(DEBUG_FLAG)

main : Package.o  PackageManager.o Node.o Leaf.o Dependency.o Providing.o DepProv.o main.o
	$(CC) $^ $(FLAGS) -o $@

Package.o : Package.cpp Package.h
	$(CC) -c $< $(FLAGS)

PackageManager.o : PackageManager.cpp PackageManager.h
	$(CC) -c $< $(FLAGS)

a.out: Node.o Leaf.o Dependency.o Providing.o DepProv.o main.o
	$(CC) $^ -o $@ $(FLAGS)

Node.o:Node.cpp Node.h NodeBase.h
	$(CC) -c $< $(FLAGS)

Leaf.o:Leaf.cpp Leaf.h NodeBase.h
	$(CC) -c $< $(FLAGS)

Dependency.o:Dependency.cpp Dependency.h DepProv.h
	$(CC) -c $< $(FLAGS)

Providing.o:Providing.cpp Providing.h DepProv.h
	$(CC) -c $< $(FLAGS)

DepProv.o:DepProv.cpp DepProv.h
	$(CC) -c $< $(FLAGS)

main.o : main.cpp
	$(CC) -c main.cpp $(FLAGS)

clean:
	rm -rvf *.o main
