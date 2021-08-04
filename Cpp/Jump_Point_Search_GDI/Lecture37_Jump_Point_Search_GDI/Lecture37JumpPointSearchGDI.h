#pragma once

#include "resource.h"
#include "Define.h"

#define MAX_LOADSTRING 100

// 전역 변수:
HINSTANCE hInst;                                // 현재 인스턴스입니다.
WCHAR szTitle[MAX_LOADSTRING];                  // 제목 표시줄 텍스트입니다.
WCHAR szWindowClass[MAX_LOADSTRING];            // 기본 창 클래스 이름입니다.

// 이 코드 모듈에 들어 있는 함수의 정방향 선언입니다.
ATOM                MyRegisterClass(HINSTANCE hInstance);
BOOL                InitInstance(HINSTANCE, int);
LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);


// 이 모듈 내에서 쓰이는 변수들
POINT				m_ptMousePos;					// 마우스의 좌표
HBITMAP				MemBit;							// 비트멥 정보 (백버퍼?)


NODE *StartNode;
NODE *EndNode;

list<NODE*>OpenList;
list<NODE*>CloseList;

list<NODE*>WallList; // 벽 정보를 담는다.
list<NODE*>PathList; // 경로의 노드 목록.

vector<NODE*>PathNodes;
list<NODE*>OptimizedList; // 최적화된 경로 (끝 -> 시작)
vector<NODE*>Optimizes;// 최적화 경로 정렬용...

NODE g_Map[MAX_BLOCK_COUNT_HIGHT][MAX_BLOCK_COUNT_WIDTH]; // 일단 칸 갯수에 맞게 생성

bool IsDrawWall;
bool IsRemoveWall;

bool IsEditBlockMode;

// 단순 직선거리 거리구하기 - ?? 휴리스틱
// GetH
// 두 점간의 최단 직선거리를 구한다.
// 멘하탄 방식? 두 점간의 절대값을 그냥 더함?
int Heuristic(int posX, int posY, int endX, int endY)
{
	return static_cast<int>((abs(static_cast<double>(posX - endX)) + abs(static_cast<double>(posY - endY))));
}

// PathFinder랑 반대긴 한대 결국
// H 값이 낮을 수록 유두리 있는거고 H값이 높을수록 빡빡하게 나온다.
// 목적지 위주일 수록 지그재그로 나온다.
int g_weight = 1; // 1 : 빡빡하게 탐색 : 목적지 위주 // 5 : 유두리있게 탐색 (넓직하게)

void InitMap();
void FindPath(NODE* startNode, NODE *endNode);

int malloc_count = 0;
void FindPath2(NODE* startNode, NODE *endNode);

NODE* GetSubNodes(NODE* currentNode);						// 가장 중요한 연산 함수 (a* )
NODE* FindNodeOnList(list<NODE*>nodeList, NODE pNode);
void InitWallList();
void ClearStartNode();
void ClearEndNode();
void Reset();
void Clear();
void Ready(HWND hWnd);
void DrawBitMap(HDC hdc, int x, int y, HBITMAP hBit);

// 설계 따라해보기
void IdentifySuccessors(NODE* currentNode); // 성공자 판단. 
void _identifySuccessors(NODE* activeNode, open_list* current, NODE* endNode);

vector<NODE>FindNeighbors(NODE* pNode);	// 이웃목록 검사 - (이웃 목록은 제대로 구하고 있는가?)
// 대각선 진행 방향의 경우 (강제된 이웃)포함 5방향의 노드가 이웃이다.
struct neighbor_xy_list *_findNeighbors(NODE* activeNode);
struct neighbor_list *getNeighbors(NODE* nd);
struct neighbor_list *insert_right(neighbor_list *list, NODE *data);

void clean_neighbor_list(neighbor_list *head)
{
	if (head != NULL) {
		struct neighbor_list *pos = head;
		struct neighbor_list *tmp = head;
		do {
			tmp = pos->right;
			free(pos);
			malloc_count--; /* [ Malloc Count ] */
			pos = tmp;
		} while (pos != head);
	}
}

struct NODE getNodeAt(int x, int y)
{
	//return &g_Map->nodes[y][x];
	return g_Map[y / CELL_SIZE][x / CELL_SIZE];
}

int euclidean(int dx, int dy)
{
	double distance = sqrt((double)(dx * dx + dy * dy)) * 10.0;
	int idistance = (int)distance;

	return idistance;
}

int manhattan(int dx, int dy)
{
	int distance = (dx + dy) * 10;
	return distance;
}

// 중앙 노드 기준 우상 진행인 경우 이웃.
// ㅋ에서 시작되어 ㅍ이 대상노드이며 ㅃ이 이웃노드가 된다.

// 우상 진행 ↗
// ㅃ ㅃ ㅃ
// ㅁ ㅍ ㅃ
// ㅋ ㅁ ㅃ

// 우하 진행 ↘
// ㅋ ㅁ ㅃ
// ㅁ ㅍ ㅃ
// ㅃ ㅃ ㅃ

// 좌하 진행 ↙
// ㅃ ㅁ ㅋ
// ㅃ ㅍ ㅁ
// ㅃ ㅃ ㅃ

// 좌상 진행 ↖
// ㅃ ㅃ ㅃ
// ㅃ ㅍ ㅁ
// ㅃ ㅁ ㅋ

NODE* Jump(int nx, int ny, int px, int py); // 점프하기 - 점프노드는? = 불필요한 노드를 구하고 있다. 왜?
int* _jump(int x, int y, int px, int py);

bool IsWalkable(int x, int y);
bool CheckEndNode(int x, int y);
bool CheckStartNode(int x, int y);
void ClearSearchedPath();

// IsWalkable 를 메크로 함수로 바꿔야 한다 (속도의 문제)
// 재귀함수 -> 반복문. 
// 방향별로 함수가 나온다.
// 클래스로 바꾸면 좋고...

// 고정 타일 (벽)
// 동적 타일 (이동 포인트)

// 맵 의 전체 정보 (속성정보)의 사본을 가지고 있게 하자?

// pathfind (..., ..., struct *point); 형태로 구조체 배열에 경로를 담아서 리턴.
// 직선 그리기..

void backtrace(NODE* endNode)
{
	// 일단은 도착지 -> 출발지로 선을 긋는다.
	while (endNode->parent != NULL)
	{
		PathList.push_back(endNode);
		endNode = endNode->parent;
	}
}

void makepath(NODE* startNode, NODE* endNode)
{
	endNode->parent = startNode;
	startNode->parent = startNode;

	PathList.push_back(endNode);
	PathList.push_back(startNode);
}

void paintcheckNode(int x, int y)
{
	// 음수가 오는 경우에는 0 처리하여 그리드상 맨 윗줄에 안칠해지는 버그 수정
	if (x < 0)
		x = 0;

	if (y < 0)
		y = 0;

	if (g_Map[y / CELL_SIZE][x / CELL_SIZE].IsWall == false)
	{
		if (!CheckEndNode(x, y))
			g_Map[y / CELL_SIZE][x / CELL_SIZE].checked = true;
	}
}

bool OptimizeFlag = false;
bool DrawTileFlag = false;
void GetLine(int startX, int startY, int endX, int endY);
void GetTile(int startX, int startY, int endX, int endY);

bool CheckWallExist(vector<NODE> nodes)
{
	// 포함된 노드들중 벽이 하나라도 있으면 true;
	// 없으면 false
	for (auto node : nodes)
	{
		if (g_Map[node.posY][node.posX].IsWall)
			return true;
	}

	return false;
}

bool bresenham(int _startX, int _startY, int _FinishX, int _FinishY, HDC MemDC)
{
	// H/W는(기울기)0과 1사이로 가정
	// 처음 찍을 점은 시작점으로 한다.

	int x = _startX;
	int y = _startY;

	// W와 H를 구해줌.

	int W = _FinishX - _startX;
	int H = _FinishY - _startY;

	// 초기값
	int F = 2 * H - W;

	// 각 판별식공식
	int dF1 = 2 * H;
	int dF2 = 2 * (H - W);

	for (x = _startX; x <= _FinishX; ++x)
	{
		// 첫 시작점 그리기
		SetPixel(MemDC, x, y, COLOR_BROWN);
		//Rectangle(MemDC, _startX*32, _startY*32, _startX*32 + CELL_SIZE, _startY*32 + CELL_SIZE);

		// 중단점이 0보다 작으면 그에 맞는 공식으로 판별식 갱신, y값은 그대로
		if (F < 0)
		{
			F += dF1;
		}
		else          // 중단점이 0보다 크거나 같으면 그에 맞는 공식으로 판별식 갱신, y값은 증가
		{
			++y;

			F += dF2;
		}
	}

	return true;
}

void drawline(int sx, int sy, int dx, int dy, HDC MemDC)
{
	// 브레즌햄 알고리즘을 이용하여 두 점간의 직선을 긋는다.

	int nx = 0;
	int ny = 0;

	int deltax = 0;
	int deltay = 0;

	int distance = 0;
	int ierror = 0;
	int icount = 0;
	int icount2 = 0;
	int ipitch = 0;

	deltax = dx - sx;
	deltay = dy - sy;

	// 직선의 방향에 따라 선이 나아갈 좌표를 정함
	nx = (deltax > 0) ? 1 : ((deltax == 0) ? 0 : -1);
	ny = (deltay > 0) ? 1 : ((deltay == 0) ? 0 : -1);

	deltax = abs(deltax);
	deltay = abs(deltay);

	// x축이 더 긴 경우 
	if (deltax > deltay)
	{
		distance = deltax;

		for (icount = distance; icount; icount--)
		{
			ierror += deltay;

			// 중간점 처리를 어떻게 해야되나...
			// 거리의 절반일때 마다 y를 1씩 올려 조금 더 부드럽게 나오도록시도.
			if (ierror >= distance / 2)
			{
				ierror -= deltax;
				sy += ny;
			}

			sx += nx;

			// 픽셀찍어보기
			//SetPixel(MemDC, sx, sy, COLOR_BROWN);
			// 박스 그려보기
			Rectangle(MemDC, sx * 32, sy * 32, sx * 32 + CELL_SIZE, sy * 32 + CELL_SIZE);
		}
	}
	else // y축이 더 긴 경우
	{
		distance = deltay;

		for (icount = distance; icount; icount--)
		{
			ierror += deltax;

			// 거리의 절반일때 마다 x를 1씩 올려 조금 더 부드럽게 나오도록시도.
			if (ierror >= distance / 2)
			{
				ierror -= deltay;
				sx += nx;
			}

			sy += ny;

			// 픽셀찍어보기
			//SetPixel(MemDC, sx, sy, COLOR_BROWN);
			// 박스 그려보기
			Rectangle(MemDC, sx * 32, sy * 32, sx * 32 + CELL_SIZE, sy * 32 + CELL_SIZE);
		}
	}
}

vector<NODE> GetTiles(int sx, int sy, int dx, int dy)
{
	// 브레즌햄 알고리즘을 이용하여 두 점간의 직선을 긋는다. 그리고 직선상의 타일을 구한다.
	vector<NODE>TilesArr;

	int nx = 0;
	int ny = 0;

	int deltax = 0;
	int deltay = 0;

	int distance = 0;
	int ierror = 0;
	int icount = 0;

	deltax = dx - sx;
	deltay = dy - sy;

	// 직선의 방향에 따라 선이 나아갈 좌표를 정함
	nx = (deltax > 0) ? 1 : ((deltax == 0) ? 0 : -1);
	ny = (deltay > 0) ? 1 : ((deltay == 0) ? 0 : -1);

	deltax = abs(deltax);
	deltay = abs(deltay);

	// x축이 더 긴 경우 
	if (deltax > deltay)
	{
		distance = deltax;

		for (icount = distance; icount; icount--)
		{
			ierror += deltay;

			// 중간점 처리를 어떻게 해야되나...
			// 시도 1 : 거리의 절반일때 마다 y를 1씩 올려 조금 더 부드럽게 나오도록시도.
			if (ierror >= distance / 2)
			{
				ierror -= deltax;
				sy += ny;
			}

			sx += nx;

			// 박스 그려보기
			//Rectangle(MemDC, sx * 32, sy * 32, sx * 32 + CELL_SIZE, sy * 32 + CELL_SIZE);
			NODE node(sy, sx);
			TilesArr.push_back(node);
		}

		// 다 넣었으면 좌표들이 담긴 직선정보를 리턴하자.
		return TilesArr;
	}
	else // y축이 더 긴 경우
	{
		distance = deltay;

		for (icount = distance; icount; icount--)
		{
			ierror += deltax;

			// 거리의 절반일때 마다 x를 1씩 올려 조금 더 부드럽게 나오도록시도.
			if (ierror >= distance / 2)
			{
				ierror -= deltay;
				sx += nx;
			}

			sy += ny;

			// 박스 그려보기
			//Rectangle(MemDC, sx * 32, sy * 32, sx * 32 + CELL_SIZE, sy * 32 + CELL_SIZE);
			NODE node(sy, sx);
			TilesArr.push_back(node);
		}

		return TilesArr;
	}
}

void optimizeLine_Inner(NODE* startNode, NODE* endNode, HDC MemDC)
{
	// EndNode가 null 이라는것은 끝까지 도착했다는 이야기
	if (endNode == NULL || startNode == NULL)
	{
		return;
	}

	// tempcode : endNode의 부모가 null 이란건 endNode가 startNode라는것이다. 시작점까지 왔으니 걍 땡
	if (endNode->parent == NULL)
	{
		//OptimizedList.pop_front();
		return;
	}

	// 시작할때 시작노드와 엔드노드를 넣어준다. (존재하지 않을때만)
	if (!FindNodeOnList(OptimizedList, *startNode))
	{
		OptimizedList.push_back(startNode);
	}

	if (!FindNodeOnList(OptimizedList, *endNode))
	{
		OptimizedList.push_back(endNode);
	}

	vector<NODE>Tiles = GetTiles(startNode->posX / CELL_SIZE, startNode->posY / CELL_SIZE,
		endNode->posX / CELL_SIZE, endNode->posY / CELL_SIZE); // 지정한 두 점간의 직선(타일좌표들을 구함)

	// 벽이있는지 없는지 체크
	if (CheckWallExist(Tiles))
	{
		// 벽이 있음
		// 여기 온 시점에서는 
		if (FindNodeOnList(OptimizedList, *startNode))
		{
			//OptimizedList.push_back(endNode);

			// 가설 : tempcode: - 마지막으로 체크하는 노드 종점이 스타트 노드라면 그 시점에 비교대상 스타트 노드는
			// 지우지 않는다.
			//if (!CheckStartNode(endNode->posX, endNode->posY))
			{
				// 가설 : tempcode
				//if (CheckEndNode(startNode->posX, startNode->posY))
				{
					OptimizedList.erase(find(OptimizedList.begin(), OptimizedList.end(), startNode));
				}
			}
		}

		optimizeLine_Inner(startNode->parent, endNode, MemDC);
	}
	else
	{
		// 벽이 없음
		// 여기 온 시점에서는 
		if (FindNodeOnList(OptimizedList, *endNode))
		{
			//OptimizedList.push_back(endNode);
			//if (!CheckEndNode(startNode->posX, startNode->posY))
			{
				//if (CheckStartNode(endNode->posX, endNode->posY))
				// tempcode : 
				//if (!CheckStartNode(endNode->parent->posX, endNode->parent->posY))
				{
					OptimizedList.erase(find(OptimizedList.begin(), OptimizedList.end(), endNode));
				}
			}
		}

		// 현 시작점과
		optimizeLine_Inner(startNode, endNode->parent, MemDC);
	}
}

// optimizeLine_Inner 함수의 버그수정 함수
void optimizeLine_Inner2(NODE* startNode, NODE* endNode, NODE* removeNode)
{
	// EndNode가 null 이라는것은 끝까지 도착했다는 이야기
	if (endNode == NULL || startNode == NULL)
	{
		return;
	}

	// 시작할때 시작노드와 엔드노드를 넣어준다. (존재하지 않을때만)
	if (!FindNodeOnList(OptimizedList, *endNode))
	{
		OptimizedList.push_back(endNode);
	}

	if (!FindNodeOnList(OptimizedList, *startNode))
	{
		OptimizedList.push_back(startNode);
	}

	// FIXME : 두 좌표를 뒤집어서 타일 탐색을 하면 탐지되는 타일이 달라짐. (위의 주석)
	// 지정한 두 점간의 직선(타일좌표들을 구함)
	//vector<NODE>Tiles = GetTiles(startNode->posX / CELL_SIZE, startNode->posY / CELL_SIZE, endNode->posX / CELL_SIZE, endNode->posY / CELL_SIZE);
	vector<NODE>Tiles = GetTiles(endNode->posX / CELL_SIZE, endNode->posY / CELL_SIZE, startNode->posX / CELL_SIZE, startNode->posY / CELL_SIZE);

	// 벽이있는지 없는지 체크
	if (CheckWallExist(Tiles))
	{
		// 벽이 있음

		// 지울노드는 지워주기. (최적화 노드에서 빠짐)
		if (FindNodeOnList(OptimizedList, *removeNode))
		{
			OptimizedList.erase(find(OptimizedList.begin(), OptimizedList.end(), removeNode));
		}

		// 재귀 함수호출
		// 장애물이 중간에 걸친경우에는 
		// 시작점을 현재 시작점의 부모로 땡긴다.
		// 도착점은 유지한다.
		// 다음 버릴 노드는 이제 갱신된 시작점 노드다.(아주 중요)(그림 그려가며 이해하기)
		optimizeLine_Inner2(startNode->parent, endNode, startNode->parent);
	}
	else
	{
		// 벽이 없음

		// 지울노드는 지워주기. (최적화 노드에서 빠짐)
		if (FindNodeOnList(OptimizedList, *removeNode))
		{
			OptimizedList.erase(find(OptimizedList.begin(), OptimizedList.end(), removeNode));
		}

		// 재귀 함수호출
		// 장애물이 중간에 없는경우에는 
		// 시작점을 유지한다.
		// 도착점은 현재 도착노드의 다음노드를 가리키게 하여 재귀를 태운다.
		// 버릴 노드는 현재의 엔드노드이다.(아주 중요)(그림 그려가며 이해하기) - 다음 재귀를 태우면 버려지게 된다.
		optimizeLine_Inner2(startNode, endNode->parent, endNode);
	}
}

// 라인 최적화 함수
void optimizeLine()
{
	// 초기값을 넣는다는 기분.
	// 재귀함수의 호출 - 
	// 시작점이 되는 노드 (현재는 EndNode가 최적화 경로의 시작점이다)
	// 종료점이 되는 노드 (시작점 바로 다음 노드를 최적화 경로 탐색의 다음노드로 한다)
	optimizeLine_Inner2(PathNodes[0], PathNodes[0]->parent, PathNodes[0]->parent);

	// 왜 이런 루프를 태웠는가?
	// 재귀함수를 호출하며 OptimizedList를 넣고 빼고 하여 나온 결과가 제대로 정렬되어있지 않다. ex) : (끝점, 중간3, 중간2, 시작, 중간1). 이런식으로 들어갔음.
	// 그래서 정렬이 맞지 않는 시작점 노드를 빼고 리스트에 있는 노드들을 백터에 정렬하여 담는다.
	// 정렬되지 않은 시작점 노드는 루프 탈출 후 푸쉬하여 강제 정렬시킴.
	// 좋은 코드가 아니므로 다시 볼 필요가 있다.
	for (auto node : OptimizedList)
	{
		if (CheckStartNode(node->posX, node->posY))
		{
			continue;
		}

		Optimizes.push_back(node);
	}

	// 꼼수 - 시작노드를 마지막에 넣어 정렬시킴
	Optimizes.push_back(StartNode);
}