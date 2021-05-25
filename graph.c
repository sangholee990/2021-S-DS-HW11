#include <stdio.h>
#include <stdlib.h>
#define MAX_VERTEX 10


/* 그래프의 정점 노드 */
typedef struct Vertex {
	int num;
	struct Vertex* link;
} Vertex;

/* 그래프의 정점에 대한 헤드 노드 */
typedef struct VertexHead {
	Vertex* head;
} VertexHead;

/* 각 헤드 노드를 가리키는 그래프 인접 리스트 */
typedef struct Graph {
	VertexHead* vlist;
} Graph;

/* Array for DFS  */
Vertex* stack[MAX_VERTEX];
int top = -1;
Vertex* pop();
void push(Vertex* vtex);


/* Queue for BFS */
Vertex* queue[MAX_VERTEX];
int front = -1;
int rear = -1;
Vertex* deQueue();
void enQueue(Vertex* vtex);



/* Functions */
Graph* createGraph(Graph* g); /* empty graph creation */
int destroyGraph(Graph* g); /* deallocating all allocated memory */
int insertVertex(Graph* g, int v); /* vertex insertion */
void deleteVertex(Graph* g, int v); /* vertex deletion */
int insertEdge(Graph* g, int u, int v); /* new edge creation between two vertices */
void deleteEdge(Graph* g, int u, int v); /* edge removal */
void depthFS(Graph *g); /* depth first search using stack */
void breadthFS(Graph* g); /* breadth first search using queue */
void printGraph(Graph* g); /* printing graph with vertices and edges */

/* Additional Functions */
void freenode(Vertex* adhead); /* delete all edges that connect to Vertex */
int findfirst(VertexHead* vlist); /* return first index where vertex exist */

int main()
{
	char command;
	int key, u, v;
	Graph* g = NULL;

	do {
		printf("\n\n");
		printf("----------------------------------------------------------------\n");
		printf("                        Graph  List                             \n");
		printf("----------------------------------------------------------------\n");
		printf("             [------- [이상호] [2018038016] -------]            \n");
		printf("----------------------------------------------------------------\n");
		printf(" Initialize Graph     = z                                       \n");
		printf(" Insert Vertex        = v      Insert Edge                  = e \n");
		printf(" Depth First Search   = d      Breath First Search          = b \n");
		printf(" Print Graph          = p      Quit                         = q \n");
		printf("----------------------------------------------------------------\n");

		printf("Command = ");
		scanf(" %c", &command);

switch (command) {
	
		case 'z': case 'Z':
			g = createGraph(g);
			break;
	
		case 'p': case 'P':
			printGraph(g);
			break;
			
		case 'v': case 'V':
			printf("Vertex Number = ");
			scanf("%d", &key);
			insertVertex(g, key);
			break;
			
		case 'e': case 'E':
			printf("Edge Number(Input ex: 1 2) = ");
			scanf("%d %d", &u, &v);
			insertEdge(g, u, v);
			break;
					
		case 'd': case 'D':
			depthFS(g);
			break;
			
		case 'b': case 'B':
			breadthFS(g);
			break;

		default:
			printf("\n       >>>>>   Concentration!!   <<<<<     \n");
			break;
		}

	} while (command != 'q' && command != 'Q');

	return 1;
}
