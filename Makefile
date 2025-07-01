CXX := g++
CXXFLAGS := -Wall -Wextra -O3 -std=c++17
LDFLAGS :=

OUTDIR := ./bin
ASSEMBLY := json_parse

SRC := $(wildcard ./src/*.cpp)
OBJ := $(SRC:.cpp=.o)
DEPS := $(SRC:.cpp=.d)
-include $(DEPS)

all: $(ASSEMBLY)

$(ASSEMBLY): $(OBJ)
	$(CXX) $(OBJ) $(LDFLAGS) -o $(OUTDIR)/$@ 

%.o: %.cpp
	$(CXX) -MMD -MP -c $(CXXFLAGS) $< -o $@

run: $(ASSEMBLY)
	$(OUTDIR)/$(ASSEMBLY)	

clean:
	rm $(OUTDIR)/$(ASSEMBLY) $(OBJ) $(DEPS)


.PHONY: all clean run