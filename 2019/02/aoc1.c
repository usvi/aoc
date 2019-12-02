#include <stdio.h>
#include <stdlib.h>

#include "aoc.h"

#define ARRAY_SIZE (500U)
#define OPCODE_ADD (1)
#define OPCODE_MUL (2)
#define OPCODE_HALT (99)


void ProcessArray(i32* ai32ParamArr, u16* pu16Pos)
{
  i32 i32Opcode = ai32ParamArr[*pu16Pos];

  if (i32Opcode == OPCODE_HALT)
  {
    return;
  }

  i32 i32Arg1 = ai32ParamArr[ai32ParamArr[(*pu16Pos) + 1]];
  i32 i32Arg2 = ai32ParamArr[ai32ParamArr[(*pu16Pos) + 2]];
  i32 i32ResPos = ai32ParamArr[(*pu16Pos) + 3];
  
  if (i32Opcode == OPCODE_ADD)
  {
    ai32ParamArr[i32ResPos] = i32Arg1 + i32Arg2;
  }
  else
  {
    ai32ParamArr[i32ResPos] = i32Arg1 * i32Arg2;
  }
  *pu16Pos += 4;
}



int main()
{
  i32 ai32Arr[ARRAY_SIZE] = { 0 };
  i32 i32Temp = 0;
  u16 u16Pos = 0;
  FILE* pxFile = fopen("input.txt", "r");

  if (pxFile == NULL)
  {
    return 1;
  }
  while(fscanf(pxFile, "%i", &i32Temp) == 1)
  {
    fseek(pxFile, 1, SEEK_CUR);
    ai32Arr[u16Pos] = i32Temp;
    u16Pos++;
    i32Temp = 0;
  }
  fclose(pxFile);

  ai32Arr[1] = 12;
  ai32Arr[2] = 2;
  u16Pos = 0;

  while(ai32Arr[u16Pos] != OPCODE_HALT)
  {
    ProcessArray(ai32Arr, &u16Pos);
  }
  printf(">%i\n", ai32Arr[0]);

  return 0;
}
