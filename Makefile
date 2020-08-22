# cpp web stuff

SRC = src/*.C test/*.C
OBJ = $(SRC:.C=.o)

SHREKXXFLAGS = -std=c++17 -ggdb3

SRC_DIR = src/
LIBS = -lgtest -lgtest_main -lpthread

http-request.o:  src/http_request.*
	$(CXX) -c -o http-request.o src/http_request.C $(SHREKXXFLAGS)

request-test: test/request.C http-request.o
	$(CXX) -o request-test test/request.C http-request.o $(SHREKXXFLAGS) -I $(SRC_DIR) $(LIBS) && ./request-test


