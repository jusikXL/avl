WFLAGS = -Wall -Wextra -Wpedantic
OBJDIR = obj

test_sequence.out: $(OBJDIR)/main.o $(OBJDIR)/sequence.o
	g++ $(WFLAGS) -g $(OBJDIR)/main.o $(OBJDIR)/sequence.o -o test_sequence.out

$(OBJDIR)/main.o: main.cpp sequence.hpp
	g++ $(WFLAGS) -g -c main.cpp -o $(OBJDIR)/main.o

$(OBJDIR)/sequence.o: sequence.cpp sequence.hpp
	g++ $(WFLAGS) -g -c sequence.cpp -o $(OBJDIR)/sequence.o

clean:
	rm -rf $(OBJDIR)/*.o test_sequence
