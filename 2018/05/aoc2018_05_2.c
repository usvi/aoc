#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>



#define POL_STORAGE_INCREMENT (1000)
#define POL_EMPTY_UNIT '.'

typedef struct
{
  int i_storage;
  int i_elements;

  char* s_units;

} polymer_type;



static void pol_init(polymer_type* px_param_polymer);
static void pol_free(polymer_type* px_param_polymer);
static void pol_add_units(polymer_type* px_param_polymer, char* s_param_units);
static void pol_react(polymer_type* px_param_polymer);
static int i_pol_reduce(polymer_type* px_param_polymer);
static void pol_copy(polymer_type* px_param_polymer, polymer_type* px_result_polymer);
static void pol_remove_unit(polymer_type* px_param_polymer, char c_param_remove_unit);



static void pol_init(polymer_type* px_param_polymer)
{
  px_param_polymer->i_storage = 0;
  px_param_polymer->i_elements = 0;
  px_param_polymer->s_units = NULL;
}


static void pol_free(polymer_type* px_param_polymer)
{
  px_param_polymer->i_storage = 0;
  px_param_polymer->i_elements = 0;
  free(px_param_polymer->s_units);
}


static void pol_add_units(polymer_type* px_param_polymer, char* s_param_units)
{
  void* p_malloc_test = NULL;

  // Check size and allocate more if needed
  while (strlen(s_param_units) > (px_param_polymer->i_storage - px_param_polymer->i_elements))
  {
    p_malloc_test = realloc(px_param_polymer->s_units, (px_param_polymer->i_storage + POL_STORAGE_INCREMENT));

    if (p_malloc_test == NULL)
    {
      printf("Malloc failed\n");

      exit(1);
    }
    px_param_polymer->s_units = (char*)p_malloc_test;
    px_param_polymer->i_storage += POL_STORAGE_INCREMENT;
  }
  // Actual add

  memcpy(px_param_polymer->s_units + px_param_polymer->i_elements,
	 s_param_units, strlen(s_param_units));
  px_param_polymer->i_elements += strlen(s_param_units);

}


static void pol_react(polymer_type* px_param_polymer)
{
  int i_pos = 0;

  for (i_pos = 0; i_pos < (px_param_polymer->i_elements - 1); i_pos++)
  {
    if ((tolower(px_param_polymer->s_units[i_pos]) == tolower(px_param_polymer->s_units[i_pos + 1])) &&
	(px_param_polymer->s_units[i_pos] != px_param_polymer->s_units[i_pos + 1]))
    {
      px_param_polymer->s_units[i_pos] = POL_EMPTY_UNIT;
      px_param_polymer->s_units[i_pos + 1] = POL_EMPTY_UNIT;
    }
  }
}


static int i_pol_reduce(polymer_type* px_param_polymer)
{
  int i_pos = 0;
  int i_pos2 = 0;
  int i_reductions = 0;
  int i_consecutive_empties = 0;

  for (i_pos = 0; i_pos < px_param_polymer->i_elements; i_pos++)
  {
    // If we find empty chars, move to left and zero tail
    if (px_param_polymer->s_units[i_pos] == POL_EMPTY_UNIT)
    {
      i_consecutive_empties = 0;
      // Actually, check how many consecutive empties there are

      for (i_pos2 = i_pos; i_pos2 < px_param_polymer->i_elements; i_pos2++)
      {
	if (px_param_polymer->s_units[i_pos2] == POL_EMPTY_UNIT)
	{
	  i_consecutive_empties++;
	}
	else
	{
	  break;
	}
      }
      memmove((px_param_polymer->s_units + i_pos),
	      (px_param_polymer->s_units + i_pos + i_consecutive_empties),
	      (px_param_polymer->i_elements - i_pos - i_consecutive_empties));
      px_param_polymer->i_elements -= i_consecutive_empties;
      memset((px_param_polymer->s_units + px_param_polymer->i_elements), 0, i_consecutive_empties);
      i_reductions = 1;
    }
  }

  return i_reductions;
}


static void pol_copy(polymer_type* px_param_polymer, polymer_type* px_result_polymer)
{
  void* p_malloc_test = NULL;

  px_result_polymer->i_storage = px_param_polymer->i_storage;
  px_result_polymer->i_elements = px_param_polymer->i_elements;

  p_malloc_test = malloc(px_param_polymer->i_storage);

  if (p_malloc_test == NULL)
  {
    printf("Malloc failed\n");

    exit(1);
  }
  px_result_polymer->s_units = (char*)p_malloc_test;
  memcpy(px_result_polymer->s_units, px_param_polymer->s_units, px_result_polymer->i_storage);
}


static void pol_remove_unit(polymer_type* px_param_polymer, char c_param_remove_unit)
{
  int i_pos = 0;

  for (i_pos = 0; i_pos < (px_param_polymer->i_elements - 1); i_pos++)
  {
    if (tolower(px_param_polymer->s_units[i_pos]) == tolower(c_param_remove_unit))
    {
      px_param_polymer->s_units[i_pos] = POL_EMPTY_UNIT;
    }
  }
}


int main(void)
{
  FILE* px_file = NULL;
  char* s_eol_pos = NULL;
  char s_line_buf[1000] = { 0 };

  polymer_type x_orig_polymer;
  polymer_type x_temp_polymer;

  char c_remove_unit = 0;

  px_file = fopen("input_aoc2018_05_1.txt", "r");
  
  if (px_file == NULL)
  {
    printf("Failed to open file\n");

    return 1;
  }
  pol_init(&x_orig_polymer);

  while (fgets(s_line_buf, 1000, px_file) != NULL)
  {
    s_eol_pos = strstr(s_line_buf, "\n");

    if (s_eol_pos != NULL)
    {
      *s_eol_pos = 0;
    }
    pol_add_units(&x_orig_polymer, s_line_buf);
  }
  fclose(px_file);

  // One by one remove stuff react + reduce
  
  for (c_remove_unit = 'a'; c_remove_unit <= 'z'; c_remove_unit++)
  {
    pol_copy(&x_orig_polymer, &x_temp_polymer);

    pol_remove_unit(&x_temp_polymer, c_remove_unit);
    pol_react(&x_temp_polymer);

    while (i_pol_reduce(&x_temp_polymer))
    {
      pol_react(&x_temp_polymer);
    }
    printf("Removing %c resulted in polymer length %zu\n",
	   c_remove_unit, strlen(x_temp_polymer.s_units));
    pol_free(&x_temp_polymer);
  }

  pol_free(&x_orig_polymer);

  return 0;
}
