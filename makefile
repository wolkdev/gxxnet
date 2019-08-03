CXX=g++
LIBTOOL=ar
LIBFLAGS=rvs
CXXFLAGS=
SRC=
OBJ=$(SRC:.cpp=.o)
LIB=libgxxnet.a

ifeq ($(OS), Windows_NT)

	RMCMD=del

else

	RMCMD=rm -rf

endif

all: $(LIB)

$(LIB): $(OBJ)
	$(LIBTOOL) $(LIBFLAGS) $(LIB) $^

%.o: %.cpp
	$(CXX) -g -o $@ -c $< $(CXXFLAGS)

.PHONY: clean

clean:
	@$(RMCMD) *.o