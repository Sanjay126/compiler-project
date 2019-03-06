removePrevious:
	rm *.o *.out -f
compileLexer:
		gcc-5 lexer.c -c -g -o1 -o lexer.o 
compileParser:
		gcc-5 parser.c -c -g -o1 -o parser.o 
compileall:removePrevious compileLexer compileParser 
		gcc-5 -g -o1 lexer.o parser.o driver.c -o a.out

