#include <stdio.h>
#include <stdlib.h>
#include <string.h>


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
    return str_cnt;
}

typedef struct node_ {
    char id;
    int number_Of_neighbors;
    struct node_ **neighbors; /* list of pointers of adjacent nodes */
    double *weights;
} Node;

typedef struct graph_ {
    int number_Of_nodes;
    struct node_ **nodes; /* list of pointers of nodes */
} Graph;


///////////////////////////////////////////////////////////////////////////////

Graph *init_graph() {
    Graph *graph = (Graph *)malloc(sizeof(Graph));
    graph->number_Of_nodes = 0;
    graph->nodes = (Node **)malloc(graph->number_Of_nodes * sizeof(Node *));
    return graph;
}

void free_graph(struct graph_ *graph) {
    int i;
    for(i=0; i<graph->number_Of_nodes; i++) {
        free(graph->nodes[i]);
    }
    free(graph->nodes);
    free(graph);
}

////////////////////////////////////////////////////////////////////////////////

Node *create_node(Graph *graph, char label) {
    for(int i=0; i<graph->number_Of_nodes; i++){
        if(label==graph->nodes[i]->id){
            return graph->nodes[i];
        }}
    Node *node;
    node = (Node *) malloc(sizeof(Node));
    node->id = label;
    node->number_Of_neighbors = 0;
    node->neighbors = (Node **) malloc(node->number_Of_neighbors * sizeof(Node *));
    if(node->neighbors==NULL){
        exit(1);
    }
    node->weights = (double *) malloc(node->number_Of_neighbors * sizeof(double));
    if(node->weights==NULL){
        exit(1);
    }
    return node;
}

void add_node(Graph *graph, Node *node) {
    for(int i=0; i<graph->number_Of_nodes; i++){
        if(node->id == graph->nodes[i]->id){
           graph->nodes[i]=node;
            return;
        }
    }
    graph->nodes = (Node **)realloc(graph->nodes, (graph->number_Of_nodes + 1) * sizeof(Node *));
    if(graph->nodes==NULL){
        exit(1);
    }
    graph->nodes[graph->number_Of_nodes] = node;
    graph->number_Of_nodes += 1;
}


void add_edge(Node *a, Node *b, double weight) {
    a->neighbors = (Node **)realloc(a->neighbors, (a->number_Of_neighbors + 1) * sizeof(Node *));
    if(a->neighbors ==NULL){
        exit(1);
    }
    a->weights = (double *)realloc(a->weights, (a->number_Of_neighbors + 1) * sizeof(double));
    if(a->weights==NULL){
        exit(1);
    }
    a->neighbors[a->number_Of_neighbors] = b;
    a->weights[a->number_Of_neighbors] = weight;
    a->number_Of_neighbors += 1;
}

///////////////////////////////////////////////////////////////////////////
void delete_edge(Graph *graph, Node *a, Node *b) {
    int save=0;
    int j=0;
    for (int i = 0; i <a->number_Of_neighbors; i++) {
        if(a->neighbors[i]->id != b->id){
            a->neighbors[j]=a->neighbors[i];
            j++;
        }else{
            save=i;

        }
    }
    int d=save+1;
    for (int i = d; i <a->number_Of_neighbors; i++) {
        a->weights[save]=a->weights[i];
        save++;
    }
    a->neighbors = (Node **)realloc(a->neighbors, (a->number_Of_neighbors - 1) * sizeof(Node *));
    if(a->neighbors==NULL){
        exit(1);
    }
    a->number_Of_neighbors -= 1;
    a->weights = (double *)realloc(a->weights, (a->number_Of_neighbors) * sizeof(double));
    if(a->weights==NULL){
        exit(1);
    }
}


void delete_node(Graph *graph, Node *node) {
    int j=0;
    for (int i = 0; i <graph->number_Of_nodes; i++) {
        if (graph->nodes[i]->id != node->id) {
            graph->nodes[j] = graph->nodes[i];
            j++;
        }}
    graph->nodes = (Node **) realloc(graph->nodes, (graph->number_Of_nodes - 1) * sizeof(Node *));
    if(graph->nodes==NULL){
        exit(1);
    }
    graph->number_Of_nodes -= 1;
    for (int i = 0; i <graph->number_Of_nodes; i++){
        Node *s =graph->nodes[i];
        for (int j = 0; j <graph->nodes[i]->number_Of_neighbors; j++){
            if(s->neighbors[j]->id == node->id){
                delete_edge(graph,s,node);
            }
        }}

}
///////////////////////////////////////////////////////////////////////////////////
int getNodeIndex(Graph *graph, Node *node) {
    int i;
    Node *n;
    for(i = 0; i < graph->number_Of_nodes; i++) {
        n = graph->nodes[i];
        if(n->id == node->id) {
            return i;
        }
    }
    printf("id : %c\n", node->id);
    printf("Invalid id detected.\n");
    return -1;
}

int sameNodes(Node *id1, Node *id2) {
    return id1 == id2 && id1->id == id2->id;
}

int Dijsktra(Graph *graph, Node *start, Node *end) {
    Node *neighbor,*node,*cur;
    int dist[graph->number_Of_nodes];
    int previous[graph->number_Of_nodes];
    int visit[graph->number_Of_nodes];
    double alternative,shortest ;
    int n_visit, indx, shortest_indx,i;

    for(i = 0; i < graph->number_Of_nodes; i++) {
        node = graph->nodes[i];
        if(node->id == start->id) {
            dist[i] = 0;}
        else {
            dist[i] = 1000;}
        previous[i] = -1;
        visit[i] = 0;
    }

    n_visit = 0;
    while(n_visit < graph->number_Of_nodes) {
        shortest = 1000;
        shortest_indx = 0;
        for(i = 0; i < graph->number_Of_nodes; i++) {
            if(dist[i] < shortest && !visit[i]) {
                shortest_indx = i;
                shortest = dist[i];
            }
        }

        cur = graph->nodes[shortest_indx];
        visit[shortest_indx] = 1;
        n_visit += 1;

        if(sameNodes(cur, end)) {
            break;
        }

        for(i = 0; i < cur->number_Of_neighbors; i++) {
            neighbor = cur->neighbors[i];
            indx = getNodeIndex(graph, neighbor);
            alternative = dist[shortest_indx] + cur->weights[i];

            if(dist[indx] > alternative) {
                dist[indx] = alternative;
                previous[indx] = shortest_indx;
            }
        }
    }

    i = getNodeIndex(graph, end);
    free(visit);
    free(previous);
    int w = dist[i];
    free(dist);
    return w;
}




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

    return g;
}
///////////////////////////////////////////////////////////
void B(char ans [],Graph *graph){
    Node *node;
    node = (Node *) malloc(sizeof(Node));
    node->id = ans[0];
    node->number_Of_neighbors = 0;
    node->neighbors = (Node **) malloc(node->number_Of_neighbors * sizeof(Node *));
    if(node->neighbors==NULL){
        exit(1);
    }
    node->weights = (double *) malloc(node->number_Of_neighbors * sizeof(double));
    if(node->weights==NULL){
        exit(1);
    }

    add_node(graph,node);
    int i=1;
    while(ans[i]!='\0'){
        Node *dest = create_node(graph,ans[i]);
        add_node(graph,dest);
        i++;
        add_edge(node,dest, ((ans[i])-'0'));
        i++;
    }
}

/////////////////////////////////////////////////////////
void D(char ans [],Graph *graph){
    Node *save;
    for (int i = 0; i <graph->number_Of_nodes ; ++i) {
        if(graph->nodes[i]->id == ans[0]){
            save=graph->nodes[i];
        }
    }
    delete_node(graph,save);
}


//////////////////////////////////////////////////////////
 void S(char ans [],Graph *graph){
    Node *src;
    for (int i = 0; i <graph->number_Of_nodes ; ++i) {
        if(graph->nodes[i]->id == ans[0]){
            src=graph->nodes[i];
        }
    }
    Node *dest;
    for (int i = 0; i <graph->number_Of_nodes ; ++i) {
        if(graph->nodes[i]->id == ans[1]){
            dest=graph->nodes[i];
        }
    }
 printf("Dijsktra shortest path: %d\n", Dijsktra(graph, src, dest));
 }
///////////////////////////////////////////////////////////

int tsp(Graph *graph, char list [], int len){
    if (len==1||len==0){
        return NULL;
    }
    if (len==2){
        return 0;
    }
    int w=0;
    for(int d=1; d<len-1; d++) {
        Node *save_id1;
        for (int i = 0; i < graph->number_Of_nodes; ++i) {
            if (graph->nodes[i]->id == list[d]) {
                save_id1 = graph->nodes[i];
            }
        }
        Node *save_id2;
        for (int i = 0; i < graph->number_Of_nodes; ++i) {
            if (graph->nodes[i]->id == list[d + 1]) {
                save_id2 = graph->nodes[i];
            }
        }
        w+= Dijsktra(graph, save_id1, save_id2);

    }
    if(w>=1000){
        return -1;
    }

    return w;
}
//////////////////////////////////////////////
void T(char ans [],Graph *graph){
  printf( "TSP shortest path: %d\n", tsp(graph,ans, strlen(ans)));
}

//////////////////////////////////////////////////////
int main(){
    char f[1024];
    gets(f);
    int len= strlen(f)+1;
    char nn[len];
    strcpy(nn,cut(f , len, 0));
    int r=0, b=0;
    while(b<len){
        if(nn[b]==' '){
            r++;
        }
        b++;
    }
    char n[len-r];
    int k=0,j=0;
    while(k<len){
        if(nn[k]!=' '){
            n[j]=nn[k];
            j++;
        }
        k++;
    }
    char cnt[strlen(n)-1];
    strcpy(cnt,cut(n , strlen(n)-1, 1));
    int i=0;
    for (int j = 0; j < strlen(cnt); j++) {
        char c= cnt[j];
        if (c=='B'||c=='D'||c=='S'||c=='T'||c=='\0'|| c=='A'){
            break;
        }else{
            i++;
        }
    }

    int copy=0;
    char current[i+1];
    for (int l = 0; l <=i-1 ; l++) {
        current[copy]= cnt[l] ;
        copy++;
    }
    current[i]='\0';
    Graph *g=A(current, i);
    char ans[strlen(cnt)-i];
    strcpy(ans,cut(cnt , (strlen(cnt)-i),i)) ;
    ans[strlen(cnt)-i]='\0';
    while (strlen(ans)!=0) {
        char first = ans[0];
        if (first == 'B') {
            int t = 0;
            for (int j = 1; j < strlen(ans); j++) {
                char c = ans[j];
                if (c == 'B' || c == 'D' || c == 'S' || c == 'T' || c == '\0' || c == 'A') {
                    break;
                } else {
                    t++;
                }
            }
            B(cut(ans, t, 1), g);
            strcpy(ans, cut(ans, (strlen(ans) - t - 1), t + 1));
        }


        if (first == 'D') {
            int t = 0;
            for (int j = 1; j < strlen(ans); j++) {
                char c = ans[j];
                if (c == 'B' || c == 'D' || c == 'S' || c == 'T' || c == '\0' || c == 'A') {
                    break;
                } else {
                    t++;
                }
            }

            D(cut(ans, t, 1), g);
            strcpy(ans, cut(ans, (strlen(ans) - t - 1), t + 1));
        }

        if (first == 'A') {
            int t = 0;
            for (int j = 1; j < strlen(ans); j++) {
                char c = ans[j];
                if (c == 'B' || c == 'D' || c == 'S' || c == 'T' || c == '\0'|| c=='A') {
                    break;
                } else {
                    t++;
                }
            }
            g=A(cut(ans, t, 1), t);
            strcpy(ans, cut(ans, (strlen(ans) - t - 1), t + 1));
        }


        if (first == 'S') {
            int t = 0;
            for (int j = 1; j < strlen(ans); j++) {
                char c = ans[j];
                if (c == 'B' || c == 'D' || c == 'S' || c == 'T' || c == '\0' || c == 'A') {
                    break;
                } else {
                    t++;
                }
            }

            S(cut(ans, t, 1), g);
            strcpy(ans, cut(ans, (strlen(ans) - t - 1), t + 1));
        }


        if (first == 'T') {
            int t = 0;
            for (int j = 1; j < strlen(ans); j++) {
                char c = ans[j];
                if (c == 'B' || c == 'D' || c == 'S' || c == 'T' || c == '\0' || c == 'A') {
                    break;
                } else {
                    t++;
                }
            }
            T(cut(ans, t, 1), g);
            strcpy(ans, cut(ans, (strlen(ans) - t - 1), t + 1));
        }
    }
//    for (int i = 0; i < g->number_Of_nodes; i++) {
//        printf("A - srccc: %c - ", g->nodes[i]->id);
//        for (int l = 0; l < g->nodes[i]->number_Of_neighbors; ++l) {
//            printf("kokod %c - ", g->nodes[i]->neighbors[l]->id);
//            printf("wight %f - ", g->nodes[i]->weights[l]);
//
//        }
//        printf("\n");
//    }
    free_graph(g);
    free(f);
    free(n);
    free(nn);
    free(cnt);
    free(current);
    free(ans);

////////////////////////////////////////////

    

}


//A 4 n 0 2 5 3 3 n 2 0 4 1 1 n 1 3 7 0 2 n 3 B 5 0 4 2 1 B 2 1 3 5 1 D 5 A 2 n 0 1 2 n 1 0 3
