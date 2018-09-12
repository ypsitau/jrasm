.cpp.o:
	gcc -c $<

jrasm: jrasm.o Token.o Tokenizer.o MmlParser.o
	gcc -o jrasm $+ -lc++
