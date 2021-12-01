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
  uint32_t u32LineInput = 0;
  uint32_t u32Prev = 0;
  uint32_t u32Increases = 0;

  
  FILE* pxFile = fopen("input.txt", "r");

  if (pxFile == NULL)
  {
    return 1;
  }
  while(fgets(sLineBuf, sizeof(sLineBuf) , pxFile) != NULL)
  {
    u32LineInput = atoi(sLineBuf);

    if (u32LineInput > u32Prev)
    {
      u32Increases++;
    }
    u32Prev = u32LineInput;
  }
  fclose(pxFile);

  // First is not counted
  if (u32Increases > 0)
  {
    u32Increases--;
  }
  printf(">%u\n", u32Increases);

  return 0;
}
