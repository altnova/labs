#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#define O printf


/*  generates file with double numbers   */
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

/*  makes insert into the file  */
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

/*  prints double numbers from file    */
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

void solve_a(FILE* f) 
{
    double val, mul = 1;
    int i, N_pos, N;
    printfile(f);
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

int main(int argc, char** argv)
{
    srand(time(NULL));
    FILE *f;
    char c;

    if (argc != 2) {
        O("ERROR\n");
        return 1;
    }

    f = fopen(argv[1], "r+");   

    if (f) {
        O("generate input file? [y/n] ");
        c = getchar();
        getchar();
        if (c == 'y')
            gen_a(argv[1]);
    }
    else {
        O("file will be generated\n");
        gen_a(argv[1]);
    }

    f = fopen(argv[1], "r+b");

    solve_a(f);
    return 0;
}