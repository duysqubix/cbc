CC = gcc
CFLAGS = -I./Include -Wall -Wextra
SRCDIR = src
OBJDIR = obj
SANDBOX_SRC = sandbox.c

SRCS = $(wildcard $(SRCDIR)/*.c)
OBJS = $(SRCS:$(SRCDIR)/%.c=$(OBJDIR)/%.o)
TARGET = cbc

.PHONY: all clean directories sandbox 

all: directories $(TARGET)

sandbox: $(SANDBOX_SRC)
	$(CC) $(SANDBOX_SRC) -o sandbox
	./sandbox


$(TARGET): $(OBJS)
	$(CC) $(OBJS) -o $@

$(OBJDIR)/%.o: $(SRCDIR)/%.c
	$(CC) $(CFLAGS) -c $< -o $@

directories:
	mkdir -p $(OBJDIR)

clean:
	rm -rf $(OBJDIR) $(TARGET) sandbox 