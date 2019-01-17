TARGET = sushi
OBJS = sushi.o sushi_read.o sushi_history.o

CFLAGS = -Wall -pedantic -Wextra -Wcast-align -Wcast-qual -Wdisabled-optimization -Wformat=2 -Winit-self -Wlogical-op -Wmissing-include-dirs -Wredundant-decls -Wshadow -Wsign-conversion -Wstrict-overflow=5 -Wswitch-default -Wundef -Werror -Wno-unused #-O3

all: $(TARGET)

sushi: $(OBJS)
	$(CC) $(OBJS) -o $(TARGET)

sushi.o: sushi.c sushi.h

sushi_read.o: sushi_read.c sushi.h

sushi_history.o: sushi_history.c sushi.h

clean:
	rm -f $(OBJS) $(TARGET)
