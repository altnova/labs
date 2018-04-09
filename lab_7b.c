#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#define O printf
#define FS(f, c) fscanf(f, "%c", &(c))
#define FP(g, s) fprintf(g, "%s\n", s)


/*  функция для генерации файла задания б   */
void gen_b(const char *s) 
{
	unsigned int i, j, count, length;
	char c;
	FILE* f = fopen(s, "w");
	count = rand() % 15;
	for (i = 0; i < count; i++) {
		length = rand() % 5;
		for (j = 0; j < length; j++) 
			for (c = '0'; c <= '9'; c++) {
				putc(c, f);
				if (!(rand() % 17))
					putc(' ', f);
			}
		putc('\n', f);
	}
	fclose(f);
}


void sublines(FILE *f, FILE *g)
{
	char *buf, c = '1';
	int i = 0, j = 0, k = 0, l = 0, _i;
	buf = calloc(21, sizeof(char) * 21);
	FS(f, c);

	while (!feof(f)) {
		/* rewind all spaces */
		for (; c == '\n' || c == ' '; FS(f,c)) {
			k++;
			if (c == '\n') {
				k = 0;
				i = 0;
			}
			if (feof(f)) {
				break;
			}
		}

		/*      get buf     */
		for (; i < 21; i++) {
			buf[i] = c;
			if (c == '\n' || feof(f))
				break;
			FS(f, c);
			k++;
		}

		/*  1   1   1   1   1   */ 
		/*  exit with break (end of line)   */
		if (c == '\n' || feof(f)) {
			if (k > 20) {
				buf[i] = '\0';
				FP(g, buf);
			}
		
			buf = calloc(21, sizeof(char) * 21);
			i = 0; 
			k = 0;
		}
		else {
			for (i = 20; i >= 0 && c != ' '; i--)
				c = buf[i];

			/*  2   2   2   2   2   */
			/*  buf is full of symbols  */
			if (i == -1) {
				c = buf[20];
				for (; c != ' ' && c != '\n' && !feof(f); FS(f, c))
					k++;
				if (c == '\n')
					k = 0;

				buf = calloc(21, sizeof(char));    
				i = 0;
			}

			/*  3   3   3   3   3   */
			/*  few words in buf    */
			else {
				buf[++i] = '\0';
				FP(g, buf);
				i++;
				_i = i;
				for (; i < 21; i++) 
					buf[i - _i] =  buf[i]; 
				i = 21 - _i;
			}
		}
	}
}


int main() 
{
	srand(time(NULL));
	char c='b', *ff="f.txt", *gg="g.txt";
	FILE *f, *g;

	f = fopen(ff, "r");
	g = fopen(gg, "w+");
	sublines(f, g);
	fclose(f);
	fclose(g);

	O("done\n");

	return 0;
}