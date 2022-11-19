#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include <stdbool.h>
#include <math.h>
#include <omp.h>

const int rows =5;
const int max_time=10;
char initial[rows][rows];
int temp[rows][rows];
bool checked[rows][rows];
bool go[rows];

const char* getfield(char* line, int num)
{
    const char* tok;
    for (tok = strtok(line, ";");
         tok && *tok;
         tok = strtok(NULL, ";\n"))
    {
        if (!--num)
            return tok;
    }
    return NULL;
}

void disp_mat(int l, int c,char mat[l][c])
{
    for(int i=0;i<l;i++)
    {
        for(int j=0; j<c;j++)
        {
            printf("%c",mat[i][j]);
            printf(" ");
        }
        printf("\n");
    }
}

void disp_mat_int(int l, int c,int mat[l][c])
{
    for(int i=0;i<l;i++)
    {
        for(int j=0; j<c;j++)
        {
            printf("%d",mat[i][j]);
            printf(" ");
        }
        printf("\n");
    }
}

int count_rows()
{
    FILE* count = fopen("/Users/adm/CLionProjects/factory/grid.csv", "r");
    char l[1024];
    int rows=0;
    while (fgets(l, 1024, count))
    {
        rows++;
    }
    return rows;
}

void create_mat()
{
    for(int i=0;i<rows;i++)
    {
        for(int j=0;j<rows;j++)
        {
            initial[i][j]=0;
        }
    }

    FILE* stream = fopen("/Users/adm/CLionProjects/factory/grid.csv", "r");
    char line[1024];

    int i=0;
    while (fgets(line, 1024, stream))
    {
        char* tmp = strdup(line);
        for(int j =0; j<rows;j++)
        {
            char s = tmp[2*j];
            int f=0;
            initial[i][j]=s;
        }
        i++;
    }
    disp_mat(rows,rows,initial);
}

void* measure(void *arg)
{
    int i = (int) arg;
    for(int j=0; j<rows;j++)
    {
        int f=0;
        if(initial[i][j]=='H')
        {
            temp[i][j]=rand()%40+60;
            checked[i][j]=1;
        }
        if(initial[i][j]=='C')
        {
            temp[i][j]=rand()%60;
            checked[i][j]=1;
        }
    }
}

void measure2()
{
    for(int r = 0;r<rows;r++)//(int) arg;
    {
        for (int i = 0; i < rows; i++) {
            int f = 0;
            if (initial[r][i] == 'H') {
                temp[r][i] = rand() % 40 + 60;
                checked[r][i] = 1;
            }
            if (initial[r][i] == 'C') {
                temp[r][i] = rand() % 60;
                checked[r][i] = 1;
            }
        }
    }
}

void* check(void* arg)
{
    for(int i=0;i<rows;i++)
    {
        bool b=1;
        for(int j=0; j<rows;j++)
        {
            if(!checked[i][j])
            {
                b=0;
            }
        }
        go[i]=b;
    }
}

void heat_equation()
{
    double alpha =2;
    double delta_x = 1;
    double delta_t = (pow(delta_x,2))/(4 * alpha);
    double gamma = (alpha * delta_t) / (pow(delta_x,2));

#pragma omp parralel for private(i,j,k) shared(temp)
    for(int k=0;k<max_time;k++)
    {
        for(int i=1;i<rows-1;i++)
        {
            for(int j=1;j<rows-1;j++)
            {
                temp[i][j]= gamma*(temp[i+1][j]+temp[i-1][j]+temp[i][j+1]+temp[i][j-1]-4*temp[i][j])+temp[i][j];
            }
        }
        printf("\n");
        disp_mat_int(rows,rows,temp);
    }
}

int main()
{
    create_mat();
    for(int i=0;i<rows;i++)
    {
        for(int j=0;j<rows;j++)
        {
            temp[i][j]=0;
            checked[i][j]=0;
        }
    }

    pthread_t threads[rows];

    for(int i=0; i<rows; i++){
        pthread_create(&threads[i], NULL, measure, (void *) i);
    }

    for(int i=0; i<rows; i++){
        pthread_join(threads[i], NULL);
    }
    printf("\n");
    disp_mat_int(rows,rows,temp);
    heat_equation();
}