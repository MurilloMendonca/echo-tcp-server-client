# Project directories
SRCDIR := src
INCDIR := include
BUILDDIR := build
BINDIR := $(BUILDDIR)/bin
OBJDIR := $(BUILDDIR)/obj

# Compiler and flags
CC := g++
CFLAGS := -Wall -I$(INCDIR) -std=c++11

# Source files
COMMON_SRCS := $(wildcard common/$(SRCDIR)/*.cpp)
CLIENT_SRCS := $(wildcard client/$(SRCDIR)/*.cpp)
SERVER_SRCS := $(wildcard server/$(SRCDIR)/*.cpp)

# Object files
COMMON_OBJS := $(patsubst common/$(SRCDIR)/%.cpp, $(OBJDIR)/common/%.o, $(COMMON_SRCS))
CLIENT_OBJS := $(patsubst client/$(SRCDIR)/%.cpp, $(OBJDIR)/client/%.o, $(CLIENT_SRCS))
SERVER_OBJS := $(patsubst server/$(SRCDIR)/%.cpp, $(OBJDIR)/server/%.o, $(SERVER_SRCS))

# Executables
CLIENT_EXE := $(BINDIR)/client
SERVER_EXE := $(BINDIR)/server

.PHONY: all clean

all: $(CLIENT_EXE) $(SERVER_EXE)

$(CLIENT_EXE): $(COMMON_OBJS) $(CLIENT_OBJS)
	@mkdir -p $(@D)
	$(CC) $^ -o $@

$(SERVER_EXE): $(COMMON_OBJS) $(SERVER_OBJS)
	@mkdir -p $(@D)
	$(CC) $^ -o $@

$(OBJDIR)/common/%.o: common/$(SRCDIR)/%.cpp
	@mkdir -p $(@D)
	$(CC) $(CFLAGS) -c $< -o $@

$(OBJDIR)/client/%.o: client/$(SRCDIR)/%.cpp
	@mkdir -p $(@D)
	$(CC) $(CFLAGS) -c $< -o $@

$(OBJDIR)/server/%.o: server/$(SRCDIR)/%.cpp
	@mkdir -p $(@D)
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	@rm -rf $(BUILDDIR)

