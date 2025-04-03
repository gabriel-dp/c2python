# Makefile by gabriel-dp!

# Defines C++ Compiler
CXX := g++

# Final file name
BINARY := compiler

# Code directory structure
BINDIR := bin
BUILDDIR := build
INCDIR := include	
SRCDIR := src

# Compiler flags
CXXFLAGS := -Wall -I $(INCDIR) -std=c++17

# Linker flags
LDFLAGS :=

# %.o file names
NAMES := $(notdir $(basename $(wildcard $(SRCDIR)/*.cpp)))
OBJECTS :=$(patsubst %,$(BUILDDIR)/%.o,$(NAMES))

# Rule for link and generate the binary file
all: $(OBJECTS)
	@ if [ ! -d ./$(BINDIR) ]; then mkdir -p $(BINDIR); fi
	$(CXX) -o $(BINDIR)/$(BINARY) $+ $(LDFLAGS)

# Rule for object binaries compilation
$(BUILDDIR)/%.o: $(SRCDIR)/%.cpp
	@ if [ ! -d ./$(BUILDDIR) ]; then mkdir -p $(BUILDDIR); fi
	$(CXX) -c $^ -o $@ $(CXXFLAGS)

# Clean BIN and BUILD dirs
.PHONY: clean
clean:
	rm -rf $(BUILDDIR) $(BINDIR) $(BINDIR)/$(BINARY)
