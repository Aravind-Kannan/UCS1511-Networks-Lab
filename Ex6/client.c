#include "common.h"
#include <math.h> //pow()

int calcNumberRedundantBits(int n)
{
    int r = 0;
    for(; pow(2,r) < (n + 1); r++);
    return r;
}

void invert(char message[], int i)
{
    if(message[i] =='1')
    {
        message[i] = '0';
    }
    else if(message[i] =='0')
    {
        message[i] = '1';
    }
}

int errorDetect(char message[], int m, int r, char check[])
{
    int flag = 0;
    int c = 0;
    for(int i = 1; i <= (int)pow(2,r-1); i *= 2)
    {
        int count = 0;
        for(int j = i; j <= m + r; j += 2*i)
        {
            int temp = j;
            
            for(int k = 1; k <= i && temp <= m + r; k++)
            {
                if(message[m + r - temp] == '1')
                {
                    count++;
                }
                temp++;
            }
        }

        //even parity
        if(count % 2 == 1) // if not, then do
        {
            flag = 1;
            check[c++] = '1';
        }
        else
        {
            check[c++] = '0';
        }
    }

    check[c] = '\0';

    return flag;
}

int main()
{
    char message[LIMIT], check[LIMIT];
    int errorBit = 0;

    printf("Received message: ");
    scanf(" %[^\n]%*c", message);

    int m = strlen(message);
    int r = calcNumberRedundantBits(m);

    m = m - r;

    printf("Number of message bits: %d\n", m);
    printf("Number of redundant bits: %d\n", r);

    printf("Message: %s\n", message);
    
    if(errorDetect(message, m, r, check) == 1)
    {
        printf("Error detected!\n");
        errorBit = (int)strtol(check, NULL, 2);
        printf("Error located at: %s [%d]\n", check, errorBit);
        invert(message, m + r - errorBit);
        printf("Corrected message: %s\n", message);
    }
    else
    {   
        printf("No Error!\n");
    }

    return 0;
}