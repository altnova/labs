#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

void debug()
{
    printf("boom\n");
}

/*  //////////////////////////////////////////////////////
 *  функции общего назначения
 */ //////////////////////////////////////////////////////

/*  проверка на принадлежность алфавиту */
int in_abc(char c)
{
    if (c < 'A')
        return 0;
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
    printf("%s", hint);
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
        if (temp) {
		  fwrite(&temp, sizeof(double), 1, f);
          // printf("%.2lf ", temp);
        }
	}
    // printf("\n");
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
            printf("%.2lf ", val);
    }
    printf("\n");
    rewind(f);
}

/*  функция для решения задания a   */
void solve_a(FILE* f) 
{
    double val, mul = 1;
    int i, N_pos, N;
    printf("position?\n");
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
    // debug();
    while (1) {
        s = realloc(s, sizeof(char)*(i + 1));
        c = getc(f);
        if (c == '\n') {
            // s[i] = '\0';
            break;
        }
        s[i] = c;
        i++;
        if (feof(f)) {    
            s = realloc(s, sizeof(char) * (i + 1));
            // s[i] = '\0';
            break;
        }
    }
    s[i] = '\0';
    // printf("%s\n", s);
    return s;
}

/*  разделение на подстроки одной строки с сохр. пробелов    */
char** sublines(char *s)
{
    char c = '1', **l, b;
    int i, j, k = 0;
    l = malloc(sizeof(char*));
 
    // debug();
    for (i = 0; c != '\0'; i++) {
        // debug();
        l = realloc(l, sizeof(char*)*(i + 2));
        l[i] = malloc(sizeof(char));
        l[i + 1] = malloc(sizeof(char));

        if (!i) 
            c = s[k++];
        else
            c = b;
        // printf("%c ", c);
        if (c == '\0') 
            break;

        if (c == ' ') 
            for (j = 0; c == ' '; j++) {
                l[i] = realloc(l[i], sizeof(char)*(j + 2));
                l[i][j] = c;
                c = s[k];
                k++;
            }
        else 
            for (j = 0; c != ' ' && c != '\0'; j++) {
                l[i] = realloc(l[i], sizeof(char)*(j + 2));
                l[i][j] = c;
                c = s[k];
                k++;
            }
        b = c;
        l[i][j] = '\0';
        // printf("1%s1\n", l[i]);
    }
    // printf("%d\n", i);
    l[i][0] = '\0';
    return l;
}

/*  вывод подстрок в файл в соответствии с условиями    */
void print_sublines(char **l, FILE *g)
{
    int i = 0, j;
   
    while (l[i][0] != 0) {
        // debug();
        if (strlen(l[i]) < 21 || l[i][0] == ' ') {
            // debug();
            // printf("1%s1\n", l[i]);
            fprintf(g, "%s", l[i]);
            // debug();
        }
        else 
            for (j = 0; j < strlen(l[i]); j++) 
                fprintf(g, " ");
        // debug();
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
        // printf("%d\n", strlen(s));
        if (strlen(s) > 20) {
            // debug();
            l = sublines(s);
            // debug();
            print_sublines(l, g);
            // debug();
        }
        // debug();
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

/*
    c = menu("hit a or b: ");
    if (c != 'a' && c != 'b') {
        printf("ERROR: unknown command\n");
        return 1;     
    }

    ff = malloc(sizeof(char) * 30);
    printf("name of input file?\n");
    scanf("%s", ff);
    getchar();
    f = fopen(ff, "r");
    if (f) {
        printf("WARNING: this file already exists\n");
        if (menu("generate input file with this name? [y/n]") == 'y') {
            if (c == 'a')
                gen_a(ff);
            else 
                gen_b(ff); 
        }
    } else {
        printf("file will be generated\n");
        if (c == 'a')
            gen_a(ff);
        else
            gen_b(ff);  
    } 
    fclose(f);
*/

    /*  разборки и предупреждения по поводу генерации файла */
    switch(c) {
    /*  решение а   */
        case 'a':
            f = fopen(ff, "r+b");
            printfile(f);
            solve_a(f);
            printf("done\n");
            break;
    /*  решение б   */
        case 'b':
            f = fopen(ff, "r");
            /*
            gg = malloc(sizeof(char) * 30);
            printf("name of output file?\n");
            scanf("%s", gg);
            getchar();
            */

            g = fopen(gg, "w+");

/*
            if (g && menu("WARNING: this output file already exists\ndo you want to stop? [y/n] ") == 'y') {
                fclose(g);
                fclose(f);
                free(ff);
                free(gg);
                return 0;
            }

            //free(gg);
*/
            solve_b(f, g);
            printf("done\n");
            break;
    }

    //free(ff);
    return 0;
}