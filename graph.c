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

/* 그래프 생성 */
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


 /* 정점 삭제 */
void deleteVertex(Graph* g, int v) 
{
	
	int i;
	if (v <= 0 || v > MAX_VERTEX) { //잘못된 입력 처리
		printf("Wrong input.\n");
		return;
	}

	int vv = v - 1; //자연수 -> 배열의 원소 기준 정수
	VertexHead* adj = g->vlist; //인접 리스트를 가리키는 포인터
	if ((adj + vv)->head != NULL) { //해당 정점이 존재한다면
		for (i = 0; i < MAX_VERTEX; i++) { 
			//다른 정점에 연결된 해당 Vertex의 간선 삭제
			if (((adj + i)->head == NULL) || (i == vv))
				//정점이 비어있거나, 삭제해야 할 정점이라면
				continue; //패스
			else { //정점이 존재한다면
				Vertex* p = (adj + i)->head; //해당 정점의 첫 번째 노드 가리키는 포인터 생성
				while (p != NULL) { //포인터가 끝까지 체크할동안
					if (p->num == vv) { //해당 노드와 연결된 간선을 발견하면
						p = p->link; //다음 링크(삭제할 노드의 다음으로) 이동, p의 값을 잃지 않기 위함
						deleteEdge(g, i + 1, v); //해당 간선 삭제
					}
					else 
						p = p->link; //해당 노드의 간선이 아니라면 다음 인접 노드로 이동
				}
			}
		}
		freenode((adj + vv)->head); //정점을 포함한 모든 간선 삭제
		(adj + vv)->head = NULL; //NULL처리
		return;
	}
	printf("no %d to delete.\n", v);
	return;
}

/* 간선 생성 */
int insertEdge(Graph* g, int u, int v) 
{
	if (u <= 0 || u > MAX_VERTEX || v <= 0 || v > MAX_VERTEX || u == v) { //잘못된 입력 처리
		printf("Wrong input.\n");
		return -1;
	}

	VertexHead* adj = g->vlist; //인접 리스트를 가리키는 포인터
	int uu = u - 1, vv = v - 1; //자연수 -> 배열의 원소 기준 정수

	// u, v중 하나의 vertex라도 존재하지 않을 때
	if (((adj + uu)->head == NULL) || ((adj + vv)->head == NULL)) {
		printf("[%d], [%d] vertexs not exist.\n", u, v);
		return -1;
	}

	/* 간선의 방향: u->v  
	 * 간선은 해당 인접 정점 리스트의 마지막에 추가된다.
	 * 탐색 시 먼저 생성된 간선부터 탐색. */
	Vertex* node = (Vertex*)malloc(sizeof(Vertex)); //간선 노드 생성
	node->num = vv; //해당 간선 수 지정
	node->link = NULL; //마지막에 추가되므로 링크는 NULL

	Vertex* p = (adj + uu)->head, * prev = NULL; //리스트 순회용 포인터
	/* 리스트의 마지막으로 순회 */
	while (p != NULL) { //p가 끝까지 갈 때까지
		if(p->num == node->num){ //간선이 이미 존재한다면
		    printf("Edge exist.\n");
		    free(node); //새로 만들었던 메모리 해제 
		    return 0; //간선을 추가하지 않고 종료
		}
		prev = p; //prev는 p로 이동(노드 1칸 이동)
		p = p->link; //p는 p다음 노드로 이동(노드 1칸 이동)
	}
	/* prev가 마지막 노드를 가리키므로 prev 뒤에 노드 추가 */
	prev->link = node; //마지막 노드의 링크가 새로 생성한 노드를 가리킴
	node->link = p; //새로 생성한 노드의 링크는 뒤 노드(NULL)를 가리킴

	return 0;
}

/* 간선 삭제 */
void deleteEdge(Graph* g, int u, int v) 
{
	if (u <= 0 || u > MAX_VERTEX || v <= 0 || v > MAX_VERTEX || u == v) { //잘못된 입력 처리
		printf("Wrong input.\n");
		return ;
	}

	int uu = u - 1, vv = v - 1; //자연수 -> 배열의 원소 기준 정수
	VertexHead* adj = g->vlist; //인접 리스트를 가리키는 포인터
	Vertex* p = (adj + uu)->head, * prev = (adj + uu)->head; //서치를 위해 u값의 리스트를 가리키는 포인터

	// u, v중 하나의 vertex라도 존재하지 않을 때
	if (((adj + uu)->head == NULL) || ((adj + vv)->head == NULL)) {
		printf("[%d], [%d] vertexs not exist.\n", u, v);
		return;
	}

	// 간선의 방향: u->v
	// 정점 u에 존재하는 간선 v 삭제
	while (p != NULL) { //마지막 노드(정점 노드)로 가기 전까지
		if (p->num == vv) { //해당하는 간선을 찾으면
			if (prev == p) //그 간선이 첫 번째에 위치할 때
				(adj + uu)->head = p->link; //헤드노드를 두 번째 노드로 지정
			else //두 번째 이상 순서일 때
				prev->link = p->link; //이전 노드의 링크를 해당 노드의 다음 노드로 지정
			free(p); //해당 간선 노드 삭제
			return;
		}
		//해당 간선이 아니면
		prev = p; 
		p = p->link;
		//포인터 이동
	}
	//간선을 찾지 못하면
	printf("no edge exist: (%d, %d)", u, v);
}

/* return first index where vertex exist */
int findfirst(VertexHead* vlist) 
{
	int index = 0;
	while (index < MAX_VERTEX) { 
		//배열에서 가장 작은 활성화 된 정점(인덱스)를 찾아서
		if ((vlist + index)->head != NULL)
			break;
		index++;
	}

	return index; //해당 인덱스 리턴
}

/* DFS using stack */
void depthFS(Graph *g) 
{
	
	if (g->vlist == NULL) { //그래프 미생성 처리
		printf("Empty Graph.\n");
		return;
	}
	VertexHead* pp = g->vlist; //인접 리스트를 가리키는 포인터
	int minvertex = findfirst(pp); //가장 작은 인덱스 저장
	Vertex* p = (pp + minvertex)->head; //첫 번재 인덱스가 가리키는 리스트의 헤드노드 저장
	int* visited = (int*)calloc(MAX_VERTEX, sizeof(int)); //방문 여부를 판별할 배열 생성

	//루트노드 
	visited[minvertex] = 1; //가장 작은 수의 정점 방문 확인
	printf("%d ", minvertex + 1); //해당 정점 출력
	push(p); //minvertex에 해당하는 인접 리스트 push
	p = (pp + p->num)->head; //p의 값을 첫 번째 인접 노드의 헤드로 수정

	while (top != -1) { //스택이 비어있지 않는 동안
		p = pop(); //더 찾아볼 인접 정점이 없으면 pop

		/* 스택 예외처리 = pop했더니 p가 가장 위에 위치한 노드 */
		if (p == (pp + minvertex)->head) {
			while (p != NULL)
				if (visited[p->num] == 0) { //방문하지 않은 인접 정점이 있다면
					push(p); //다시 push
					break;
				}
				else
					p = p->link;
		}

		while (p != NULL) { //해당 정점의 인접 정점 끝까지
			if (visited[p->num] == 0) { //방문하지 않은 노드를 찾으면
				visited[p->num] = 1; //방문 처리
				printf("%d ", (p->num) + 1); //해당 노드 출력
				push((pp + p->num)->head); //해당 노드와 같은 값을 갖는 인접 리스트 push
				p = (pp + p->num)->head; //p의 값을 첫 번째 인접 노드의 헤드로 수정
			}
			else //이미 방문한 노드라면
				p = p->link; //다음 인접 정점으로 이동
		}
	}
	free(visited); //방문 판별 배열 해제
}

/* BFS using queue */
void breadthFS(Graph* g) 
{
	if (g->vlist == NULL) { //그래프 미생성 처리
		printf("Empty Graph.\n");
		return;
	}
	
	VertexHead* pp = g->vlist; //인접 리스트를 가리키는 포인터
	int minvertex = findfirst(pp); //가장 작은 인덱스 저장
	Vertex* p = (pp + minvertex)->head; //첫 번재 인덱스가 가리키는 리스트의 헤드노드 저장
	int* visited = (int*)calloc(MAX_VERTEX, sizeof(int)); //방문 여부를 판별할 배열 생성

	//루트노드 삽입
	visited[minvertex] = 1; //가장 작은 수의 정점 방문 확인
	printf("%d ", minvertex + 1); //해당 정점 출력
	enQueue(p); //minvertex에 해당하는 인접 리스트의 첫 번째 enqueue

	while (1) {
		p = deQueue(); //큐에서 빼낸 후
		if (p == NULL)
			break;
		while (p != NULL) { //빼낸 정점의 인접 정점 찾기
			if (visited[p->num] == 0) { //방문하지 않은 정점이면
				visited[p->num] = 1; //방문 처리
				printf("%d ", (p->num) + 1); //정점 출력
				enQueue((pp + p->num)->head); //해당 정점 큐에 넣기
			}
			/* 해당 정점의 방문하지 않은 정점을 전부 큐에 넣음으로써 다음 레벨로 내려감*/
			p = p->link; //다음 인접 정점으로
		}
	}
	free(visited); //방문 판별 배열 해제
}

/* 그래프 출력 */
void printGraph(Graph* g) 
{
	int i;
	if (g->vlist == NULL) { //그래프 미생성 처리
		printf("Empty Graph.\n");
		return;
	}

	VertexHead* gp = g->vlist; //인접 리스트를 가리키는 포인터
	Vertex *p; //노드 서치용 포인터
	int count = 0; //프린트 카운트 포인터

	for (i = 0; i < MAX_VERTEX; i++) {
		p = (gp + i)->head; //각 인접 리스트의 헤드로 지정

		if (p != NULL) { //정점이 생성되어있다면
			printf("vertex [%d]", i + 1); //정점 프린트
			count++; //정점 1개 프린트 시마다 증가
			p = p->link; //첫 번째 노드는 해당 정점이므로 인접 정점으로 이동
			while (p != NULL) { //정점의 모든 인접 노드 탐색
				printf(" -> %d", (p->num) + 1); //인접 노드 프린트
				p = p->link; //다음 인접 노드로 이동
			}
			printf("\n");
		}
	}
	if (count == 0) //count값이 그대로면
		printf("Nothing to print - Empty Vertex.\n"); //정점이 없음을 출력
}


///for stack
/* 스택에서 꺼내기 */
Vertex* pop() 
{
	Vertex* x;
	if (top == -1)
		return '\0'; //스택이 이미 비어있을 땐 null문자 반환
	else {
		x = stack[top--];
	}
	return x; //끝부분이 한 칸 줄어들은 스택 반환
}
/* 스택에 추가하기 */
void push(Vertex* vtex) 
{
	stack[++top] = vtex; //스택에 해당 노드 값 추가
}
///


/// for queue
/* 큐에서 꺼내기 */
Vertex* deQueue() 
{
	if (front == rear) //큐가 비어있으면
		return '\0'; //NULL 반환
	else {
		front = (front + 1) % MAX_VERTEX; //front의 값을 1 증가(3일 땐 0으로)
		return queue[front];
	}
}
/* 큐에 넣기 */
void enQueue(Vertex* vtex)
{
	rear = (rear + 1) % MAX_VERTEX; //rear의 값을 1 증가(3일 땐 0으로)
	if (front == rear) { //큐가 꽉 차있으면
		printf(" Full Queue \n"); //꽉 찼다고 출력하고 들어온 값은 넣지 않음
		rear = (rear - 1) % MAX_VERTEX; //rear의 값을 원래대로 돌려주기
	}
	else
		queue[rear] = vtex; //증가된 rear가 위치한 자리에 문자 넣기
}
