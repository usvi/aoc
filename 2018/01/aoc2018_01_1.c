#include <stdio.h>
#include <stdlib.h>


int main()
{
  char s_line_buf[100] = { 0 };
  int i_sum = 0;
  FILE* px_file = fopen("input_aoc2018_01_1.txt", "r");
  

  if (px_file == NULL)
  {
    return 1;
  }
  while(fgets(s_line_buf, sizeof(s_line_buf) , px_file) != NULL)
  {
    i_sum += atoi(s_line_buf);
  }
  fclose(px_file);
  printf(">%d\n", i_sum);

  return 0;
}
