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

Graph* createGraph(Graph* g) 
{
	int i;

	if (g != NULL) //생성된 그래프가 있으면
		destroyGraph(g); //그래프 지우기
	g = (Graph*)malloc(sizeof(Graph)); //그래프 생성!
	g->vlist = (VertexHead *)malloc(sizeof(VertexHead) * MAX_VERTEX); //인접 리스트 생성
	for (i = 0; i < MAX_VERTEX; i++)
		(g->vlist + i)->head = NULL; //헤드노드(정점 노드) NULL
	
	return g;
}

/* 정점에 연결된 간선들 전부 삭제 */
void freenode(Vertex* adhead) 
{
	if (adhead->link != NULL) //다음 간선이 존재할 때
		freenode(adhead->link); //간선들 삭제!
	free(adhead); //첫 번째 간선을 포함한 모든 간선 해제
}

/* 그래프 삭제 */
int destroyGraph(Graph* g) 
{
	int i;
	
	VertexHead* gp = g->vlist; //인접 리스트를 가리키는 포인터
	for (i = 0; i < MAX_VERTEX; i++)
		if ((gp + i)->head != NULL) //vlist에 정점이 있다면
			freenode((gp + i)->head); //정점과 간선(인접 정점) 삭제
	free(gp); //인접 리스트 삭제
	free(g); //그래프 삭제
	return 0;
}

/* 정점 추가 */
int insertVertex(Graph* g, int v) 
{
	if (v <= 0 || v > MAX_VERTEX) { //잘못된 입력 처리
		printf("Wrong input.\n");
		return -1;
	}

	int vv = v - 1; //자연수 -> 배열의 원소 기준 정수
	VertexHead* adj = g->vlist; //인접 리스트를 가리키는 포인터
	if ((adj + vv)->head == NULL) { //해당 정점이 비어있다면
		Vertex* node = (Vertex*)malloc(sizeof(Vertex)); //노드 생성
		node->num = vv; //식별용 - 탐색 시 첫 번째 노드가 됨, 출력의 기준이 되어준다.
		node->link = NULL;
		(adj + vv)->head = node; //헤드노드로 지정
		return 1; //바로 함수 종료
	}
	//정점이 생성되어있다면
	printf("Vertex already exist: %d\n", v);
	return 0;
}
