#include <stdio.h>
//#include <omp.h>

int** result(int l1, int c1,int A[l1][c1] , int l2, int c2,int B[l2][c2])
{
    if(c1!=l2)
    {
        return 0;
    }
    int result[l1][c2];

    int i=0;
    int j=0;
    int k=0;

    for(i=0;i<l1;i++)
    {
        for(j=0;j<l2;j++)
        {
            result[i][j]=0;
        }
    }
    for(i=0;i<l1;i++)
    {
        j=0;
        for(j=0;j<c2;j++)
        {
            k=0;
            for(k=0;k<l2;k++)
            {
                result[i][j]+=A[i][k]*B[k][j];
            }
        }
    }

    i=0;
    j=0;

    for(i=0;i<l1;i++)
    {
        for(j=0; j<c2;j++)
        {
            //printf("ok");
            printf("%d",result[i][j]);
            printf(" ");
        }
        printf("\n");
    }

    return result;
}

int main()
{
    {
        int mat1[2][3] = {
                {1, 2, 3},
                {4, 5, 6}
        };

        int mat2[3][2] = {
                {7,8},
                {9,10},
                {11,12}
        };
        result(2,3,mat1,3,2,mat2);
    }
    return 0;
}
