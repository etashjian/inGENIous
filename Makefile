OBJDIR = obj
BINDIR = bin
SRCDIR = src
INCDIR = include
OBJS = Socket.o

CXX=g++
#CPPFLAGS= -Wall -ansi -g -O0 -std=c++11
CPPFLAGS= -Wall -ansi -O0 -std=c++11 -I $(INCDIR)
#CPPFLAGS= -Wall -ansi -O3 -std=c++11

#LFLAGS= -Wall -ansi -g -O0
LFLAGS= -Wall -ansi -O0
#LFLAGS= -Wall -ansi -O3


all: $(BINDIR)/Server $(BINDIR)/Client

$(BINDIR)/Server: $(OBJDIR)/$(OBJS) $(OBJDIR)/Server.o
	$(CXX) $(addprefix $(OBJDIR)/, $(OBJS) Server.o) -o $@ $(LFLAGS)

$(BINDIR)/Client : $(OBJDIR)/$(OBJS) $(OBJDIR)/Client.o
	$(CXX) $(addprefix $(OBJDIR)/, $(OBJS) Client.o) -o $@ $(LFLAGS)

$(OBJDIR)/%.o: $(SRCDIR)/%.cpp $(INCDIR)/*.h
	$(CXX) -c $< -o $@ $(CPPFLAGS)

clean:
	rm -rf $(BINDIR) $(OBJDIR)

$(shell mkdir -p $(BINDIR) $(OBJDIR))
