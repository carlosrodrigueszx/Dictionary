CXX = g++
CXXFLAGS = -std=c++17 -Wall -Iinclude

SRC = src/main.cpp \
      src/TextProcessor.cpp \
      src/Utils.cpp

OBJ = $(SRC:.cpp=.o)
OUT = freq

all: $(OUT)

$(OUT): $(OBJ)
	$(CXX) $(OBJ) -o $(OUT)

clean:
	rm -f $(OBJ) $(OUT)

.PHONY: all clean
