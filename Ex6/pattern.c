#include <stdio.h>
#include <math.h>

int main()
{
    int m = 7, r = 4;
    
    for(int i = 1; i <= (int)pow(2,r-1); i *= 2)
    {
        printf("\ni: %d\n", i);
        for(int j = i; j <= m + r; j += 2*i)
        {
            int temp = j;
            for(int k = 1; k <= i && temp <= m+r; k++)
            {
                // printf("(%d) %d ",temp, m + r - temp);
                printf("%d ", m + r - temp);
                temp++;
            }
        }
    }

    return 0;
}
