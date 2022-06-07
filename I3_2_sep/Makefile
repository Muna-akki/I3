CC = gcc
BINDIR = bin
INCLUDEDIR = include
LIBDIR = lib
SRCDIR = src

CFLAGS = -Wall -I$(INCLUDEDIR)
LDLIBS = -lm

SRCS = $(wildcard $(SRCDIR)/*.c)
OBJS = $(SRCS:%.c=%.o)
TARGET = $(BINDIR)/I3_2

$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) -o $@ $^ $(LDFLAGS) $(LDLIBS)

.PHONY: tmpclean clean

tempclean:
	rm -f $(SRCDIR)/*~ *~
clean: tmpclean
	rm -f $(TARGET) $(OBJS)













