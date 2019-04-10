compileAll:removePrevious compileLexer compileParser compilerAST compileSymbolTable
		gcc-5 lexer.o parser.o ast.o symbolTable.o driver.c -g -o stage1exe
removePrevious:
	rm *.o *.out -f
compileLexer:
	gcc-5 lexer.c -c -g -o lexer.o 
compileParser:
	gcc-5 parser.c -c -g -o parser.o 
compilerAST:
	gcc-5 ast.c -c -g -o ast.o 
compileSymbolTable:
	gcc-5 symbolTable.c -c -g -o symbolTable.o
