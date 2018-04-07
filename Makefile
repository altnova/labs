lab7:
	rm -f g.txt
	gcc -w -g -Werror lab_7.c -o lab7

clean:
	rm -f g.txt
	rm -rf lab7 *.dSYM
