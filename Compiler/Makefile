CC = gcc

CFLAGS = -g -Wall

LIBS = -lfl

YACC = bison

LEX = flex

SOURCE = parse.y main.c scan.c tree.c eval.c

OBJECTS = parse.o main.o scan.o tree.o eval.o

see: $(OBJECTS)
	$(CC) $(CFLAGS) $(OBJECTS) $(LIBS) -o see

y.tab.h parse.c: parse.y
	$(YACC) -y -t -v -d parse.y
	mv y.tab.c parse.c

scan.c: scan.lex	
	${LEX} -o $@ $<

sacn.o: scan.c y.tab.h

eval.o: eval.c eval.h

tree.o: tree.c tree.h

clean:
	-rm -f *.o parse.c y.output scan.c y.tab.h see
