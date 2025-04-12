CXX = g++

CXXFLAGS=-O3 -std=c++11 -Wall -Wextra -g

OUTDIR=build
SRCDIR=src

CP_EXEC = $(OUTDIR)/cp
MV_EXEC = $(OUTDIR)/mv

MV_SRC = $(SRCDIR)/mv.cpp
CP_SRC = $(SRCDIR)/cp.cpp

all: $(CP_EXEC) $(MV_EXEC)

clean: 
	rm -v $(OUTDIR)/*

$(CP_EXEC): $(CP_SRC)
	$(CXX) $(CXXFLAGS) -o $(CP_EXEC) $(CP_SRC) $(SRCDIR)/utils.cpp

$(MV_EXEC): $(MV_SRC)
	$(CXX) $(CXXFLAGS) -o $(MV_EXEC) $(MV_SRC) $(SRCDIR)/utils.cpp
