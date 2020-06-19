#include <limits.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include<time.h>

typedef struct node {
  unsigned long value;
  struct node *next;
} node;

void dealloc_queue(node **head, node** tail);
void dealloc_list(node **head);
void dealloc_2DArray( long** arr, long n);
long** alloc_2DArray(long n);
node *create_node(long value, node *next);
void add_node(node **head, long value);
long dequeue(node **head, node **tail);
void enqueue(node **tail, node **head, long value);
bool empty(node *head);
node **create_graph_file(const char *file_name, long n, long e, long choice);
void bfs_aug(long n, node **graph, long **num_spaths, long** len_spaths);
long bfs(node** graph, long v,long n, long* cc_id);
node** extract_giant_cc(node** graph, long* v);
void floyd_warshall_aug(long n, node **graph, long** num_spaths, long** len_spaths);
double* compute_btw_cnlty(node **graph, long n,  long floydwarshall_or_bfs);
node** create_graph_console(long n, long e);

