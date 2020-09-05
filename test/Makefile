SRC = ../src/*.C *.C
OBJ = $(SRC:.C=.o)

SHREKXXFLAGS = -std=c++17 -ggdb3

BUILD_DIR = ../build/
LOCAL_OBJS = $(BUILD_DIR)/http-request.o $(BUILD_DIR)/http-common.o $(BUILD_DIR)/http-response.o $(BUILD_DIR)/tcp-server.o
SRC_DIR = ../src/
LIBS = -lgtest -lgtest_main -lpthread -lgmock

libs:
	@cd ..; make libs

request-test: request.C libs
	$(CXX) -o $(BUILD_DIR)/$@ $< $(LOCAL_OBJS) $(SHREKXXFLAGS) -I $(SRC_DIR) $(LIBS) && ./$(BUILD_DIR)/$@

response-test: response.C libs
	$(CXX) -o $(BUILD_DIR)/$@ $< $(LOCAL_OBJS) $(SHREKXXFLAGS) -I $(SRC_DIR) $(LIBS) && ./$(BUILD_DIR)/$@

route-handler-test: route_handler_test.C $(SRC_DIR)/route_handler.H request-test 
	$(CXX) -o $(BUILD_DIR)/$@ $< $(LOCAL_OBJS) $(SHREKXXFLAGS) -I $(SRC_DIR) $(LIBS) && ./$(BUILD_DIR)/$@

server-test: server_test.C route-handler-test
	$(CXX) -o $(BUILD_DIR)/$@ $< $(LOCAL_OBJS) $(SHREKXXFLAGS) -I $(SRC_DIR) $(LIBS) && ./$(BUILD_DIR)/$@

libnetbridge: mock_network_bridge.C
	$(CXX) $< -o $(BUILD_DIR)/$@.so -fPIC -shared -ldl -D_GNU_SOURCE $(SHREKXXFLAGS)

libnetpreload: network_preload.C libnetbridge
	$(CXX) $< -o $(BUILD_DIR)/libnetpreload.so -lnetbridge -L $(BUILD_DIR) -fPIC -shared -ldl -D_GNU_SOURCE $(SHREKXXFLAGS)

network-test: netmocks_test.C libnetpreload
	$(CXX) -o $(BUILD_DIR)/$@ $< $(SHREKXXFLAGS) -I $(SRC_DIR) $(LIBS) -lnetbridge -L $(BUILD_DIR) \
&& LD_PRELOAD=$(BUILD_DIR)/libnetpreload.so:$(BUILD_DIR)/libnetbridge.so ./$(BUILD_DIR)/$@

test-application: test_main.C libs
	$(CXX) -o $(BUILD_DIR)/$@ $< $(LOCAL_OBJS) $(SHREKXXFLAGS) -I $(SRC_DIR) $(LIBS)


test-all: request-test response-test route-handler-test server-test network-test
