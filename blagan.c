#include <stdio.h>
#include <stdlib.h>
#include <string.h>


typedef struct node_t {
    char label;
    int n_neighbors;
    struct node_t **neighbors; /* list of pointers of adjacent nodes */
    double *weights;
} Node;

typedef struct graph_t {
    int n_nodes;
    struct node_t **nodes; /* list of pointers of nodes */
} Graph;

typedef struct path_t {
    char *path; /* represent a path in string */
    double cost;
} Path;

///////////////////////////////////////////////////////////////////////////////

//Allocate memory for a graph and set default values
Graph *init_graph() {
    Graph *graph = (Graph *)malloc(sizeof(Graph));
    graph->n_nodes = 0;
    graph->nodes = (Node **)malloc(graph->n_nodes*sizeof(Node *));
    return graph;
}


//Free the memory area that a graph used
void free_graph(struct graph_t *graph) {
    int i;
    for(i=0; i<graph->n_nodes; i++) {
        free(graph->nodes[i]);
    }

    free(graph->nodes);
    free(graph);
}


////////////////////////////////////////////////////////////////////////////////



//Allocate memory for a node
Node *create_node(Graph *graph, char label) {
    for(int i=0; i<graph->n_nodes;i++){
        if(label==graph->nodes[i]->label){
            return graph->nodes[i];
        }}
    Node *node;
    node = (Node *) malloc(sizeof(Node));
    node->label = label;
    node->n_neighbors = 0;
    node->neighbors = (Node **) malloc(node->n_neighbors * sizeof(Node *));
    node->weights = (double *) malloc(node->n_neighbors * sizeof(double));

    return node;
}






///////////////////////////////////////////////////////////////////////////////


//Add a node to a graph
void add_node(Graph *graph, Node *node) {
    for(int i=0; i<graph->n_nodes;i++){
        if(node->label==graph->nodes[i]->label){
            return;
        }
    }
    graph->nodes = (Node **)realloc(graph->nodes,(graph->n_nodes+1)*sizeof(Node *));
    graph->nodes[graph->n_nodes] = node;
    graph->n_nodes += 1;
}


//Connect a to b
void add_edge(Node *a, Node *b, double weight) {
    a->neighbors = (Node **)realloc(a->neighbors,(a->n_neighbors+1)*sizeof(Node *));
    a->weights = (double *)realloc(a->weights,(a->n_neighbors+1)*sizeof(double));
    a->neighbors[a->n_neighbors] = b;
    a->weights[a->n_neighbors] = weight;
    a->n_neighbors += 1;
}

///////////////////////////////////////////////////////////////////////////
void delete_edge(Graph *graph, Node *a, Node *b) {
    int save=0;
    int j=0;
    for (int i = 0; i <a->n_neighbors; i++) {
        if(a->neighbors[i]->label!=b->label){
            a->neighbors[j]=b;
        }else{
            save=i;
        }
    }
    a->neighbors = (Node **)realloc(a->neighbors,(a->n_neighbors-1)*sizeof(Node *));
    a->n_neighbors -= 1;
    for (int i = save+1; i <a->n_neighbors; i++) {
        a->weights[save]=a->weights[i];
        save++;
    }
    a->weights = (double *)realloc(a->weights,(a->n_neighbors-1)*sizeof(double));

}



void delete_node(Graph *graph, Node *node) {
    int j=0;
    for (int i = 0; i <graph->n_nodes; i++) {
        if (graph->nodes[i]->label != node->label) {
            graph->nodes[j] = graph->nodes[i];
            j++;
        }  }
        graph->nodes = (Node **) realloc(graph->nodes, (graph->n_nodes - 1) * sizeof(Node *));
        graph->n_nodes -= 1;
        for (int i = 0; i <graph->n_nodes; i++){
            for (int j = 0; j <graph->nodes[i]->n_neighbors; j++){
                if(graph->nodes[i]->neighbors[j]->label==node->label){
                    Node *s=graph->nodes[i];
                    Node *r=graph->nodes[i]->neighbors[j];
                    delete_edge(graph,s,r);
                }
            }}
    }



///////////////////////////////////////////////////////////////////////////////////


////////////////////////////////////////////////////////////


int get_node_index(Graph *graph, Node *node) {
    int i;
    Node *n;
    for(i = 0; i < graph->n_nodes; i++) {
        n = graph->nodes[i];
        if(n->label == node->label) {
            return i;
        }
    }
    printf("label : %c\n", node->label);
    printf("Invalid label detected.\n");
    return -1;
}

int are_same_nodes(Node *node1, Node *node2) {
    return node1 == node2 && node1->label == node2->label;
}

Path *dijkstra(Graph *graph, Node *start, Node *end) {
    int i;
    Node *neighbor;
    Node *node;
    Node *current;
    double distances[graph->n_nodes];
    double alternative;
    double shortest;
    int previous[graph->n_nodes];
    int visited[graph->n_nodes];
    int path_indices[graph->n_nodes];
    int n_visited, index, shortest_index, path_length, u;

    for(i = 0; i < graph->n_nodes; i++) {
        node = graph->nodes[i];

        if(node->label == start->label) {
            distances[i] = 0;
        } else {
            distances[i] = 10000;
        }

        previous[i] = -1;
        visited[i] = 0; /* set ith node as unvisited */
    }

    n_visited = 0;
    while(n_visited < graph->n_nodes) {
        shortest = 10000;
        shortest_index = 0;
        for(i = 0; i < graph->n_nodes; i++) {
            if(distances[i] < shortest && !visited[i]) {
                shortest_index = i;
                shortest = distances[i];
            }
        }

        current = graph->nodes[shortest_index];
        visited[shortest_index] = 1;
        n_visited += 1;

        if(are_same_nodes(current, end)) {
            break;
        }

        for(i = 0; i < current->n_neighbors; i++) {
            neighbor = current->neighbors[i];
            index = get_node_index(graph, neighbor);
            alternative = distances[shortest_index] + current->weights[i];

            if(distances[index] > alternative) {
                distances[index] = alternative;
                previous[index] = shortest_index;
            }
        }
    }

    path_length = 0;
    u = get_node_index(graph, end);
    while(u != -1) {
        path_indices[path_length] = u;
        u = previous[u];
        path_length += 1;
    }

    Path *path = (Path *)malloc(sizeof(Path));
    char *p = (char *)malloc(path_length+1);
    for(i = 0; i < path_length; i++) {
        index = path_indices[path_length-i-1];
        p[i] = graph->nodes[index]->label;
    }
    p[path_length] = '\0';

    i = get_node_index(graph, end);
    path->path = p;
    path->cost = distances[i];
    return path;
}


//void show_path(Path *path, int show_cost) {
//    int i;
//    printf("  path: ");
//    for(i=0; i<strlen(path->path); i++) {
//        printf("%c ", path->path[i]);
//    }
//    if(show_cost) {
//        printf("cost: %lf", path->cost);
//    }
//    printf("\n");
//}

//////////////////////////////////////////////////////////




///////////////////////////////////////////////////////
Graph * A(char *ans, int len ){
    Graph *g=init_graph();
    int i=1;
    while (i<len-1){
        if (ans[i]=='n'){
            i++;
            Node *src = create_node(g,ans[i]);
            add_node(g,src);
            if (i>=len-2){
                break;
            }
            i++;
            while(ans[i]!='n'){
                Node *dest = create_node(g,ans[i]);
                add_node(g,dest);
                i++;
                add_edge(src,dest, ((ans[i])-'0'));
                i++;
            }

        }
    }
    for(int i=0; i<g->n_nodes; i++) {
        printf("%c\n ", g->nodes[i]->label);
    }
    return g;
}
///////////////////////////////////////////////////////////
void B(char ans [],Graph *graph){
    Node *src = create_node(graph,ans[0]);
    add_node(graph,src);
    int i=1;
    while(ans[i]!='\0'){
        Node *dest = create_node(graph,ans[i]);
        add_node(graph,dest);
        i++;
        add_edge(src,dest, ((ans[i])-'0'));
        i++;
    }
}

/////////////////////////////////////////////////////////
void D(char ans [],Graph *graph){
    Node *save;
    for (int i = 0; i <graph->n_nodes ; ++i) {
        if(graph->nodes[i]->label==ans[0]){
            save=graph->nodes[i];
        }
    }
    delete_node(graph,save);
}


//////////////////////////////////////////////////////////
// void S(char ans [],Graph *graph){
// Node *src = create_node(ans[0]);
// add_node(graph,src);
// Node *dest = create_node(ans[1]);
// add_node(graph,dest);
// printf( "short path: %s", dijkstra(graph,src,dest));
// }

///////////////////////////////////////////////////////////
void T(char ans [],Graph *graph){
//    int i=1;
//    int cunt=(int) (ans[0]);
//    while(ans[i]!=""|| ans[i]!='\0'){
//    Node *dest = create_node(ans[i]);
//    add_node(graph,dest);
//     i++;
//  }
//  Node arr[cunt];
//  for(int d=0; d<graph->n_nodes; d++){
//    arr[d]=*graph->nodes[d];
//  }
//  printf( "TSP: %S", tsp(graph,arr,cunt));
}



// Path *tsp(Graph *graph, Node *nodes_list[], int len){
//  if (len==0){
// return NULL;
// }
// Node path =(Node **)malloc(len)*sizeof(Node *));
// int j=0;
// Node id1=nodes_list[j];
// if (len==1){
// return id1;
//  }
// int w=0;
// for(int i=0; i<len-1; i++){
// Node id1=nodes_list[i];
// Node id2=nodes_list[i+1];
// Node cnt[];
// cnt=dijkstra(graph, id1,id2)
// if (i>0){
// for(int j=1; j<cntlen; j++){
// int h=0;
// cnt[h]=cnt[i];
// }
// }
// cnt = (Node **)realloc(cntlen-1)*sizeof(Node *));
// cntlen-=1;
// int save = path_length;
// path= (Node **)reallo(len+cntlen))*sizeof(Node *));
// for(int j=save; j<path_length; j++){
//     e=0;
// path[j]=cnt[e];
// e++;
// }
// return path
// }

#include <stdlib.h>


char* cut(char str[], int t, int i){
    char* str_cnt=(char*)malloc(t + 1);
    int g=0;
    int k = i;
    int cnt=0;
    while (cnt<t){
        str_cnt[g++]=str[k++];
        cnt++;
    }
    str_cnt[t]='\0';
    //printf("--------------\n %s",str_cnt);
    return str_cnt;
}


int main(){

    char f[1024];
    gets(f);
    int len= strlen(f)+1;
    char nn[len];
    strcpy(nn,cut(f , len, 0));
//    printf("%s\n",nn);
    int r=0, b=0;
    while(b<len){
        if(nn[b]==' '){
            r++;
        }
        b++;
    }
//    printf("%d\n",len);
//    printf("r=%d\n",r);
    char n[len-r];
    int k=0,j=0;
    while(k<len){
        if(nn[k]!=' '){
            n[j]=nn[k];
            j++;
        }
        k++;
    }
//    printf("len-r = %d\n", strlen(n));
    char cnt[strlen(n)-1];
    strcpy(cnt,cut(n , strlen(n)-1, 1));
//    printf("%s\n",cnt);
//    printf("cnt[strlen(n)-1]=%d\n",strlen(n)-1);
    int i=0;
    for (int j = 0; j < strlen(cnt); j++) {
        char c= cnt[j];
        if (c=='B'||c=='D'||c=='S'||c=='T'){
            break;
        }else{
            i++;
        }
    }


//    printf("i= %d\n",i);
//    printf("cnt= %s\n",cnt);
    int copy=0;
    char current[i+1];
    for (int l = 0; l <=i-1 ; l++) {
        current[copy]= cnt[l] ;
        printf("%c",current[copy]);
        copy++;
    }
    current[i]='\0';

    Graph *g=A(current, i);
    for (int i = 0; i < g->n_nodes; i++) {
        printf("srccc: %c - ", g->nodes[i]->label);
    for (int l = 0; l < g->nodes[i]->n_neighbors; ++l) {
        printf("kokod %c - ", g->nodes[i]->neighbors[l]->label);
        printf("wight %f - ", g->nodes[i]->weights[l]);

    }
        printf("\n");
    }


    char ans[strlen(cnt)-i];
    strcpy(ans,cut(cnt , (strlen(cnt)-i),i)) ;
    printf("-------%s\n",ans);
    ans[strlen(cnt)-i]='\0';
    while (strlen(ans)!=0) {
        char first = ans[0];
        if (first == 'B') {
            int t = 0;
            for (int j = 1; j < strlen(ans); j++) {
                char c = ans[j];
                if (c == 'B' || c == 'D' || c == 'S' || c == 'T' || c == '\0') {
                    break;
                } else {
                    t++;
                }
            }

//            printf("t=%d\n", t);
//            printf("b=%s\n", cut(ans, t, 1));
            B(cut(ans, t, 1), g);
            for (int i = 0; i < g->n_nodes; i++) {
                printf("%c-  ", g->nodes[i]->label);
            }
//            printf("bbbbbbbbbbbbbbbbb=%s\n", cut(ans, (strlen(ans) - t - 1), t + 1));
            strcpy(ans, cut(ans, (strlen(ans) - t - 1), t + 1));
            continue;
        }

        for (int i = 0; i < g->n_nodes; i++) {
            printf("srccc: %c - ", g->nodes[i]->label);
            for (int l = 0; l < g->nodes[i]->n_neighbors; ++l) {
                printf("kokod %c - ", g->nodes[i]->neighbors[l]->label);
                printf("wight %f - ", g->nodes[i]->weights[l]);

            }
            printf("\n");
        }

        if (first == 'D') {
            int t = 0;
            for (int j = 1; j < strlen(ans); j++) {
                char c = ans[j];
                if (c == 'B' || c == 'D' || c == 'S' || c == 'T' || c == '\0') {
                    break;
                } else {
                    t++;
                }
            }
            printf("d=%d\n", t);
            D(cut(ans, t, 1), g);
            strcpy(ans, cut(ans, (strlen(ans) - t - 1), t + 1));

        }

        for (int i = 0; i < g->n_nodes; i++) {
            printf("srccc: %c - ", g->nodes[i]->label);
            for (int l = 0; l < g->nodes[i]->n_neighbors; ++l) {
                printf("kokod %c - ", g->nodes[i]->neighbors[l]->label);
                printf("wight %f - ", g->nodes[i]->weights[l]);

            }
            printf("\n");
        }
    }
//
//        if (first == 'S') {
//            int t = 0;
//            for (int j = 1; j < strlen(ans); j++) {
//                char c = cnt[j];
//                if (c == 'B' || c == 'D' || c == 'S' || c == 'T'|| c =='\0') {
//                    break;
//                } else {
//                    t++;
//                }
//            }
//            // S(cut(ans, strlen(ans), 1, 1+t), g);
//            ans = cut(ans, strlen(ans), t+2, strlen(ans));
//        }
//
//        if (first == 'T') {
//            int t = 0;
//            for (int j = 1; j < strlen(ans); j++) {
//                char c = cnt[j];
//                if (c == 'B' || c == 'D' || c == 'S' || c == 'T'|| c =='\0') {
//                    break;
//                } else {
//                    t++;
//                }
//            }
//
//            T(cut(ans, strlen(ans), 1,1+t), g);
//            ans = cut(ans, strlen(ans), t+2, strlen(ans));
//        }

    }
//    show_graph(g);
//    return 0;
//
//}
//A 4 n 0 2 5 3 3 n 2 0 4 1 1 n 1 3 7 0 2 n 3 B 5 0 4 2 1 B 2 1 3 5 1 D 5
