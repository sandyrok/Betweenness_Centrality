#include "header.h"

void dealloc_queue(node **head, node** tail) {
  node *tmp;
  while (*head != NULL) {
    tmp = (*head)->next;
    free(*head);
    *head = tmp;
  }
  *tail = NULL;
}

void dealloc_list(node **head) {
  node *tmp;
  while (*head != NULL) {
    tmp = (*head)->next;
    free(*head);
    *head = tmp;
  }
}

long** alloc_2DArray(long n)
{
   long** arr,i;
   arr = (long**)malloc(n*sizeof(long*));
   for(i = 0 ; i < n;i++)
     arr[i] =  (long*)malloc(n*sizeof(long)); 
   return arr;  
}

void dealloc_2DArray( long** arr, long n)
{
   for(int i = 0 ; i < n;i++)
     free(arr[i]);
   free(arr);  
}

node *create_node(long value, node *next) {
  node *tmp = (node *)malloc(sizeof(node));
  tmp->value = value;
  tmp->next = next;
  return tmp;
}

void add_node(node **head, long value) {
  node *tmp = create_node(value, *head);
  *head = tmp;
}

long dequeue(node **head, node **tail) {
  long tmp = (*head)->value;
  node *t = *head;
  *head = (*head)->next;
  free(t);
  if (*head == NULL)
    *tail = NULL;
  return tmp;
}

void enqueue(node **tail, node **head, long value) {
  if (*tail == NULL) {
    (*head) = (*tail) = create_node(value, NULL);
    return;
  }
  (*tail)->next = create_node(value, NULL);
  (*tail) = (*tail)->next;
  return;
}

bool empty(node *head) { return head == NULL; }

node **create_graph_file(const char *file_name, long n, long e, long choice) {
  FILE *file = fopen(file_name, "r");
 
  node **graph;
  long i, j, k, w;
  graph = malloc((n + 1) * sizeof(node *));
  if (graph == NULL)
    printf("Error");
  
  for (i = 0; i <= n; i++)
     graph[i] = NULL;
  
  while (e--) {
    if(choice==1){
    fscanf(file, "%ld%ld%ld", &i, &j, &w);
    add_node(&graph[i], j);
    add_node(&graph[j], i);
    }
    else{
     fscanf(file, "%ld%ld", &i, &j);
    add_node(&graph[i+1], j+1);
    add_node(&graph[j+1], i+1);
    }
  }
  fclose(file);
  return graph;
}

void bfs_aug(long n, node **graph, long **num_spaths, long** len_spaths) {
 
  long i, j, k, v;
  node *q_head = NULL, *q_tail = NULL;
  node* tmp;

  for (v = 1; v <= n; v++) {
    for (i = 1; i <= n; i++) {
      len_spaths[v - 1][i - 1] = LONG_MAX;
      num_spaths[v - 1][i - 1] = 0;
    }
   
    num_spaths[v - 1][v - 1] = 1;
    len_spaths[v - 1][v - 1] = 0;
    enqueue(&q_tail, &q_head, v);

    for (long it = 1; !empty(q_head) && it <= n; it++) {
      i = dequeue(&q_head, &q_tail);
      for ( tmp = graph[i]; tmp != NULL; tmp = tmp->next) {
        if (len_spaths[v-1][tmp->value-1] == LONG_MAX) {
          len_spaths[v-1][tmp->value-1] = len_spaths[v-1][i-1] + 1;
          num_spaths[v-1][tmp->value-1] += num_spaths[v-1][i-1];
          enqueue(&q_tail, &q_head, tmp->value);
        } else if (len_spaths[v-1][tmp->value-1] ==  len_spaths[v-1][i-1] + 1 ) {
           num_spaths[v-1][tmp->value-1] += num_spaths[v-1][i-1];
        }
      }
    }
    dealloc_queue(&q_tail,&q_head);
    num_spaths[v - 1][v - 1] = 0;
  }

}
long bfs(node** graph, long v,long n, long* cc_id)
{
    long i, j, cc_size;
    node *q_head = NULL, *q_tail = NULL;
    node* tmp;
    enqueue(&q_tail, &q_head, v);
    cc_id[v] = v;
     for (cc_size = 0; !empty(q_head) ; cc_size++) {
      i = dequeue(&q_head, &q_tail);
     
      for ( tmp = graph[i+1]; tmp != NULL; tmp = tmp->next) {
          if(cc_id[tmp->value-1] == -1){
          cc_id[tmp->value-1] = v; 
          enqueue(&q_tail, &q_head, tmp->value-1);
        }
        
      }
    }
    dealloc_queue(&q_tail,&q_head);
    return cc_size;
}


node** extract_giant_cc(node** graph, long* v)
{
   long i,j,k,t,x,y,z,n = *v;
   long* cc_id;
   node** giant_cc;
   long* map;
   cc_id = (long*)malloc(n*sizeof(long));
   map = (long*)malloc((n)*sizeof(long));
   for(i=0;i<n;i++)
     map[i] = cc_id[i] = -1;
   for(i=j=0,k=-1;i<n;i++)
     { 
        if(cc_id[i]==-1)
         {
            t = bfs(graph,i,n,cc_id);
            if( t > j)
            {
              j = t;
              k = i;
            }
         }
     }
    for(i=1,x=1;i<=n;i++)
     {   if(cc_id[i-1]!=k)
          dealloc_list(&graph[i]);
         else
          map[i-1] = x++;
      } 
   giant_cc = malloc((j + 1) * sizeof(node *));
   if(giant_cc == NULL)
     printf("Error\n");
   for (i = 0; i <= j; i++)
      giant_cc[i] = NULL;
   for(i=1,t=0;i<=n;i++)
     {
      if(graph[i]!=NULL)
        giant_cc[map[i-1]] = graph[i];
      for (node*  tmp = graph[i]; tmp != NULL; tmp = tmp->next)
      {  
           t++;
           tmp->value = map[tmp->value-1];
       }
     }
    
     free(graph);
     *v = j;
     printf("Giant CC vertices: %ld    Edges : %ld\n",j,t/2);
     return giant_cc;
     
 
}


void floyd_warshall_aug(long n, node **graph, long** num_spaths,
                        long** len_spaths) {
                   
     
      long v,i,j,k;
      for(i=1;i<=n;i++)
        for(j=1;j<=n;j++){
          num_spaths[i-1][j-1] = 0;
          len_spaths[i-1][j-1] = LONG_MAX;
         }
      for(i=1;i<=n;i++)
       {
         len_spaths[i-1][i-1] = 0;
        } 
        
        
        
        
      for(i=1; i<=n;i++)
      {
         for( node* tmp = graph[i]; tmp!= NULL ; tmp = tmp->next )
         {
              num_spaths[i-1][tmp->value-1] = 1;
              len_spaths[i-1][tmp->value-1] = 1;
         }     
      }
 
    
      for(v=1;v<=n;v++)
      {
         for(i=1;i<=n;i++)
         {
            for(j=1;j<=n;j++)
             {
                if( len_spaths[i-1][v-1] != LONG_MAX &&  len_spaths[v-1][j-1] != LONG_MAX ){
                
                  if(len_spaths[i-1][j-1] > len_spaths[i-1][v-1] + len_spaths[v-1][j-1] )
                   {
                     len_spaths[i-1][j-1] = len_spaths[i-1][v-1] + len_spaths[v-1][j-1];
                     num_spaths[i-1][j-1] = num_spaths[i-1][v-1] * num_spaths[v-1][j-1];
                   }
                  else if(len_spaths[i-1][j-1] == len_spaths[i-1][v-1] + len_spaths[v-1][j-1])
                    num_spaths[i-1][j-1] += num_spaths[i-1][v-1] * num_spaths[v-1][j-1];
                 }    
              }
          }
     }
     
     
   /*   for (v = 1; v <= n; v++) 
    for (i = 1; i <= n; i++) {
      if(len_spaths_std[v - 1][i - 1] != len_spaths[v - 1][i - 1] || num_spaths_std[v - 1][i - 1] != num_spaths[v - 1][i - 1])
          printf("Error in Shortest Paths %ld %ld\n",v,i);
    }
     */
}



double* compute_btw_cnlty(node **graph, long n,  long floydwarshall_or_bfs) {
 
 long** num_spaths, **len_spaths;
 double* btw_cnlty;
 num_spaths = alloc_2DArray(n);
 len_spaths = alloc_2DArray(n);
 long i,v,j,k;
btw_cnlty = (double *)malloc((n) * sizeof(double));
 
 if(floydwarshall_or_bfs == 1)
  bfs_aug(n, graph, num_spaths,len_spaths);
 else if(floydwarshall_or_bfs == 2)
  floyd_warshall_aug(n, graph, num_spaths,len_spaths); 
 else
  printf("Invalid option while choosing the algorithm\n");


 for(i=0;i<n;i++)
    btw_cnlty[i] = 0;
  
  for( v= 1; v <= n; v++)
  {
     for( i = 1; i<= n; i++)
     {
        if(i==v)
          continue;
        for( j = i+1; j<=n ;j++)
         {
            if(j==v)
             continue;
            if( len_spaths[i-1][j-1] == len_spaths[i-1][v-1] + len_spaths[v-1][j-1] )
               btw_cnlty[v-1] += ((double)(num_spaths[i-1][v-1]*num_spaths[v-1][j-1]))/(num_spaths[i-1][j-1]);
         }
     }
     btw_cnlty[v-1] =  (btw_cnlty[v-1]*2)/((n-1)*(n-2));
 }
  dealloc_2DArray(num_spaths, n);
  dealloc_2DArray(len_spaths, n);
  return btw_cnlty;

}

node** create_graph_console(long n, long e) {
  node **graph;
  graph = (node **)malloc((n + 1) * sizeof(node *));
  long i, j, k;
  for (i = 0; i <= n; i++)
    graph[i] = NULL;

  while (e--) {
    scanf("%ld%ld", &i, &j);
    add_node(&graph[i], j);
    add_node(&graph[j], i);
  }
  return graph;
}

