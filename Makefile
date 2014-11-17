OBJDIR = obj
BINDIR = bin
SRCDIR = src
INCDIR = include
OBJS = Socket.o Scheduler.o

CXX=g++
#CPPFLAGS= -Wall -ansi -g -O0 -std=c++11 -I $(INCDIR)
CPPFLAGS= -Wall -ansi -O0 -std=c++11 -I $(INCDIR)
#CPPFLAGS= -Wall -ansi -O3 -std=c++11 -I $(INCDIR)

#LFLAGS= -Wall -ansi -g -O0 -pthread
LFLAGS= -Wall -ansi -O0 -pthread
#LFLAGS= -Wall -ansi -O3 -pthread


all: $(BINDIR)/Server $(BINDIR)/Client $(BINDIR)/SimpleClient $(BINDIR)/SimpleServer

$(BINDIR)/Server: $(addprefix $(OBJDIR)/, $(OBJS) Server.o)
	$(CXX) $(addprefix $(OBJDIR)/, $(OBJS) Server.o) -o $@ $(LFLAGS)

$(BINDIR)/Client : $(addprefix $(OBJDIR)/, $(OBJS) Client.o)
	$(CXX) $(addprefix $(OBJDIR)/, $(OBJS) Client.o) -o $@ $(LFLAGS)

$(BINDIR)/SimpleClient : $(addprefix $(OBJDIR)/, $(OBJS) SimpleClient.o)
	$(CXX) $(addprefix $(OBJDIR)/, $(OBJS) SimpleClient.o) -o $@ $(LFLAGS)

$(BINDIR)/SimpleServer : $(addprefix $(OBJDIR)/, $(OBJS) SimpleServer.o)
	$(CXX) $(addprefix $(OBJDIR)/, $(OBJS) SimpleServer.o) -o $@ $(LFLAGS)

$(OBJDIR)/%.o: $(SRCDIR)/%.cpp $(INCDIR)/*.h
	$(CXX) -c $< -o $@ $(CPPFLAGS)

clean:
	rm -rf $(BINDIR) $(OBJDIR)

$(shell mkdir -p $(BINDIR) $(OBJDIR))
