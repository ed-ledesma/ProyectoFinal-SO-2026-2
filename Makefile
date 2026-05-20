CC      = gcc
CFLAGS  = -g -I./include/
LIBS    = -lncurses
TARGET  = build/top
SRCS    = src/main.c src/memory.c src/ui.c src/uptime.c src/network.c

$(TARGET): $(SRCS)
	$(CC) $(CFLAGS) $(SRCS) -o $(TARGET) $(LIBS)

clean:
	rm -f build/top

run:
	chmod +r build/top && ./build/top

.PHONY: clean
