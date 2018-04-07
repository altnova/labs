#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#define O printf

/*  //////////////////////////////////////////////////////
 *  функции общего назначения
 */ //////////////////////////////////////////////////////

/*  проверка на принадлежность алфавиту */
int in_abc(char c)
{
    if (c >= 'A' && c <= 'Z')
        return 1;
    if (c >= 'a' && c <= 'z')
        return 1;
    return 0;
}

/*  функция, которая просит пользователя что-то нажать   */
char menu(const char *hint) 
{
    char c;
    O("%s", hint);
    c = getchar();
    getchar(); // отлавливаем нажатие на enter
    return c;
}

/*  //////////////////////////////////////////////////////
 *  функции для задания а    
 */ //////////////////////////////////////////////////////

/*  функция, которая генерирует файл для задания a   */
void gen_a(const char *s) 
{
	unsigned int i, count;
	double temp;
	FILE *f = fopen(s, "wb");
	count = (rand() % 9) + 2;
	for (i = 0; i < count; i++) {
		temp = rand() % 1000 / 100.0 - 3;
        if (temp) 
		  fwrite(&temp, sizeof(double), 1, f);
	}
	fclose(f);
}

/*  функция, которая делает вставку посреди файла   */
void finsert(FILE* f, double data) 
{
    double temp;
    int result;
    unsigned int pos = ftell(f);
    result = fread(&temp, sizeof(double), 1, f) == 1;
    fseek(f, pos, SEEK_SET);
    fwrite(&data, sizeof(double), 1, f);
    if (result)
        finsert(f, temp);
}

/*  функция вывода файла double на экран    */
void printfile(FILE* f) 
{
    double val;
    while (!feof(f)) {
        fread(&val, sizeof(double), 1, f);
        if (!feof(f))
            O("%.2lf ", val);
    }
    O("\n");
    rewind(f);
}

/*  функция для решения задания a   */
void solve_a(FILE* f) 
{
    double val, mul = 1;
    int i, N_pos, N;
    O("position?\n");
    scanf("%d", &N);
    for (i = 1; fread(&val, sizeof(double), 1, f) == 1; i++) {
        mul *= val;
        if (i <= N) 
            N_pos = ftell(f);  
    }
    fseek(f, N_pos, SEEK_SET);
    finsert(f, mul);
    rewind(f);

    printfile(f);
    fclose(f);
}

/*  //////////////////////////////////////////////////////
 *  функции для задания б
 */  //////////////////////////////////////////////////////

/*  функция для генерации файла задания б   */
void gen_b(const char *s) 
{
    unsigned int i, j, count, length;
    char c;
    FILE* f = fopen(s, "w");
    count = rand() % 15;
    for (i = 0; i < count; i++) {
        length = rand() % 45;
        for (j = 0; j < length; j++) {
            for (c = rand()%'{'; !in_abc(c); c = rand()%'{');
            putc(c, f);
            if (!(rand() % 17))
                putc(' ', f);
        }
        putc('\n', f);
    }
    fclose(f);
}

char* get_line(FILE *f)
{
    int i = 0;
    char c, *s;
    s = malloc(sizeof(char));
    while (1) {
        s = realloc(s, sizeof(char)*(i + 1));
        c = getc(f);
        if (c == '\n') {
            break;
        }
        s[i] = c;
        i++;
        if (feof(f)) {    
            s = realloc(s, sizeof(char) * (i + 1));
            break;
        }
    }
    s[i] = '\0';
    return s;
}

/*  разделение на подстроки одной строки с сохр. пробелов    */
char** sublines(char *s)
{
    char c = '1', **l, b;
    int i, j, k = 0;
    l = malloc(sizeof(char*));

    for (i = 0; c != '\0'; i++) {
        l = realloc(l, sizeof(char*)*(i + 2));
        l[i] = malloc(sizeof(char));
        l[i + 1] = malloc(sizeof(char));
        c = i?b:s[k++];

        if (c == '\0') 
            break;

        if (c == ' ') 
            for (j = 0; c == ' '; j++) {
                l[i] = realloc(l[i], sizeof(char)*(j + 2));
                l[i][j] = c;
                c = s[k++];
            }
        else 
            for (j = 0; c != ' ' && c != '\0'; j++) {
                l[i] = realloc(l[i], sizeof(char)*(j + 2));
                l[i][j] = c;
                c = s[k++];
            }
        b = c;
        l[i][j] = '\0';
    }
    l[i][0] = '\0';
    return l;
}

/*  вывод подстрок в файл в соответствии с условиями    */
void print_sublines(char **l, FILE *g)
{
    int i = 0, j;
    while (l[i][0] != 0) {
        if (strlen(l[i]) < 21 || l[i][0] == ' ') 
            fprintf(g, "%s", l[i]);
        else 
            for (j = 0; j < strlen(l[i]); j++) 
                fprintf(g, " ");
        free(l[i]);
        i++;
    }
    fprintf(g, "\n");
    free(l);
}

/*  функция для решения задания б   */
void solve_b(FILE *f, FILE *g)
{
    char **buf, *s, **l;
    int i, j, k, n;
    
    while (!feof(f)) {
        s = get_line(f);
        if (strlen(s) > 20) {
            l = sublines(s);
            print_sublines(l, g);
        }
        free(s);
    }
    fclose(f);
    fclose(g);
}

/*
 *  организация меню и всё такое     
 */
int main() 
{
    srand(time(NULL));
    char c='b', *ff="f.txt", *gg="g.txt";
    FILE *f, *g;

  /*  разборки и предупреждения по поводу генерации файла */
    c = menu("hit a or b: ");
    if (c != 'a' && c != 'b') {
        O("ERROR: unknown command\n");
        return 1;     
    }

    ff = malloc(sizeof(char) * 30);
    O("name of input file?\n");
    scanf("%s", ff);
    getchar();
    f = fopen(ff, "r");
    if (f) {
        O("WARNING: this file already exists\n");
        if (menu("generate input file with this name? [y/n]") == 'y') {
            if (c == 'a')
                gen_a(ff);
            else 
                gen_b(ff); 
        }
    } else {
        O("file will be generated\n");
        if (c == 'a')
            gen_a(ff);
        else
            gen_b(ff);  
    } 
    fclose(f);


    switch(c) {
    /*  решение а   */
        case 'a':
            f = fopen(ff, "r+b");
            printfile(f);
            solve_a(f);
            O("done\n");
            break;
    /*  решение б   */
        case 'b':
            f = fopen(ff, "r");

            gg = malloc(sizeof(char) * 30);
            O("name of output file?\n");
            scanf("%s", gg);
            getchar();
            
            g = fopen(gg, "w+");
            free(gg);

            if (g && menu("WARNING: this output file already exists\ndo you want to stop? [y/n] ") == 'y') {
                fclose(g);
                fclose(f);
                free(ff);
                return 0;
            }

            solve_b(f, g);
            O("done\n");
            break;
    }

    free(ff);
    return 0;
}