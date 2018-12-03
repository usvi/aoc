#include <stdio.h>
#include <string.h>
#include <stdlib.h>


typedef struct
{
  int i_xmax;
  int i_ymax;

  char** aac_patches;

} patch_map;


static int analyze_patch(char* s_input_buf, int* pi_id, int* pi_xpos,
			 int* pi_ypos, int* pi_xlen, int* pi_ylen);
static void pm_init(patch_map* px_param_map, int i_param_xmax, int i_param_ymax);
static void pm_free(patch_map* px_param_map);
static int pm_add_patch(patch_map* px_param_map, char* s_input_patch);



static int analyze_patch(char* s_input_buf, int* pi_id, int* pi_xpos,
			 int* pi_ypos, int* pi_xlen, int* pi_ylen)
{
  if (5 == sscanf(s_input_buf, "#%d @ %d,%d: %dx%d",
		  pi_id, pi_xpos, pi_ypos, pi_xlen, pi_ylen))
  {
    return 1;
  }

  return 0;
}


static void pm_init(patch_map* px_param_map, int i_param_xmax, int i_param_ymax)
{
  void* p_malloc_test = NULL;
  int i_pos = 0;
  px_param_map->i_xmax = i_param_xmax;
  px_param_map->i_ymax = i_param_ymax;
  
  // Allocate number of rows first
  p_malloc_test = malloc((px_param_map->i_ymax + 1) * sizeof(char*));

  if (p_malloc_test == NULL)
  {
    printf("Malloc failed\n");
    
    exit(1);
  }
  px_param_map->aac_patches = (char**)p_malloc_test;

  // Allocate actual rows then
  for (i_pos = 0; i_pos < (px_param_map->i_ymax + 2); i_pos++)
  {
    p_malloc_test = calloc((px_param_map->i_xmax + 1), sizeof(char));

    if (p_malloc_test == NULL)
    {
      printf("Malloc failed\n");
      
      exit(1);
    }
    px_param_map->aac_patches[i_pos] = (char*)p_malloc_test;
  }
}


static int pm_add_patch(patch_map* px_param_map, char* s_input_patch)
{
  int i_xpos = 0;
  int i_ypos = 0;

  int i_patch_id = 0;
  int i_xpatch_pos = 0;
  int i_ypatch_pos = 0;
  int i_xpatch_len = 0;
  int i_ypatch_len = 0;

  int i_patch_dups = 0;

  // We have analyzed this once already, it will be ok
  analyze_patch(s_input_patch, &i_patch_id, &i_xpatch_pos,
		&i_ypatch_pos, &i_xpatch_len, &i_ypatch_len);

  // Now, map patch to map
  for (i_ypos = i_ypatch_pos; i_ypos < (i_ypatch_pos + i_ypatch_len); i_ypos++)
  {
    for (i_xpos = i_xpatch_pos; i_xpos < (i_xpatch_pos + i_xpatch_len); i_xpos++)
    {
      px_param_map->aac_patches[i_ypos][i_xpos]++;

      if (px_param_map->aac_patches[i_ypos][i_xpos] == 2)
      {
	// New dup
	i_patch_dups++;
      }
      else if (px_param_map->aac_patches[i_ypos][i_xpos] > 1)
      {
	// Suppressing overloads
	px_param_map->aac_patches[i_ypos][i_xpos] = 2;
      }
    }

  }
  
  return i_patch_dups;
}


static void pm_free(patch_map* px_param_map)
{
  int i_pos = 0;

  for (i_pos = 0; i_pos < (px_param_map->i_ymax + 2); i_pos++)
  {
    free(px_param_map->aac_patches[i_pos]);
  }
  free(px_param_map->aac_patches);
}


int main(void)
{
  int i_dup_sum = 0;
  char s_line_buf[100] = { 0 };
  char* s_eol_pos = NULL;
  patch_map x_patch_map;
  FILE* px_file = NULL;

  int i_id = 0;
  int i_xpos = 0;
  int i_ypos = 0;
  int i_xlen = 0;
  int i_ylen = 0;

  int i_xmax = 0;
  int i_ymax = 0;

  px_file = fopen("input_aoc2018_03_1.txt", "r");

  if (px_file == NULL)
  {
    printf("Failed to open file\n");

    return 1;
  }
  while (fgets(s_line_buf, sizeof(s_line_buf) , px_file) != NULL)
  {
    s_eol_pos = strstr(s_line_buf, "\n");

    if (s_eol_pos != NULL)
    {
      *s_eol_pos = 0;
    }
    if (analyze_patch(s_line_buf, &i_id, &i_xpos, &i_ypos, &i_xlen, &i_ylen))
    {
      i_xmax = (i_xmax > (i_xpos + i_xlen) ? i_xmax : (i_xpos + i_xlen));
      i_ymax = (i_ymax > (i_ypos + i_ylen) ? i_ymax : (i_ypos + i_ylen));
    }
    else
    {
      printf("Error on line %s", s_line_buf);
      fclose(px_file);

      exit(1);
    }
  }
  // Set up the map as we know max dimensions
  pm_init(&x_patch_map, i_xmax, i_ymax);
  // Rewind file and do our magic again.
  rewind(px_file);

  while (fgets(s_line_buf, sizeof(s_line_buf) , px_file) != NULL)
  {
    s_eol_pos = strstr(s_line_buf, "\n");

    if (s_eol_pos != NULL)
    {
      *s_eol_pos = 0;
    }
    i_dup_sum += pm_add_patch(&x_patch_map, s_line_buf);
  }
  fclose(px_file);
  pm_free(&x_patch_map);

  printf("Duplicate areas: %d\n", i_dup_sum);

  return 0;
}
