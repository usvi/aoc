#include "aoc.h"

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>



uint32_t u32ProcessLine(uint32_t u32ParamLine)
{
  return u32ParamLine;
}


int main()
{
  char sLineBuf[LINE_BUF_SIZE] = { 0 };
  uint32_t u32TotalSum = 0;
  uint32_t u32LineInput = 0;

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
