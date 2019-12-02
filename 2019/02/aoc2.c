#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "aoc.h"

#define ARRAY_SIZE (500U)
#define OPCODE_ADD (1)
#define OPCODE_MUL (2)
#define OPCODE_HALT (99)
#define TARGET_NUM (19690720)
#define INPUT_MAX (99U)

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
  i32 ai32OrigArr[ARRAY_SIZE] = { 0 };
  i32 ai32TryArr[ARRAY_SIZE] = { 0 };
  i32 i32Temp = 0;
  u16 u16Pos = 0;
  FILE* pxFile = fopen("input.txt", "r");
  i32 i = 0;
  i32 j = 0;
  
  if (pxFile == NULL)
  {
    return 1;
  }
  while(fscanf(pxFile, "%i", &i32Temp) == 1)
  {
    fseek(pxFile, 1, SEEK_CUR);
    ai32OrigArr[u16Pos] = i32Temp;
    u16Pos++;
    i32Temp = 0;
  }
  fclose(pxFile);


  for (i = 0; i <= INPUT_MAX; i++)
  {
    for (j = 0; j <= INPUT_MAX; j++)
    {
      memcpy(ai32TryArr, ai32OrigArr, sizeof(ai32TryArr));
  
      ai32TryArr[1] = i;
      ai32TryArr[2] = j;
      u16Pos = 0;

      while(ai32TryArr[u16Pos] != OPCODE_HALT)
      {
	ProcessArray(ai32TryArr, &u16Pos);
      }
      if (ai32TryArr[0] == TARGET_NUM)
      {
	printf(">%i\n", 100 * i + j);

	return 0;
      }
    }
  }

  return 0;
}
