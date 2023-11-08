WFLAGS = -Wall -Wextra -Wpedantic
OBJDIR = obj

test_sequence.out: $(OBJDIR)/main.o
	g++ $(WFLAGS) -g $(OBJDIR)/main.o -o test_sequence.out

$(OBJDIR)/main.o: main.cpp sequence.hpp
	g++ $(WFLAGS) -g -c main.cpp -o $(OBJDIR)/main.o

clean:
	rm -rf $(OBJDIR)/*.o test_sequence.out
