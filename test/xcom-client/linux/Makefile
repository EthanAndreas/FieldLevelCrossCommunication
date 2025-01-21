CC ?= gcc
CFLAGS ?= -g -O0 -ggdb -Wall -Wextra
LDLIBS ?=

INCLUDE_PATH = ../../../xcom

TARGET   = linux_client

SRCDIR   = ../../../xcom
OBJDIR   = obj
BINDIR   = build

SOURCES  := $(wildcard $(SRCDIR)/*.c)
INCLUDES := $(wildcard $(INCLUDE_PATH)/*.h)
OBJECTS  := $(SOURCES:$(SRCDIR)/%.c=$(OBJDIR)/%.o)

$(BINDIR)/$(TARGET): $(OBJECTS)
	mkdir -p $(BINDIR)
	cc -o obj/linux_client.o -c linux_client.c -g -O0 -ggdb -isystem../../../xcom
	$(CC) -o $@ $^ obj/linux_client.o $(CFLAGS) $(LDLIBS)
	@echo "Linking complete!"

$(OBJECTS): $(OBJDIR)/%.o : $(SRCDIR)/%.c $(INCLUDES)
	mkdir -p $(OBJDIR)
	$(CC) -o $@ -c $< $(CFLAGS) -isystem$(INCLUDE_PATH)


$(OBJDIR)/%.o: $(SRCDIR)/%.c $(INCLUDES)
	$(CC) -o $@ -c $< $(CFLAGS) -isystem$(INCLUDE_PATH)

.PHONY: clean cov

test:
	make clean && make && ./build/linux_client

clean:
	rm -f *.tar
	rm -f $(OBJDIR)/*.o
	rm -f $(OBJDIR)/*.gcda
	rm -f $(OBJDIR)/*.gcno
	rm -f $(BINDIR)/$(TARGET)