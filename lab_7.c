#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <unistd.h>
#define O printf
#define FS(f, c) fscanf(f, "%c", &(c))
#define FP(g, s) fprintf(g, "%s\n", s)


/*  //////////////////////////////////////////////////////
 *  функции общего назначения
 */ //////////////////////////////////////////////////////

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
    /*  пока есть что читать    */
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

/*  устанавливает значение переменных по их адресу */
void flags(char *a, char *b, char *c, char i, char j, char k)
{
    *a = i; *b = j; *c = k;
}

/*  instead of realloc   */
void zero(char buf[], int n)
{
    for (int i = 0; i < n; buf[i++] = 0);
}
/*  функция для решения задания б   */
void solve_b(FILE *f, FILE *g)
{
    char i, j, k, v, c = '1';
    char buf[21];
    flags(&i, &j, &k, 0, 0, 0);
    /*  
     *  i, j, k -  показательные параметры    
     *      i   -  номер элемента буфера
     *      j   -  флаг для исполнения некоторых итераций
     *      k   -  длина строки
     */
    FS(f, c);

    while (!feof(f)) {
    /* отмотать пробелы, т. к. buf начинается с буквы */
        for (;!j && !feof(f) && (c == '\n' || c == ' '); ) {
            k = k > 20 ? k : ++k;
            if (c == '\n') 
                flags(&i, &j, &k, 0, j, 0);
            FS(f, c);
        }
        
        if (feof(f))
            break;
        flags(&i, &j, &k, i, 0, k);

        /* буфер набирается посимвольно     */
        for (; i < 21; i++) {
            buf[i] = c;
            if (c == '\n' || feof(f))
                break;
            if (i < 20) 
                FS(f, c);
            k = k > 20 ? k : ++k;
        }

        /*  1 cлучай  */ 
        /*  выход из-за конца строки    */
        /*  т. е. буфер заполнился не полностью*/
        if (c == '\n' || feof(f)) {
        /*  если длина слова больше 20, то буфер выписывается  */    
            if (k > 20) {
                buf[i] = '\0';
                FP(g, buf);
            }
            zero(buf, 21);
            flags(&i, &j, &k, 0, 0, 0);
        }
        else {
            /*  проверка на наличие пробелов в буфере   */
            for (i = 20; i >= 0 && buf[i] != ' '; ) 
                c = buf[i--];

            /*  2   случай   */
            /*  buf состоит из одного сплошного слова */
            /*  то есть он не удовлетворяет условия и не выписывается */
            if (i == -1) {
                c = buf[20];
                for (; c != ' ' && c != '\n' && !feof(f); FS(f, c))
                    k = k > 20 ? k : ++k;
                if (c == '\n')
                    flags(&i, &j, &k, i, j, 0);
                zero(buf, 21);
                flags(&i, &j, &k, 0, 0, k);
            }
            /*  3   случай  */
            /*  buf состоит из нескольких слов  */
            else {
            /*  выписывается весь буфер до последнего пробела */
                buf[i] = 0;
                FP(g, buf);
                v = ++i;
            /*  символы после последнего пробела сдвигаются в начало буфера */
                for (; i < 21; i++) 
                    buf[i - v] =  buf[i]; 
                FS(f, c);  
            /*  i устанавливантся на последний символ + 1 */
            /*  т. е. следующий набор буфера не перекроет символы,
             *  которые раньше шли после последнего пробела */
                flags(&i, &j, &k, 21 - v, 1, k);
            }
        }
    }
    fclose(f);
    fclose(g);
}
/*
 *  организация меню и всё такое     
 */
int main(int argc, char **argv) 
{
    srand(time(NULL));
    FILE *f, *g;

  /*  разборки и предупреждения по поводу генерации файла */
    switch(argc) {
    /*  решение а   */   
        case 2:
            if (access(argv[1], F_OK ) != -1) {
                if(menu("generate input file? [y/n] ") == 'y') 
                    gen_a(argv[1]);
            }
            else {
                O("input file will be generated\n");
                gen_a(argv[1]);
            }

            f = fopen(argv[1], "r+b");
            printfile(f);
            solve_a(f);
            O("done\n");
            break;
    /*  решение б   */   
        case 3:
            if (access(argv[1], F_OK) != -1) {
                if(menu("generate input file? [y/n] ") == 'y') {
                    gen_b(argv[1]);
                }
            }
            else {
                O("input file will be generated\n");
                gen_b(argv[1]);
            }

            if (access(argv[2], F_OK) != -1) 
                if(menu("WARNING: this output file already exists\ncontinue? [y/n] ") == 'n')
                    return 0;
            
            f = fopen(argv[1], "r");
            g = fopen(argv[2], "w+");

            solve_b(f, g);
            O("done\n");
            break;

        default:
            O("usage: %s <input> <output>\n", argv[0]);
        return 1;  
    }

    return 0;
}
