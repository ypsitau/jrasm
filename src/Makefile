.cpp.o:
	gcc -std=c++11 -c $<

jrasm: jrasm.o Common.o Token.o Tokenizer.o Parser.o MmlParser.o CJRFormat.o InstInfo.o Expr.o
	gcc -o jrasm $+ -lc++

clean:
	rm jrasm *.o
