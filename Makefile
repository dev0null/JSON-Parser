CXXFLAGS := -Wall -Wextra -O3
LDFLAGS := 
OUT := json_parse

SRC := $(wildcard ./src/*.cpp)
OBJ := $(SRC:.cpp=.o)


all: $(OUT)

$(OUT): $(OBJ)
	g++ $^ -o $@ $(LDFLAGS) 

%.o: %.cpp
	g++ -c $(CXXFLAGS) $< -o $@

run: $(OUT)
	./$(OUT)	

clean:
	rm $(OUT) $(OBJ)


.PHONY: all clean run