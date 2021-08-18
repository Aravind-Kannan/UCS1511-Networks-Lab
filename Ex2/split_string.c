#include <stdio.h>
#include <stdlib.h>

#include <string.h>

char* newFileName(char file[], char addnStr[])
{
  char* fileName;
  fileName = malloc(sizeof(strlen(file) + strlen(addnStr)));

  int j = 0;
  for(int i = 0; i <= strlen(file); i++, j++)
  {
    if(file[i] == '.')
    {
      for(int k = 0; k < strlen(addnStr); k++, j++)
      {
        fileName[j] = addnStr[k];
      }
    }
    fileName[j] = file[i];
  }
  return fileName;
}

int main()
{
  char string[256];

  scanf("%[^\n]%*c", string);

  char* res = newFileName(string, "_new");

  printf("%s\n%s\n", string, res);

  return 0;
}
