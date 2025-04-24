CC = gcc
CFLAGS = -I./Include -Wall -Wextra -g
ifeq ($(DEBUG),1)
    CFLAGS += -O0
else
    CFLAGS += -O3
endif
LDFLAGS = -lSDL2 -lSDL2_ttf
SRCDIR = src
OBJDIR = obj
SANDBOX_SRC = sandbox.c

SRCS = $(wildcard $(SRCDIR)/*.c)
OBJS = $(SRCS:$(SRCDIR)/%.c=$(OBJDIR)/%.o)
TARGET = cbc

.PHONY: all clean directories sandbox rom rom-clean

all: clean directories $(TARGET)

sandbox: $(SANDBOX_SRC)
	$(CC) $(SANDBOX_SRC) -o sandbox $(LDFLAGS)
	./sandbox

rom:
	$(MAKE) -C default_rom


$(TARGET): $(OBJS)
	$(CC) $(OBJS) -o $@ $(LDFLAGS)

$(OBJDIR)/%.o: $(SRCDIR)/%.c
	$(CC) $(CFLAGS) -c $< -o $@

directories: 
	mkdir -p $(OBJDIR)

clean:
	rm -rf $(OBJDIR) $(TARGET) sandbox 