# cpp web stuff

SRC = src/*.C test/*.C
OBJ = $(SRC:.C=.o)

SHREKXXFLAGS = -std=c++17 -ggdb3

SRC_DIR = src/
LIBS = -lgtest -lgtest_main -lpthread -lgmock

http-request.o:  src/http_request.C src/http_request.H
	$(CXX) -c -o build/$@ $< $(SHREKXXFLAGS)

request-test: test/request.C http-request.o
	$(CXX) -o build/$@ $< build/http-request.o $(SHREKXXFLAGS) -I $(SRC_DIR) $(LIBS) && ./build/$@

route-handler-test: test/route_handler_test.C src/route_handler.H request-test 
	$(CXX) -o build/$@ $< build/http-request.o $(SHREKXXFLAGS) -I $(SRC_DIR) $(LIBS) && ./build/$@


