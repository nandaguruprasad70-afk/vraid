CXX = g++
CXXFLAGS = -std=c++17 -O2 -Wall -Wextra -I./include -pthread
CXXFLAGS += -DDEBUG 
LDFLAGS = -pthread

SRCS = src/vdisk.cpp src/raid5.cpp src/controller.cpp src/main.cpp
OBJS = $(SRCS:.cpp=.o)
TARGET = raidctl

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CXX) $(CXXFLAGS) -o $@ $^ $(LDFLAGS)

%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c -o $@ $<

clean:
	rm -f $(OBJS) $(TARGET) disk*.img spare.img

test: $(TARGET)
	./$(TARGET) init && ./$(TARGET) status && ./$(TARGET) test

.PHONY: all clean test
