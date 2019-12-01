#include <stdio.h>
#include <stdlib.h>

#include "aoc.h"


u32 u32ProcessLine(u32 u32ParamLine)
{
  return u32ParamLine;
}


int main()
{
  char sLineBuf[LINE_BUF_SIZE] = { 0 };
  u32 u32TotalSum = 0;
  u32 u32LineInput = 0;

  FILE* pxFile = fopen("input.txt", "r");

  if (pxFile == NULL)
  {
    return 1;
  }
  while(fgets(sLineBuf, sizeof(sLineBuf) , pxFile) != NULL)
  {
    u32LineInput = atoi(sLineBuf);
    u32TotalSum += u32ProcessLine(u32LineInput);
  }
  fclose(pxFile);

  printf(">%u\n", u32TotalSum);

  return 0;
}
