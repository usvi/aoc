#include <stdio.h>
#include <stdlib.h>
#include <string.h>


#define SL_STORAGE_INCREMENT (100)
#define SL_ADD_NONDUP (0)
#define SL_ADD_DUP (1)


typedef struct
{
  int i_storage_size;
  int i_elements_count;

  int* ai_values;

} sorted_list;


static void sl_init(sorted_list* px_param_sl);
static void sl_free(sorted_list* px_param_sl);
static int sl_add(sorted_list* px_param_sl, int i_param_value);


static void sl_init(sorted_list* px_param_sl)
{
  void* p_malloc_test = NULL;

  px_param_sl->i_elements_count = 0;
  px_param_sl->ai_values = NULL;

  p_malloc_test = realloc(px_param_sl->ai_values, SL_STORAGE_INCREMENT * sizeof(int));

  if (p_malloc_test == NULL)
  {
    printf("Malloc failed\n");

    exit(1);
  }
  px_param_sl->i_storage_size = SL_STORAGE_INCREMENT;
  px_param_sl->ai_values = (int*)p_malloc_test;
}


static void sl_free(sorted_list* px_param_sl)
{
  free(px_param_sl->ai_values);
  px_param_sl->ai_values = NULL;
  px_param_sl->i_storage_size = 0;
  px_param_sl->i_elements_count = 0;
}

static int sl_add(sorted_list* px_param_sl, int i_param_value)
{
  void* p_malloc_test = NULL;
  int i_part_start = 0;
  int i_part_end = px_param_sl->i_elements_count;
  int i_part_middle = (i_part_start + i_part_end) / 2;

  while (i_part_middle != i_part_start)
  {
    if (i_param_value < px_param_sl->ai_values[i_part_middle])
    {
      // Value to be added smaller than middle, advance left
      // i_part_start stays
      i_part_end = i_part_middle;
      i_part_middle = (i_part_start + i_part_end) / 2;
    }
    else if (i_param_value > px_param_sl->ai_values[i_part_middle])
    {
      // Value larger than middle, advance right
      i_part_start = i_part_middle;
      // i_part_end stays
      i_part_middle = (i_part_start + i_part_end) / 2;
    }
    else
    {
      // Exact match
      return SL_ADD_DUP;
    }
  }
  // Lets see if we should actually be on one more right
  if (i_param_value > px_param_sl->ai_values[i_part_middle])
  {
    i_part_middle++;
  }

  // Do we have storage?
  if (px_param_sl->i_elements_count == px_param_sl->i_storage_size)
  {
    p_malloc_test = realloc(px_param_sl->ai_values, ((px_param_sl->i_storage_size + SL_STORAGE_INCREMENT) * sizeof(int)));

    if (p_malloc_test == NULL)
    {
      printf("Malloc failed\n");
      
      exit(1);
    }
    px_param_sl->ai_values = (int*)p_malloc_test;
    px_param_sl->i_storage_size += SL_STORAGE_INCREMENT;
  }

  memmove(px_param_sl->ai_values + i_part_middle + 1,
	  px_param_sl->ai_values + i_part_middle,
	  (px_param_sl->i_elements_count - i_part_middle) * sizeof(int));

  px_param_sl->ai_values[i_part_middle] = i_param_value;
  px_param_sl->i_elements_count++;

  return SL_ADD_NONDUP;
}

int main(void)
{
  char s_line_buf[100] = { 0 };
  int i_sum = 0;
  FILE* px_file = fopen("input_aoc2018_01_1.txt", "r");
  sorted_list x_list;

  sl_init(&x_list);
  sl_add(&x_list, 0);

  if (px_file == NULL)
  {
    sl_free(&x_list);

    return 1;
  }
  while (1)
  {
    if (fgets(s_line_buf, sizeof(s_line_buf) , px_file) == NULL)
    {
      // Might be end of line stuff, checking
      if (feof(px_file))
      {
	rewind(px_file);
      }
      else
      {
	printf("Error reading file\n");
	fclose(px_file);
	sl_free(&x_list);

	return 1;
      }
    }
    else
    {
      i_sum += atoi(s_line_buf);

      if (sl_add(&x_list, i_sum) == SL_ADD_DUP)
      {
	printf("First same was %d\n", i_sum);

	fclose(px_file);
	sl_free(&x_list);

	return 0;
      }
    }
  }
  fclose(px_file);
  sl_free(&x_list);

  return 0;
}
