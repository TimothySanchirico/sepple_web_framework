SRC = ../src/*.C *.C
OBJ = $(SRC:.C=.o)

SHREKXXFLAGS = -std=c++17 -ggdb3 -O3 -fno-rtti -fno-exceptions -fno-omit-frame-pointer

BUILD_DIR = ../build/
LOCAL_OBJS = $(BUILD_DIR)/http-request.o $(BUILD_DIR)/http-common.o $(BUILD_DIR)/http-response.o $(BUILD_DIR)/tcp-server.o $(BUILD_DIR)/gzip.o
SRC_DIR = ../src/
LIBS = -lgtest -lgtest_main -lpthread -lgmock -lz -lbenchmark

http: http_benchmarks.C
	$(CXX) http_benchmarks.C $(LOCAL_OBJS) $(SHREKXXFLAGS) $(LIBS) -I $(SRC_DIR) -o benchmark_http


