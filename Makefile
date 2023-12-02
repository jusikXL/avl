WFLAGS = -Wall -Wextra -Wpedantic
OBJDIR = obj

test.out: $(OBJDIR)/main.o
	g++ -fsanitize=address -fsanitize=undefined $(WFLAGS) -g $(OBJDIR)/main.o -o test.out

$(OBJDIR)/main.o: main.cpp bi_ring_test.cpp bi_ring.h bi_ring_test.h
	g++ -fsanitize=address -fsanitize=undefined $(WFLAGS) -g -c main.cpp -o $(OBJDIR)/main.o

clean:
	rm -rf $(OBJDIR)/*.o test.out
