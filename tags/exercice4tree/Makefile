target := vico_miguelangel_4

pathSRC := code
pathINC := include
pathOBJ := .objects

pSRC := $(wildcard $(pathSRC)/*.cpp)
SRC := $(notdir $(pSRC))
OBJ := $(SRC:.cpp=.o)
pOBJ := $(addprefix $(pathOBJ)/,$(OBJ))
DEP := $(addprefix .,$(SRC:.cpp=.d))

dummy := $(shell test -d $(pathOBJ) || mkdir -p $(pathOBJ))

CXXFLAGS += -I $(pathINC) -Wall -Wextra -Wshadow -Wwrite-strings -Wswitch-enum \
            -Wswitch-default -ansi -pedantic -std=c++98
LDFLAGS += -lglut -lGLEW

vpath %.h $(pathINC)
vpath %.cpp $(pathSRC)
vpath %.o $(pathOBJ)

all: $(target)

$(target): $(OBJ)
	@echo "Linking the program."
	@$(CXX) -o $@ $(LDFLAGS) $(pOBJ)

# Inclusion of the generated dependencies
-include $(DEP)

# Automatic generation of dependencies
$(DEP): .%.d : %.cpp
	@$(CXX) -MM -I$(pathINC) $< | sed -e 's!^\(.*\)\.o:!.\1.d \1.o:!' > $@

$(OBJ): %.o: %.cpp
	@echo "Compiling $<"
	@$(CXX) $(CXXFLAGS) -o $(pathOBJ)/$@ -c $< 

.PHONY: clean
clean:
	@$(RM) -f $(pOBJ) $(DEP) $(target)
