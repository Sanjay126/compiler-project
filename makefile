compileLexer:
		gcc lexer.c -o lexer.o -c -g
compileParser:
		gcc parser.c -o parser.o -c -g 
compileall:compileLexer compileParser
		gcc lexer.o parser.o driver.c -g

