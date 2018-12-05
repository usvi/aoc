#include <stdio.h>
#include <string.h>
#include <stdlib.h>


#define SSF_ELEMENT_SIZE (50)
#define SSF_ITEM_INCREMENT (50)
#define SSF_TIME_COMP_START (1)
#define SSF_TIME_COMP_LEN (16)
#define SSF_EVENT_START (19)



typedef struct
{
  int i_storage_size;
  int i_elements;

  char* s_elements;

} sorted_sleep_file;


typedef struct
{
  int i_guard_id;
  int i_total_sleep;

  int ai_minutes[60];

} guard_sleep_log;



static void ssf_init(sorted_sleep_file* px_param_file);
static void ssf_free(sorted_sleep_file* px_param_file);
static void ssf_add(sorted_sleep_file* px_param_file, char* s_line);
static void ssf_print(sorted_sleep_file* px_param_file);
static void ssf_process_sleeps(sorted_sleep_file* px_param_file);



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


static void ssf_process_sleeps(sorted_sleep_file* px_param_file)
{
  guard_sleep_log* px_guard_sleeps = NULL;
  guard_sleep_log* px_guard_pointer = NULL;
  void* p_malloc_test = NULL;
  int i_pos = 0;
  int i_pos2 = 0;
  int i_guards = 0;
  int i_guard = 0;
  int i_minutes = 0;

  char* s_element_pos = NULL;

  int i_sleep_start = 0;
  int i_sleep_end = 0;

  int i_max_sleeps = 0;
  int i_sleepiest_minute = 0;
  guard_sleep_log* px_sleepiest_guard = NULL;

  for (i_pos = 0; i_pos < px_param_file->i_elements; i_pos++)
  {
    //[1518-11-01 00:00] Guard #10 begins
    s_element_pos = px_param_file->s_elements + ((i_pos * SSF_ELEMENT_SIZE) + SSF_EVENT_START);

    if (1 == sscanf(s_element_pos, "Guard #%d begins", &i_guard))
    {
      // Need to get the guard pointer. If does not exist, realloc
      px_guard_pointer = NULL;

      for (i_pos2 = 0; (i_pos2 < i_guards) && (px_guard_pointer == NULL); i_pos2++)
      {
	if (px_guard_sleeps[i_pos2].i_guard_id == i_guard)
	{
	  px_guard_pointer = px_guard_sleeps + i_pos2;
	}
      }
      // We should have guard pointer. But if we don't, we realloc
      if (px_guard_pointer == NULL)
      {
	// No guard, make it
	p_malloc_test = realloc(px_guard_sleeps, (i_guards + 1) * sizeof(guard_sleep_log));
	
	if (p_malloc_test == NULL)
	{
	  printf("Malloc failed\n");

	  exit(1);
	}
	px_guard_sleeps = (guard_sleep_log*)p_malloc_test;
	px_guard_pointer = px_guard_sleeps + i_guards;
	memset(px_guard_pointer, 0, sizeof(guard_sleep_log));
	px_guard_pointer->i_guard_id = i_guard;
	i_guards++;
      }
      // Now in any case we have a valid guard pointer
    }
    else if (1 == sscanf(s_element_pos - 4, "%d", &i_minutes))
    {
      // We have either "falls asleep" or "wakes up"
      if (strcmp(s_element_pos, "falls asleep") == 0)
      {
	i_sleep_start = i_minutes;
      }
      else if (strcmp(s_element_pos, "wakes up") == 0)
      {
	i_sleep_end = i_minutes;
	px_guard_pointer->i_total_sleep += (i_sleep_end - i_sleep_start);

	// Mark the minutes to "sleep map"
	for (i_pos2 = i_sleep_start; i_pos2 < i_sleep_end; i_pos2++)
	{
	  px_guard_pointer->ai_minutes[i_pos2]++;

	  if (px_guard_pointer->ai_minutes[i_pos2] > i_max_sleeps)
	  {
	    i_max_sleeps = px_guard_pointer->ai_minutes[i_pos2];
	    i_sleepiest_minute = i_pos2;
	    px_sleepiest_guard = px_guard_pointer;
	  }
	}
      }
    }
  }
  printf("Sleepiest minute %d guard was %d  resulting in answer %d\n",
	 i_sleepiest_minute, px_sleepiest_guard->i_guard_id,
	 (i_sleepiest_minute * px_sleepiest_guard->i_guard_id));

  free(px_guard_sleeps);
}


int main(void)
{
  char s_line_buf[100] = { 0 };
  char* s_eol_pos = NULL;
  sorted_sleep_file x_sleep_file;

  FILE* px_file = fopen("input_aoc2018_04_1.txt", "r");

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
    ssf_add(&x_sleep_file, s_line_buf);
  }
  fclose(px_file);
  ssf_process_sleeps(&x_sleep_file);
  ssf_free(&x_sleep_file);

  return 0;
}
