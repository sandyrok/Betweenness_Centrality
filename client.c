#include "header.h"
int main(int argc, char *argv[]) {
  long n, i, e;
  int c=2;
  double* btw_cnlty_bfs, *btw_cnlty_fw_algo;
  char* file_name;
  node **graph = NULL;
  node *tmp;
  time_t seconds; 
  seconds = time(NULL); 
 scanf("%d", &c);
  if (c == 1) {
    scanf("%ld%ld", &n, &e);
    graph = create_graph_console(n, e);

  } else {
     scanf("%d", &c);
    if(c==1)
      n = 327, e = 5818;
    else 
       scanf("%ld%ld", &n, &e);
   
    graph = create_graph_file(argv[1], n, e, c);
   }
   graph = extract_giant_cc(graph,&n);
   seconds = time(NULL);
   btw_cnlty_bfs = compute_btw_cnlty(graph, n,1);
   printf("%ld sec BFS\n", time(NULL)-seconds);
    seconds = time(NULL);
   btw_cnlty_fw_algo = compute_btw_cnlty(graph, n,2);
    printf("%ld sec FW Algorithm\n", time(NULL)-seconds);

   for(i=0;i<n;i++)
      printf("%f ", btw_cnlty_bfs[i]);
   printf("\n");
   for(i=0;i<n;i++)
      printf("%f ", btw_cnlty_fw_algo[i]);


  for(i=0;i<=n;i++)
    dealloc_list(&graph[i]);
  free(graph);
}
