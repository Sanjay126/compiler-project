compileAll:removePrevious compileLexer compileParser 
		gcc-5 lexer.o parser.o driver.c -o stage1exe
removePrevious:
	rm *.o *.out -f
compileLexer:
		gcc-5 lexer.c -c -o lexer.o 
compileParser:
		gcc-5 parser.c -c -o parser.o 

