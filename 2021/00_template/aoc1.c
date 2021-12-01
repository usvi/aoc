#include "aoc.h"

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <inttypes.h>


int64_t i64ProcessLine(int64_t i64ParamLine)
{
  return i64ParamLine;
}


int main(int argc, char *argv[])
{
  char sLineBuf[LINE_BUF_SIZE] = { 0 };
  int64_t i64TotalSum = 0;
  int64_t i64LineInput = 0;

  FILE* pxFile = fopen("input.txt", "r");

  if (pxFile == NULL)
  {
    return 1;
  }
  while(fgets(sLineBuf, sizeof(sLineBuf) , pxFile) != NULL)
  {
    i64LineInput = atoll(sLineBuf);
    i64TotalSum += i64ProcessLine(i64LineInput);
  }
  fclose(pxFile);
  printf(">%" PRIi64 "\n", i64TotalSum);

  return 0;
}
