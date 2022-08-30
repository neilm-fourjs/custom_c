
all: mylib.so main.42m 

main.42m: main.4gl
	fglcomp main.4gl

mylib.so: mylib.c
	fglmkext -o $@ $^

run: mylib.so main.42m
	fglrun main

clean:
	rm -f *.so *.42m
