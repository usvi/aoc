#include <stdlib.h>
#include <stdio.h>
#include <string.h>


typedef struct tree_node tree_node;

struct tree_node
{
  int i_num_children;
  int i_num_meta;

  tree_node** apx_children;
  int* ai_meta;
};



static tree_node* px_tn_get_nodes(FILE* px_param_file);
static int i_tn_count_value(tree_node* px_param_node);
static void tn_free_node(tree_node* px_param_node);



static tree_node* px_tn_get_nodes(FILE* px_param_file)
{
  int i_num_children;
  int i_num_meta;
  void* p_malloc_test = NULL;
  tree_node* px_return_node = NULL;
  int i_pos = 0;

  if (fscanf(px_param_file, "%d %d", &i_num_children, &i_num_meta) != 2)
  {
    printf("Failed to read input\n");
    fclose(px_param_file);

    exit(1);
  }
  // Allocate memory for the new node first
  p_malloc_test = malloc(sizeof(tree_node));

  if (p_malloc_test == NULL)
  {
    printf("Malloc failed\n");
    fclose(px_param_file);
    
    exit(1);
  }
  px_return_node = (tree_node*)p_malloc_test;
  // Set defaults
  px_return_node->apx_children = NULL;
  px_return_node->ai_meta = NULL;
  // Set amount of children and meta
  px_return_node->i_num_children = i_num_children;
  px_return_node->i_num_meta = i_num_meta;
  // Allocate memory to children pointers and meta table
  if (px_return_node->i_num_children > 0)
  {
    p_malloc_test = malloc(px_return_node->i_num_children * sizeof(tree_node*));

    if (p_malloc_test == NULL)
    {
      printf("Malloc failed\n");
      fclose(px_param_file);
    
      exit(1);
    }
    px_return_node->apx_children = (tree_node**)p_malloc_test;
  }
  if (px_return_node->i_num_meta > 0)
  {
    p_malloc_test = malloc(px_return_node->i_num_meta * sizeof(int));

    if (p_malloc_test == NULL)
    {
      printf("Malloc failed\n");
      fclose(px_param_file);
    
      exit(1);
    }
    px_return_node->ai_meta = (int*)p_malloc_test;
  }
  // Subcall i_num_children times, place pointers to our table
  for (i_pos = 0; i_pos < px_return_node->i_num_children; i_pos++)
  {
    px_return_node->apx_children[i_pos] = px_tn_get_nodes(px_param_file);
  }
  // Read i_num_meta characters to meta table
  for (i_pos = 0; i_pos < px_return_node->i_num_meta; i_pos++)
  {
    if (fscanf(px_param_file, "%d", &(px_return_node->ai_meta[i_pos])) != 1)
    {
      printf("Failed to read input\n");
      fclose(px_param_file);

      exit(1);
    }
  }
  
  return px_return_node;
}


static int i_tn_count_value(tree_node* px_param_node)
{
  int i_retsum = 0;
  int i_pos = 0;

  // "If a node has no child nodes, its value is
  // the sum of its metadata entries."

  if (px_param_node->i_num_children == 0)
  {
    for (i_pos = 0; i_pos < px_param_node->i_num_meta; i_pos++)
    {
      i_retsum += px_param_node->ai_meta[i_pos];
    }
    
    return i_retsum;
  }
  // "However, if a node does have child nodes,
  // the metadata entries become indexes which
  // refer to those child nodes."

  for (i_pos = 0; i_pos < px_param_node->i_num_meta; i_pos++)
  {
    // "A metadata entry of 1 refers to the first
    // child node, 2 to the second, 3 to the third, and so on."

    if (((px_param_node->ai_meta[i_pos] - 1) >= 0) && 
	((px_param_node->ai_meta[i_pos] - 1) < px_param_node->i_num_children))
    {
      i_retsum += i_tn_count_value(px_param_node->apx_children[px_param_node->ai_meta[i_pos] - 1]);
    }
  }

  return i_retsum;
}


static void tn_free_node(tree_node* px_param_node)
{
  int i_pos = 0;

  // First free children

  for (i_pos = 0; i_pos < px_param_node->i_num_children; i_pos++)
  {
    tn_free_node(px_param_node->apx_children[i_pos]);
  }
  free(px_param_node->apx_children);
  free(px_param_node->ai_meta);
  free(px_param_node);
}


int main(void)
{
  FILE* px_file = NULL;
  tree_node* px_tree = NULL;

  //px_file = fopen("example_input_aoc2018_08_1.txt", "r");
  px_file = fopen("input_aoc2018_08_1.txt", "r");
  
  if (px_file == NULL)
  {
    printf("Unable to open file \n");

    return 0;
  }
  px_tree = px_tn_get_nodes(px_file);
  printf("Final value was %d\n", i_tn_count_value(px_tree));
  tn_free_node(px_tree);
  fclose(px_file);
  
  return 0;
}
