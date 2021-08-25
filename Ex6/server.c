#include "common.h"
#include <math.h> //pow()

int calcNumberRedundantBits(int m)
{
    int r = 0;
    for(; pow(2,r) < (m + r + 1); r++);
    return r;
}

void modifyMessage(char message[], int m, int r)
{
    char modifiedMessage[LIMIT];

    int redundantBit = r - 1;
    int msgIndex = 0;
    int i = 0;

    for(; i < m + r; i++)
    {
        // printf("%d %d %d %d\n", i, m + r, (int)pow(2,redundantBit), (i == m - pow(2,redundantBit)));

        if(i == m + r - pow(2,redundantBit))
        {
            modifiedMessage[i] = '0';
            redundantBit--;
        }
        else
        {
            modifiedMessage[i] = message[msgIndex++];
        }
    }

    modifiedMessage[i] = '\0';

    strcpy(message, modifiedMessage);
    // printf("Modified Message: %s\n", modifiedMessage);
}

void calcRedundantBits(char message[], int m, int r)
{
    for(int i = 1; i <= (int)pow(2,r-1); i *= 2)
    {
        int count = 0;

        // printf("\ni: %d\n", i);

        for(int j = i; j <= m + r; j += 2*i)
        {
            int temp = j;
            
            for(int k = 1; k <= i && temp <= m + r; k++)
            {
                if(message[m + r - temp] == '1')
                {
                    count++;
                }
                // printf("%d ", m + r - temp);
                temp++;
            }
        }

        //even parity
        if(count % 2 == 1) // if not, then do
        {
            // printf("(%d [%d])", i, m + r - i);
            message[m + r - i] = '1';
        }
    } 
}
    

int main()
{
    char message[LIMIT];

    printf("Enter message: ");
    scanf(" %[^\n]%*c", message);

    int m = strlen(message);
    int r = calcNumberRedundantBits(m);

    printf("Number of message bits: %d\n", m);
    printf("Number of redundant bits: %d\n", r);

    printf("Message: %s\n", message);
    modifyMessage(message, m, r);
    printf("After Message: %s\n", message);

    calcRedundantBits(message, m, r);

    printf("Parity calc Message: %s\n", message);

    return 0;
}