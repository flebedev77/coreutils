CXX = g++

CXXFLAGS=-O3 -std=c++17 -Wall -Wextra -g

OUTDIR=build
OBJDIR=build/objs
SRCDIR=src

CP_EXEC = $(OUTDIR)/cp
MV_EXEC = $(OUTDIR)/mv

MV_SRC = $(SRCDIR)/mv.cpp
CP_SRC = $(SRCDIR)/cp.cpp

OBJS = $(OBJDIR)/utils.o

all: $(CP_EXEC) $(MV_EXEC)

clean: 
	rm -v $(CP_EXEC) $(MV_EXEC) $(OBJS)

$(CP_EXEC): $(CP_SRC) $(OBJS)
	$(CXX) $(CXXFLAGS) -o $(CP_EXEC) $(CP_SRC) $(OBJS)

$(MV_EXEC): $(MV_SRC) $(OBJS)
	$(CXX) $(CXXFLAGS) -o $(MV_EXEC) $(MV_SRC) $(OBJS)

$(OBJDIR)/%.o: $(SRCDIR)/%.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@
