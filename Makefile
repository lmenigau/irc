CXX=clang++
CXXFLAGS=-std=c++98
SRC = main.cpp
OBJ = $(SRC:.c=.o)
ircserv: main.o
	$(CXX) -o ircserv main.o
