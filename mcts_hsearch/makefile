
######### Compiler and flags ######### 
CC=gcc
CFLAGS=-std=c11 -g -O2 -fopenmp
TARGET=search
TARGET_TEST=test
SRCDIR = src
CFGDIR = $(SRCDIR)/cfg
OBJDIR = obj
TSTDIR = tst

# Object files for source
OBJS = 	$(OBJDIR)/config.o \
		$(OBJDIR)/matrix_game.o \
		$(OBJDIR)/conditions.o \
		$(OBJDIR)/shared_memory.o \
		$(OBJDIR)/mcts.o \
		$(OBJDIR)/parser.o \
		$(OBJDIR)/lexer.o \
		$(OBJDIR)/main.o

# Object files for test
TEST_OBJS = $(OBJDIR)/test_main.o \
			$(OBJDIR)/test_framework.o \
			$(OBJDIR)/test_conditions.o \
			$(OBJDIR)/conditions.o

.PHONY: all clean


######### Source Targets ######### 
all: clean $(TARGET) $(TARGET_TEST)


# Main Executable
$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) -o $@ $^ -lm -lfl

# Test Executable
$(TARGET_TEST): $(TEST_OBJS)
	$(CC) $(CFLAGS) -o $@ $^ -lm


# Compile source files
$(OBJDIR)/%.o: $(SRCDIR)/%.c $(SRCDIR)/%.h
	@mkdir -p $(OBJDIR)
	$(CC) $(CFLAGS) -c -o $@ $<

$(OBJDIR)/main.o: $(SRCDIR)/main.c
	@mkdir -p $(OBJDIR)
	$(CC) $(CFLAGS) -c -o $@ $<

# Compile config parser files
$(OBJDIR)/%.o: $(CFGDIR)/%.c $(CFGDIR)/%.h
	@mkdir -p $(OBJDIR)
	$(CC) $(CFLAGS) -c -o $@ $<

$(OBJDIR)/parser.o: $(CFGDIR)/parser.tab.c $(CFGDIR)/parser.tab.h
	@mkdir -p $(OBJDIR)
	$(CC) $(CFLAGS) -c -o $@ $<

$(CFGDIR)/parser.tab.c $(CFGDIR)/parser.tab.h: $(CFGDIR)/parser.y
	bison -d $(CFGDIR)/parser.y -o $(CFGDIR)/parser.tab.c

$(OBJDIR)/lexer.o: $(CFGDIR)/lexer.c
	@mkdir -p $(OBJDIR)
	$(CC) $(CFLAGS) -c -o $@ $<

$(CFGDIR)/lexer.c: $(CFGDIR)/lexer.fl
	flex -o $(CFGDIR)/lexer.c $(CFGDIR)/lexer.fl

# Compile test files
$(OBJDIR)/%.o: $(TSTDIR)/%.c $(TSTDIR)/%.h
	@mkdir -p $(OBJDIR)
	$(CC) $(CFLAGS) -c -o $@ $<

$(OBJDIR)/test_main.o: $(TSTDIR)/test_main.c
	@mkdir -p $(OBJDIR)
	$(CC) $(CFLAGS) -c -o $@ $<

######### Clean up ######### 
clean:
	rm -f $(TARGET) $(TARGET_TEST)
	rm -f *~
	rm -f *.o
	rm -f $(CFGDIR)/parser.tab.c
	rm -f $(CFGDIR)/parser.tab.h
	rm -f $(CFGDIR)/lexer.c
	rm -f a.out
	rm -rf $(OBJDIR)
