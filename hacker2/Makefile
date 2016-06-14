# Compiler
CC = clang
# Flags
FLAGS = -ggdb3 -O0 -std=c11 -Wall -Werror -Wshadow
# Libs
LIBS = -lcs50 -lcrypt -lm


crack: crack.c Makefile
	$(CC) $(FLAGS) crack.c -o crack $(LIBS)

crackN: crackN.c Makefile
	$(CC) $(FLAGS) crackN.c -o crackN $(LIBS)
	
crackU: crackU.c Makefile
	$(CC) $(FLAGS) crackU.c -o crackU $(LIBS)
	
crackL: crackL.c Makefile
	$(CC) $(FLAGS) crackL.c -o crackL $(LIBS)

crackA: crackA.c Makefile
	$(CC) $(FLAGS) crackA.c -o crackA $(LIBS)