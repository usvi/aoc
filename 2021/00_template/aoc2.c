#include "aoc.h"

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <inttypes.h>
#include <string.h>


int main(int argc, char *argv[])
{
  char sLineBuf[LINE_BUF_SIZE] = { 0 };
  int64_t i64LineInput = 0;

  int64_t i64Forward = 0;
  int64_t i64Depth = 0;
  int64_t i64Aim = 0;

  FILE* pxFile = fopen(argv[argc - 1], "r");

  if (pxFile == NULL)
  {
    printf("Unable to open input file %s\n", argv[argc - 1]);
    
    return 1;
  }
  while(fgets(sLineBuf, sizeof(sLineBuf) , pxFile) != NULL)
  {
    if (strstr(sLineBuf, "forward") == sLineBuf)
    {
      i64LineInput = atoll(strstr(sLineBuf, " ") + 1);
      i64Forward += i64LineInput;
      i64Depth += i64LineInput * i64Aim;
    }
    else if (strstr(sLineBuf, "down") == sLineBuf)
    {
      i64LineInput = atoll(strstr(sLineBuf, " ") + 1);
      i64Aim += i64LineInput;
    }
    else if (strstr(sLineBuf, "up") == sLineBuf)
    {
      i64LineInput = atoll(strstr(sLineBuf, " ") + 1);
      i64Aim -= i64LineInput;
    }
    
  }
  fclose(pxFile);

  //printf(">%" PRIi64 "\n", i64TotalSum);

  printf(">%" PRIi64 "  %" PRIi64 "  %" PRIi64 "\n", i64Forward, i64Depth, (i64Forward * i64Depth));

  return 0;
}
