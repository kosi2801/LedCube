SHELL = /bin/bash
TARGET = cube

ifneq ($(MAKECMDGOALS),release)
	BC=debug
else
	BC=release
endif

ifndef BC
	BC=debug
endif

CC = g++
CFLAGS = -Wall -lncurses
DEFINES = 
INCPATH = -Iinclude/

ifeq ($(BC),debug)
	CFLAGS += -g3 
else
	CFLAGS += -funroll-loops -flto -O3
endif

DEPDIR=$(BC)/deps
OBJDIR=$(BC)/objs
SRCDIR=src/

# Build a list of the object files to create, based on the .cpps we find
OTMP = $(patsubst $(SRCDIR)%.cxx,%.o,$(wildcard $(SRCDIR)*.cxx))

# Build the final list of objects
OBJS = $(patsubst %,$(OBJDIR)/%,$(OTMP))

# Build a list of dependency files
DEPS = $(patsubst %.o,$(DEPDIR)/%.d,$(OTMP))

debug: all

release: all

all: init $(OBJS)
	$(CC) $(DEFINES) $(CFLAGS) -o $(TARGET) $(OBJS)

init:
	mkdir -p $(DEPDIR)
	mkdir -p $(OBJDIR)

# Pull in dependency info for our objects
-include $(DEPS)

# Compile and generate dependency info
# 1. Compile the .cpp file
# 2. Generate dependency information, explicitly specifying the target name
# 3. The final three lines do a little bit of sed magic. The following
#    sub-items all correspond to the single sed command below:
#    a. sed: Strip the target (everything before the colon)
#    b. sed: Remove any continuation backslashes
#    c. fmt -1: List words one per line
#    d. sed: Strip leading spaces
#    e. sed: Add trailing colons
$(OBJDIR)/%.o : $(SRCDIR)%.cxx
	$(CC) $(DEFINES) $(CFLAGS) $(INCPATH) -o $@ -c $<
	$(CC) -MM -MT $(OBJDIR)/$*.o $(DEFINES) $(CFLAGS) $(INCPATH) $(SRCDIR)$*.cxx > $(DEPDIR)/$*.d
	@cp -f $(DEPDIR)/$*.d $(DEPDIR)/$*.d.tmp
	@sed -e 's/.*://' -e 's/\\$$//' < $(DEPDIR)/$*.d.tmp | fmt -1 | sed -e 's/^ *//' -e 's/$$/:/' >> $(DEPDIR)/$*.d
	@rm -f $(DEPDIR)/$*.d.tmp

clean:
	rm -fr debug/*
	rm -fr release/*
	rm -fr $(TARGET)
