#include <stdlib.h>
#include <stdio.h>
#include <string.h>


#define SN_CHILDREN_MAX ('Z' - 'A' + 1)
#define SN_NOT_FOUND_ID (122)
#define SN_STATE_UNKNOWN (0)
#define SN_STATE_SEEN (1)
#define SN_STATE_FINISHED (2)



typedef struct
{
  int i_node_state;
  int i_reqs_left;
  char s_children[SN_CHILDREN_MAX];
  
} step_node;



static void nl_add_step(step_node* ax_param_nodes, char c_param_id, char c_param_req);
static char c_nl_find_next_visitable(step_node* ax_param_nodes);
static void nl_visit_step(step_node* ax_param_nodes, char c_visit_step);



static void nl_add_step(step_node* ax_param_nodes, char c_param_node, char c_param_child)
{
  ax_param_nodes[c_param_node - 'A'].s_children[c_param_child - 'A'] = 1;
  ax_param_nodes[c_param_node - 'A'].i_node_state = SN_STATE_SEEN;
  ax_param_nodes[c_param_child - 'A'].i_node_state = SN_STATE_SEEN;
  ax_param_nodes[c_param_child - 'A'].i_reqs_left++;
}


static char c_nl_find_next_visitable(step_node* ax_param_nodes)
{
  // Just go through the list alphaberically and find
  // node with zero reqs and state seen

  int i_pos = 0;

  for (i_pos = 0; i_pos < SN_CHILDREN_MAX; i_pos++)
  {
    if ((ax_param_nodes[i_pos].i_node_state == SN_STATE_SEEN) &&
	(ax_param_nodes[i_pos].i_reqs_left == 0))
    {
      return (i_pos + 'A');
    }
  }
  return 0;
}


static void nl_visit_step(step_node* ax_param_nodes, char c_visit_step)
{
  int i_pos = 0;
  // Decrement counter of every childs reqs

  for (i_pos = 0; i_pos < SN_CHILDREN_MAX; i_pos++)
  {
    if (ax_param_nodes[c_visit_step - 'A'].s_children[i_pos])
    {
      ax_param_nodes[i_pos].i_reqs_left--;
    }
  }
  ax_param_nodes[c_visit_step - 'A'].i_node_state = SN_STATE_FINISHED;
}


int main(void)
{
  FILE* px_file = NULL;

  step_node ax_nodes['Z' - 'A' + 1];

  char s_line_buf[100] = { 0 };
  char s_out_buf[100] = { 0 };
  char c_step_id = 0;
  char c_step_child = 0;
  char c_next_step = 0;
  int i_pos = 0;

  memset(ax_nodes, 0, sizeof(ax_nodes));

  px_file = fopen("input_aoc2018_07_1.txt", "r");
  //px_file = fopen("example_input_aoc2018_07_1.txt", "r");

  if (px_file == NULL)
  {
    printf("Unable to open file\n");

    return 1;
  }
  while (fgets(s_line_buf, 100, px_file) != NULL)
  {
    if (sscanf(s_line_buf, "Step %c must be finished before step %c can begin",
	      &c_step_id, &c_step_child) != 2)
    {
      printf("Error parsing line");
      fclose(px_file);

      return 1;
    }
    nl_add_step(ax_nodes, c_step_id, c_step_child);
  }

  while ((c_next_step = c_nl_find_next_visitable(ax_nodes)))
  {
    s_out_buf[i_pos++] = c_next_step;
    nl_visit_step(ax_nodes, c_next_step);
  }
  fclose(px_file);
  printf("%s\n", s_out_buf);

  return 0;
}
