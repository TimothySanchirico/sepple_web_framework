# cpp web stuff

SRC = src/*.C test/*.C
OBJ = $(SRC:.C=.o)

SHREKXXFLAGS = -std=c++17 -ggdb3

SRC_DIR = src/
LIBS = -lgtest -lgtest_main -lpthread -lgmock

http-request.o:  src/http_request.*
	$(CXX) -c -o build/http-request.o src/http_request.C $(SHREKXXFLAGS)

request-test: test/request.C http-request.o
	$(CXX) -o build/request-test test/request.C build/http-request.o $(SHREKXXFLAGS) -I $(SRC_DIR) $(LIBS) && ./build/request-test

route-handler-test: test/route_handler_test.C request-test src/route_handler.H
	$(CXX) -o build/route-handler-test test/route_handler_test.C build/http-request.o $(SHREKXXFLAGS) -I $(SRC_DIR) $(LIBS) && ./build/route-handler-test


