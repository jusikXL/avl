WFLAGS = -Wall -Wextra -Wpedantic
OBJDIR = obj

test.out: $(OBJDIR)/main.o
	g++ $(WFLAGS) -g $(OBJDIR)/main.o -o test.out

$(OBJDIR)/main.o: main.cpp ring.hpp
	g++ $(WFLAGS) -g -c main.cpp -o $(OBJDIR)/main.o

clean:
	rm -rf $(OBJDIR)/*.o test.out
