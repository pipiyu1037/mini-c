bison -d -v parser.ypp
flex lex.l
gcc -c lex.yy.c
g++ -c parser.tab.cpp displayAST.cpp sematic.cpp mid.cpp GenObject.cpp
g++ -o parser parser.tab.o displayAST.o lex.yy.o sematic.o mid.o GenObject.o
del *.o