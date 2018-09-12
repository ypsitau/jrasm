.cpp.o:
	gcc -c $<

jrasm: jrasm.o Token.o Tokenizer.o
	gcc -o jrasm $+ -lc++
