#include "aoc.h"

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <inttypes.h>



static int64_t i64ProcessLine(int64_t i64ParamLine);



int64_t i64ProcessLine(int64_t i64ParamLine)
{
  return i64ParamLine;
}


int main(int argc, char *argv[])
{
  char sLineBuf[LINE_BUF_SIZE] = { 0 };
  int64_t i64LineInput = 0;
  int64_t i64PrevWindow = 0;
  int64_t i64ThisWindow = 0;
  uint64_t u64Increases = 0;
  int64_t ai64Numbers[4] = {0};
  uint64_t u64LineNum = 0;
  uint64_t u64LineIndex = 0;
  
  FILE* pxFile = fopen("input.txt", "r");

  if (pxFile == NULL)
  {
    return 1;
  }
  
  while(fgets(sLineBuf, sizeof(sLineBuf) , pxFile) != NULL)
  {
    i64LineInput = atoll(sLineBuf);
    i64LineInput = i64ProcessLine(i64LineInput);
    
    u64LineIndex = u64LineNum % 4;
    ai64Numbers[u64LineIndex] = i64LineInput;

    if (u64LineNum >= 3)
    {
      // Actual counts
      i64PrevWindow =
	ai64Numbers[(u64LineIndex + 4 - 1) % 4] +
	ai64Numbers[(u64LineIndex + 4 - 2) % 4] +
	ai64Numbers[(u64LineIndex + 4 - 3) % 4];

      i64ThisWindow =
	ai64Numbers[(u64LineIndex + 4 - 0) % 4] +
	ai64Numbers[(u64LineIndex + 4 - 1) % 4] +
	ai64Numbers[(u64LineIndex + 4 - 2) % 4];

      //printf(">Prev: %" PRIi64  "  This %" PRIi64 "\n", i64PrevWindow, i64ThisWindow);

      if (i64ThisWindow > i64PrevWindow)
      {
	u64Increases++;
      }
    }
    u64LineNum++;
  }
  fclose(pxFile);

  printf(">%" PRIu64 "\n", u64Increases);

  return 0;
}
