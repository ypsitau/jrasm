.cpp.o:
	gcc -c $<

jrasm: jrasm.o
	gcc -o jrasm $+ -lc++
