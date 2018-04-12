#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#define O printf
#define FS(f, c) fscanf(f, "%c", &(c))
#define FP(g, s) fprintf(g, "%s\n", s)

char menu(const char *hint) 
{
    char c;
    O("%s", hint);
    c = getchar();
    getchar(); // отлавливаем нажатие на enter
    return c;
}

/*  generates input file   */
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
                if (!(rand() % 17)) {
                    putc(' ', f);
                    c = '0' - 1;
                }
            }
        putc('\n', f);
    }
    fclose(f);
}

/*  instead of realloc   */
char* zero(char *buf, int len)
{
    for (int i = 0; i < len; i++)
        buf[i] = 0;
    return buf;
}

void flags(char *a, char *b, char *c, char i, char j, char k)
{
    *a = i; *b = j; *c = k;
}

void solve_b(FILE *f, FILE *g)
{
    char i, j, k , v, c = '1';
    char *buf = calloc(21, sizeof(char));
    flags(&i, &j, &k, 0, 0, 0);

    FS(f, c);

        while (!feof(f)) {
        /* rewind spaces because buf must start with letter */
        for (;!j && !feof(f) && (c == '\n' || c == ' '); ) {
            k = k > 20 ? k : ++k;
            if (c == '\n') 
                flags(&i, &j, &k, 0, j, 0);
            FS(f, c);
        }
        
        if (feof(f))
            break;
        flags(&i, &j, &k, i, 0, k);

        /*      get buf     */
        for (; i < 21; i++) {
            buf[i] = c;
            if (c == '\n' || feof(f))
                break;
            if (i < 20) 
                FS(f, c);
            k = k > 20 ? k : ++k;
        }

        /*  1   1   1   1   1   */ 
        /*  exit because of end of line    */
        if (c == '\n' || feof(f)) {
        /*  if word length is > 20  */    
            if (k > 20) {
                buf[i] = '\0';
                FP(g, buf);
            }
            buf = zero(buf, 21);
            flags(&i, &j, &k, 0, 0, 0);
        } 
        else {
            /*  checking existence of spaces    */
            for (i = 20; i >= 0 && buf[i] != ' '; ) 
                c = buf[i--];
                
            /*  2   2   2   2   2   */
            /*  entire buf is a word */
            if (i == -1) {
                c = buf[20];
                for (; c != ' ' && c != '\n' && !feof(f); FS(f, c))
                    k = k > 20 ? k : ++k;
                if (c == '\n')
                    flags(&i, &j, &k, i, j, 0);
                buf = zero(buf, 21);
                flags(&i, &j, &k, 0, 0, k);
            }
            /*  3   3   3   3   3   */
            /*  buf contains a few words  */
            else {
                buf[i] = 0;
                FP(g, buf);
                v = ++i;
                for (; i < 21; i++) 
                    buf[i - v] =  buf[i]; 
                FS(f, c);  
                flags(&i, &j, &k, 21 - v, 1, k);
            }
        }
    }
    fclose(f);
    fclose(g);
}

int main(int argc, char **argv)
{
	srand(time(NULL));
	char c;
	FILE *f, *g;

    if (argc != 3) {
        O("usage: %s infile outfile\n", argv[0]);
        return 1;
    }

    f = fopen(argv[1], "r");

    if(f && menu("generate input file? [y/n] ") == 'y') 
        gen_b(argv[1]);
    else if (!f) {
        O("input file will be generated\n");
        gen_b(argv[1]);
    }

    f = fopen(argv[1], "r");
    g = fopen(argv[2], "w+");

    if (g && menu("WARNING: this output file already exists\ncontinue? [y/n] ") == 'n') {
        fclose(g);
        fclose(f);
        return 0;
    } 

    solve_b(f, g);
	return 0;
}
