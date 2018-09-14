.cpp.o:
	gcc -std=c++11 -c $<

jrasm: jrasm.o Common.o Token.o Tokenizer.o Parser.o Generator.o MmlParser.o CJRFormat.o Generator_M6800.o Expr.o
	gcc -o jrasm $+ -lc++

clean:
	rm jrasm *.o
