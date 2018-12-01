#include <stdio.h>
#include <stdlib.h>


int main()
{
  char s_line_buf[100] = { 0 };
  int i_sum = 0;
  FILE* fp = fopen("input_aoc2018_01_1.txt", "r");
  

  if (fp == NULL)
  {
    return 1;
  }
  while(fgets(s_line_buf, sizeof(s_line_buf) , fp) != NULL)
  {
    i_sum += atoi(s_line_buf);
  }
  printf(">%d\n", i_sum);

  return 0;
}
