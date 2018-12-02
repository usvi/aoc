#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// ivjhcadokesltwgsfsmqwrbnuy
#define ID_LEN (26)
#define DL_INCREMENT (100)

typedef struct
{
  int i_storage_size;
  int i_elements_count;

  char* as_ids;

} dumb_list;

static int calc_id_diffs(char* s_param_id1, char* s_param_id2);
static void dl_init(dumb_list* px_param_list);
static void dl_free(dumb_list* px_param_list);
static int dl_add(dumb_list* px_param_list, char* s_param_id, char* s_result_buf);


static int calc_id_diffs(char* s_param_id1, char* s_param_id2)
{
  int i_diffs = 0;
  int i_pos = 0;

  for (i_pos = 0; i_pos < ID_LEN; i_pos++)
  {
    if (s_param_id1[i_pos] != s_param_id2[i_pos])
    {
      i_diffs++;
    }
  }
  
  return i_diffs;
}


static void dl_init(dumb_list* px_param_list)
{
  px_param_list->i_storage_size = 0;
  px_param_list->i_elements_count = 0;
  px_param_list->as_ids = NULL;
}

static void dl_free(dumb_list* px_param_list)
{
  px_param_list->i_storage_size = 0;
  px_param_list->i_elements_count = 0;
  free(px_param_list->as_ids);
}

static int dl_add(dumb_list* px_param_list, char* s_param_id, char* s_result_buf)
{
  void* p_malloc_test = NULL;
  int i_pos = 0;
  int i_id_size = ID_LEN + 1;

  // Check if have storage space
  if (px_param_list->i_storage_size == px_param_list->i_elements_count)
  {
    //#define ID_LEN (26)
    //#define DL_INCREMENT (100)

    // Reallocate (original + increment) * size of one id
    p_malloc_test = realloc(px_param_list->as_ids,
			    (px_param_list->i_storage_size + DL_INCREMENT) * i_id_size);
    
    if (p_malloc_test == NULL)
    {
      printf("Malloc failed\n");

      exit(1);
    }
    px_param_list->as_ids = (char*)p_malloc_test;
    px_param_list->i_storage_size += DL_INCREMENT;
  }
  // Loop all elements in the buffer

  for (i_pos = 0; i_pos < px_param_list->i_elements_count; i_pos++)
  {
    //printf("%s ", px_param_list->as_ids + (i_id_size * i_pos));
    if (calc_id_diffs((px_param_list->as_ids + (i_id_size * i_pos)),
		      s_param_id) == 1)
    {
      memcpy(s_result_buf, (px_param_list->as_ids + (i_id_size * i_pos)), i_id_size);

      return 1;
    }
  }
  //printf("\n");
  memcpy(px_param_list->as_ids + (i_id_size * i_pos),
	 s_param_id, i_id_size);
  px_param_list->i_elements_count++;

  return 0;
}


int main(void)
{
  char s_line_buf[100] = { 0 };
  char s_pair_buf[100] = { 0 };
  char s_res_buf[100] = { 0 };
  dumb_list x_id_list;
  int i_pos = 0;
  int i_res_pos = 0;
  FILE* px_file = fopen("input_aoc2018_02_1.txt", "r");

  dl_init(&x_id_list);

  if (px_file == NULL)
  {
    return 1;
  }
  while(fgets(s_line_buf, sizeof(s_line_buf) , px_file) != NULL)
  {
    // Erase end of line
    *(s_line_buf + ID_LEN) = 0;

    if (dl_add(&x_id_list, s_line_buf, s_pair_buf))
    {
      break;
    }
  }
  fclose(px_file);
  dl_free(&x_id_list);

  for (i_pos = 0; i_pos < ID_LEN; i_pos++)
  {
    if (s_line_buf[i_pos] == s_pair_buf[i_pos])
    {
      s_res_buf[i_res_pos] = s_line_buf[i_pos];
      i_res_pos++;
    }
  }
  printf("Common characters: %s\n", s_res_buf);

  return 0;
}
