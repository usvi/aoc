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
  int64_t i64Prev = 0;
  uint64_t u64Increases = 0;
  
  FILE* pxFile = fopen("input.txt", "r");

  if (pxFile == NULL)
  {
    return 1;
  }
  while(fgets(sLineBuf, sizeof(sLineBuf) , pxFile) != NULL)
  {
    i64LineInput = atoll(sLineBuf);
    i64LineInput = i64ProcessLine(i64LineInput);

    //printf("Comparing this: %" PRIi64 " and prev %" PRIi64 "\n", i64LineInput, i64Prev);
    
    if (i64LineInput > i64Prev)
    {
      u64Increases++;
    }
    i64Prev = i64LineInput;
  }
  fclose(pxFile);

  // First is not counted
  if (u64Increases > 0)
  {
    u64Increases--;
  }
  printf(">%" PRIu64 "\n", u64Increases);

  return 0;
}
