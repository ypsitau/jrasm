.cpp.o:
	gcc -std=c++11 -c $<

jrasm: jrasm.o Token.o Tokenizer.o MmlParser.o
	gcc -o jrasm $+ -lc++

clean:
	rm jrasm *.o
