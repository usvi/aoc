#include <stdio.h>
#include <stdlib.h>

#include "aoc.h"


u32 u32FuelFromMass(u32 u32ParamMass)
{
  uint32_t u32ResultFuel = 0;

  u32ResultFuel = (u32ParamMass / 3) - 2;

  return u32ResultFuel;
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
    u32TotalSum += u32FuelFromMass(u32LineInput);
  }
  fclose(pxFile);

  printf(">%u\n", u32TotalSum);

  return 0;
}
