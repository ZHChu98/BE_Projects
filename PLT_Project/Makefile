FLAGS = -Wall -Werror -pedantic -std=c++11 -g -fsanitize=address

OBJ = analyseur_lexical.o analyseur_syntaxique.o analyseur_semantique.o util.o

EXEC = compile_automate Executeur

all: $(EXEC) clean_OBJ

compile_automate: compile_automate.o $(OBJ)
	gcc $(FLAGS) -o $@ $^

Executeur: Executeur.o util.o
	gcc $(FLAGS) -o $@ $^

analyseur_lexical.o: analyseur_lexical.c analyseur_lexical.h util.h
	cc -c $<

analyseur_syntaxique.o: analyseur_syntaxique.c analyseur_syntaxique.h util.h
	cc -c $<

analyseur_semantique.o: analyseur_semantique.c analyseur_semantique.h util.h
	cc -c $<

compile_automate.o: compile_automate.c analyseur_lexical.h analyseur_syntaxique.h \
	analyseur_semantique.h util.h
	cc -c $<

Executeur.o: Executeur.c util.h
	cc -c $<

util.o: util.c util.h
	cc -c $<

clean:
	rm -rf *.o $(EXEC)

clean_OBJ:
	rm -rf *.o