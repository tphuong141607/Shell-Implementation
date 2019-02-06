TARGET = sushi
OBJS = sushi.o sushi_read.o sushi_history.o sushi_parse.o \
	sushi_yyparser.tab.o lex.yy.o

CFLAGS = -Wall -pedantic -Wextra -Wcast-align -Wcast-qual \
	-Wdisabled-optimization -Wformat=2 -Winit-self \
	-Wmissing-include-dirs -Wredundant-decls -Wshadow \
	-Wstrict-overflow=5 -Wundef -Werror -Wno-unused \
	-g #-O3 -Wlogical-op -Wsign-conversion -Wswitch-default 

all: $(TARGET)

sushi: $(OBJS)
	$(CC) $(OBJS) -o $(TARGET)

sushi.o: sushi.c sushi.h

sushi_read.o: sushi_read.c sushi.h

sushi_history.o: sushi_history.c sushi.h

sushi_parse.o: sushi_yyparser.tab.h

sushi_yyparser.tab.c sushi_yyparser.tab.h: sushi_yyparser.y
	bison -d sushi_yyparser.y

sushi_yyparser.tab.o: sushi_yyparser.tab.c sushi.h

lex.yy.o: lex.yy.c sushi.h sushi_yyparser.tab.h

lex.yy.c: sushi_yylexer.l
	flex -I sushi_yylexer.l 

clean:
	rm -f $(OBJS) $(TARGET) lex.yy.c\
		sushi_yyparser.tab.c sushi_yyparser.tab.h
