lab7b:
	rm -f g.txt
	gcc -w -g -Werror lab_7b.c -o lab7b

lab7:
	rm -f g.txt
	gcc -w -g -Werror lab_7.c -o lab7

clean:
	rm -f g.txt
	rm -rf lab7 lab7b *.dSYM
