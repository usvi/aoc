#include <stdio.h>
#include <string.h>
#include <stdlib.h>


#define SSF_ELEMENT_SIZE (50)
#define SSF_ITEM_INCREMENT (50)
#define SSF_TIME_COMP_START (1)
#define SSF_TIME_COMP_LEN (16)


typedef struct
{
  int i_storage_size;
  int i_elements;

  char* s_elements;

} sorted_sleep_file;


static void ssf_init(sorted_sleep_file* px_param_file);
static void ssf_free(sorted_sleep_file* px_param_file);
static void ssf_add(sorted_sleep_file* px_param_file, char* s_line);
static void ssf_print(sorted_sleep_file* px_param_file);



static void ssf_init(sorted_sleep_file* px_param_file)
{
  px_param_file->i_storage_size = 0;
  px_param_file->i_elements = 0;
  px_param_file->s_elements = NULL;
}


static void ssf_free(sorted_sleep_file* px_param_file)
{
  px_param_file->i_storage_size = 0;
  px_param_file->i_elements = 0;
  free(px_param_file->s_elements);
}

static void ssf_add(sorted_sleep_file* px_param_file, char* s_line)
{
  int i_pos = 0;
  void* p_malloc_test = NULL;

  if (px_param_file->i_storage_size == px_param_file->i_elements)
  {
    // Need to add more storage
    px_param_file->i_storage_size += SSF_ITEM_INCREMENT;
    p_malloc_test = realloc(px_param_file->s_elements, 
			    (px_param_file->i_storage_size * SSF_ELEMENT_SIZE * sizeof(char)));

    if (p_malloc_test == NULL)
    {
      printf("Malloc failed\n");

      exit(1);
    }
    px_param_file->s_elements = (char*)p_malloc_test;
  }
  // Run our elements until we find a position
  for (i_pos = 0; i_pos < px_param_file->i_elements; i_pos++)
  {
    if (strncmp(s_line + SSF_TIME_COMP_START,
		px_param_file->s_elements + (i_pos * SSF_ELEMENT_SIZE + SSF_TIME_COMP_START),
		SSF_TIME_COMP_LEN) < 0)
    {
      break;
    }
  }
  // Make space for the new entry, then add it
  memmove(px_param_file->s_elements + ((i_pos + 1) * SSF_ELEMENT_SIZE),
	  px_param_file->s_elements + (i_pos * SSF_ELEMENT_SIZE),
	  (px_param_file->i_elements - i_pos) * SSF_ELEMENT_SIZE * sizeof(char));
  memset(px_param_file->s_elements + (i_pos * SSF_ELEMENT_SIZE),
	 0, SSF_ELEMENT_SIZE);
  strcpy(px_param_file->s_elements + (i_pos * SSF_ELEMENT_SIZE), s_line);
  px_param_file->i_elements++;
}



static void ssf_print(sorted_sleep_file* px_param_file)
{
  int i_pos = 0;

  for (i_pos = 0; i_pos < px_param_file->i_elements; i_pos++)
  {
    printf("%s\n", px_param_file->s_elements + (i_pos * SSF_ELEMENT_SIZE));
  }
}

int main(void)
{
  char s_line_buf[100] = { 0 };
  char* s_eol_pos = NULL;
  sorted_sleep_file x_sleep_file;

  //FILE* px_file = fopen("input_aoc2018_04_1.txt", "r");
  FILE* px_file = fopen("input.txt", "r");


  if (px_file == NULL)
  {
    printf("Failed to open file\n");

    return 1;
  }
  ssf_init(&x_sleep_file);

  while (fgets(s_line_buf, sizeof(s_line_buf) , px_file) != NULL)
  {
    s_eol_pos = strstr(s_line_buf, "\n");

    if (s_eol_pos != NULL)
    {
      *s_eol_pos = 0;
    }
    //printf("Read %s\n", s_line_buf);
    ssf_add(&x_sleep_file, s_line_buf);
  }
  fclose(px_file);
  ssf_print(&x_sleep_file);
  ssf_free(&x_sleep_file);


  return 0;
}
