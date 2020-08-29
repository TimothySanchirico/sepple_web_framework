# cpp web stuff

SRC = src/*.C test/*.C
OBJ = $(SRC:.C=.o)

SHREKXXFLAGS = -std=c++17 -ggdb3

SRC_DIR = src/
LIBS = -lgtest -lgtest_main -lpthread -lgmock

http-common.o:  src/http.C
	$(CXX) -c -o build/$@ $< $(SHREKXXFLAGS)

http-request.o:  src/http_request.C src/http_request.H http-common.o
	$(CXX) -c -o build/$@ $< $(SHREKXXFLAGS)

libs:  http-request.o

test-all:
	@ cd test; make test-all; cd -


