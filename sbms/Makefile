CXX = g++
CXXFLAGS = -std=c++17 -Wall -Isrc

SRC = $(wildcard src/*.cpp)
TARGET = sbms

all: $(TARGET)

$(TARGET): $(SRC)
	$(CXX) $(CXXFLAGS) $(SRC) -o $(TARGET)

clean:
	rm -f $(TARGET)

.PHONY: all clean
