SRC = ../src/*.C *.C
OBJ = $(SRC:.C=.o)

SHREKXXFLAGS = -std=c++17 -ggdb3

BUILD_DIR = ../build/
SRC_DIR = ../src/
LIBS = -lgtest -lgtest_main -lpthread -lgmock

libs:
	@cd ..; make libs

request-test: request.C libs
	$(CXX) -o $(BUILD_DIR)/$@ $< $(BUILD_DIR)/http-request.o $(SHREKXXFLAGS) -I $(SRC_DIR) $(LIBS) && ./$(BUILD_DIR)/$@

route-handler-test: route_handler_test.C $(SRC_DIR)/route_handler.H request-test 
	$(CXX) -o $(BUILD_DIR)/$@ $< $(BUILD_DIR)/http-request.o $(SHREKXXFLAGS) -I $(SRC_DIR) $(LIBS) && ./$(BUILD_DIR)/$@

server-test: server_test.C route-handler-test
	$(CXX) -o $(BUILD_DIR)/$@ $< $(BUILD_DIR)/http-request.o $(SHREKXXFLAGS) -I $(SRC_DIR) $(LIBS) && ./$(BUILD_DIR)/$@

test-all: request-test route-handler-test server-test
