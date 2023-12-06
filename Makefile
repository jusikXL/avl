WFLAGS = -Wall -Wextra -Wpedantic
OBJDIR = obj

test.out: $(OBJDIR)/main.o $(OBJDIR)/bi_ring_test.o
	g++ -fsanitize=address -fsanitize=undefined $(WFLAGS) -g $(OBJDIR)/main.o $(OBJDIR)/bi_ring_test.o -o test.out

$(OBJDIR)/main.o: main.cpp bi_ring.h
	g++ -fsanitize=address -fsanitize=undefined $(WFLAGS) -g -c main.cpp -o $(OBJDIR)/main.o

$(OBJDIR)/bi_ring_test.o: bi_ring_test.cpp bi_ring_test.h
	g++ -fsanitize=address -fsanitize=undefined $(WFLAGS) -g -c bi_ring_test.cpp -o $(OBJDIR)/bi_ring_test.o

clean:
	rm -rf $(OBJDIR)/*.o test.out
