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
  uint32_t u32PrevWindow = 0;
  uint32_t u32ThisWindow = 0;
  uint32_t u32Increases = 0;
  uint32_t au32Numbers[4] = {0};
  uint32_t u32LineNum = 0;
  uint32_t u32LineIndex = 0;
  
  FILE* pxFile = fopen("input.txt", "r");

  if (pxFile == NULL)
  {
    return 1;
  }
  
  while(fgets(sLineBuf, sizeof(sLineBuf) , pxFile) != NULL)
  {
    u32LineInput = atoi(sLineBuf);
    u32LineIndex = u32LineNum % 4;

    au32Numbers[u32LineIndex] = u32LineInput;

    if (u32LineNum >= 3)
    {
      // Actual counts
      u32PrevWindow =
	au32Numbers[(u32LineIndex + 4 - 1) % 4] +
	au32Numbers[(u32LineIndex + 4 - 2) % 4] +
	au32Numbers[(u32LineIndex + 4 - 3) % 4];

      u32ThisWindow =
	au32Numbers[(u32LineIndex + 4 - 0) % 4] +
	au32Numbers[(u32LineIndex + 4 - 1) % 4] +
	au32Numbers[(u32LineIndex + 4 - 2) % 4];

      //printf(">Prev: %u  This %u\n", u32PrevWindow, u32ThisWindow);

      if (u32ThisWindow > u32PrevWindow)
      {
	u32Increases++;
      }
    }
    u32LineNum++;
  }
  fclose(pxFile);

  printf(">%u\n", u32Increases);

  return 0;
}
