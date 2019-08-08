CXX=g++
LIBTOOL=ar
LIBFLAGS=rvs
CXXFLAGS=-lws2_32
SRC=Socket.cpp Peer.cpp Packet.cpp
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


Socket.cpp: Socket.hpp
Peer.cpp: Peer.hpp
Packet.cpp: Packet.hpp


.PHONY: clean

clean:
	@$(RMCMD) *.o