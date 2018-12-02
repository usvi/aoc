#include <stdio.h>
#include <stdlib.h>
#include <string.h>


#define NUM_CHARS (26)

static void check_line(char* s_line, int* pi_dups, int* pi_trips);



static void check_line(char* s_line, int* pi_dups, int* pi_trips)
{
  int i_size = 0;
  int i_pos = 0;
  char* s_eol_pos = NULL;

  //abcdefghijklmnopqrstuvwxyz
  int ai_freqs[NUM_CHARS] = { 0 };

  s_eol_pos = strstr(s_line, "\n");

  if (s_eol_pos != NULL)
  {
    *s_eol_pos = 0;
  }

  i_size = strlen(s_line);
  
  for (i_pos = 0; i_pos < i_size; i_pos++)
  {
    ai_freqs[s_line[i_pos] - 'a']++;
  }
  // Count trips and dups
  *pi_dups = 0;
  *pi_trips = 0;

  for (i_pos = 0; i_pos < NUM_CHARS; i_pos++)
  {
    if (ai_freqs[i_pos] == 2)
    {
      *pi_dups = 1;
    }
    else if (ai_freqs[i_pos] == 3)
    {
      *pi_trips = 1;
    }
  }
}


int main(void)
{
  char s_line_buf[100] = { 0 };
  FILE* px_file = fopen("input_aoc2018_02_1.txt", "r");
  int i_dup_sum = 0;
  int i_trip_sum = 0;
  int i_dup = 0;
  int i_trip = 0;


  if (px_file == NULL)
  {
    return 1;
  }
  while(fgets(s_line_buf, sizeof(s_line_buf) , px_file) != NULL)
  {
    check_line(s_line_buf, &i_dup, &i_trip);
    i_dup_sum += i_dup;
    i_trip_sum += i_trip;
  }
  printf("Dups: %d, trips: %d, sum: %d\n",
	 i_dup_sum, i_trip_sum, (i_dup_sum * i_trip_sum));
  fclose(px_file);


  return 0;
}
