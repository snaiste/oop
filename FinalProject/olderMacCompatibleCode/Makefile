# # Makefile

CXX=g++
RM=rm -f
CPPWARNINGS=-pedantic -Wall -Wextra -Wcast-align -Wcast-qual -Wctor-dtor-privacy -Wdisabled-optimization -Wformat=2 -Winit-self -Wmissing-declarations -Wmissing-include-dirs -Wold-style-cast -Woverloaded-virtual -Wredundant-decls -Wshadow -Wsign-conversion -Wsign-promo -Wstrict-overflow=5 -Wswitch-default -Wundef -Wno-unused
CPPFLAGS=-std=c++0x $(CPPWARNINGS)

SRCS=project.cpp msg.cpp simulation.cpp menus.cpp main.cpp
OBJS=$(subst .cpp,.o,$(SRCS))

all: datahero

datahero: $(OBJS)
	$(CXX) -o all $(OBJS) -v

depend: .depend

.depend: $(SRCS)
	rm -f ./.depend
	$(CXX) $(CPPFLAGS) -MM $^>>./.depend;

clean:
	$(RM) $(OBJS)

dist-clean: clean
	$(RM) *~ .depend

include .depend