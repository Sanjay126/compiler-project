compileAll:removePrevious compileLexer compileParser compilerAST
		gcc-5 lexer.o parser.o ast.o driver.c -g -o stage1exe
removePrevious:
	rm *.o *.out -f
compileLexer:
	gcc-5 lexer.c -c -g -o lexer.o 
compileParser:
	gcc-5 parser.c -c -g -o parser.o 
compilerAST:
	gcc-5 ast.c -c -g -o ast.o 

