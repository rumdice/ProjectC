#pragma once

#include "resource.h"
#include "Define.h"

#define MAX_LOADSTRING 100

// ���� ����:
HINSTANCE hInst;                                // ���� �ν��Ͻ��Դϴ�.
WCHAR szTitle[MAX_LOADSTRING];                  // ���� ǥ���� �ؽ�Ʈ�Դϴ�.
WCHAR szWindowClass[MAX_LOADSTRING];            // �⺻ â Ŭ���� �̸��Դϴ�.

// �� �ڵ� ��⿡ ��� �ִ� �Լ��� ������ �����Դϴ�.
ATOM                MyRegisterClass(HINSTANCE hInstance);
BOOL                InitInstance(HINSTANCE, int);
LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);


// �� ��� ������ ���̴� ������
POINT				m_ptMousePos;					// ���콺�� ��ǥ
HBITMAP				MemBit;							// ��Ʈ�� ���� (�����?)


NODE *StartNode;
NODE *EndNode;

list<NODE*>OpenList;
list<NODE*>CloseList;

list<NODE*>WallList; // �� ������ ��´�.
list<NODE*>PathList; // ����� ��� ���.

vector<NODE*>PathNodes;
list<NODE*>OptimizedList; // ����ȭ�� ��� (�� -> ����)
vector<NODE*>Optimizes;// ����ȭ ��� ���Ŀ�...

NODE g_Map[MAX_BLOCK_COUNT_HIGHT][MAX_BLOCK_COUNT_WIDTH]; // �ϴ� ĭ ������ �°� ����

bool IsDrawWall;
bool IsRemoveWall;

bool IsEditBlockMode;

// �ܼ� �����Ÿ� �Ÿ����ϱ� - ?? �޸���ƽ
// GetH
// �� ������ �ִ� �����Ÿ��� ���Ѵ�.
// ����ź ���? �� ������ ���밪�� �׳� ����?
int Heuristic(int posX, int posY, int endX, int endY)
{
	return static_cast<int>((abs(static_cast<double>(posX - endX)) + abs(static_cast<double>(posY - endY))));
}

// PathFinder�� �ݴ�� �Ѵ� �ᱹ
// H ���� ���� ���� ���θ� �ִ°Ű� H���� �������� �����ϰ� ���´�.
// ������ ������ ���� ������׷� ���´�.
int g_weight = 1; // 1 : �����ϰ� Ž�� : ������ ���� // 5 : ���θ��ְ� Ž�� (�����ϰ�)

void InitMap();
void FindPath(NODE* startNode, NODE *endNode);

int malloc_count = 0;
void FindPath2(NODE* startNode, NODE *endNode);

NODE* GetSubNodes(NODE* currentNode);						// ���� �߿��� ���� �Լ� (a* )
NODE* FindNodeOnList(list<NODE*>nodeList, NODE pNode);
void InitWallList();
void ClearStartNode();
void ClearEndNode();
void Reset();
void Clear();
void Ready(HWND hWnd);
void DrawBitMap(HDC hdc, int x, int y, HBITMAP hBit);

// ���� �����غ���
void IdentifySuccessors(NODE* currentNode); // ������ �Ǵ�. 
void _identifySuccessors(NODE* activeNode, open_list* current, NODE* endNode);

vector<NODE>FindNeighbors(NODE* pNode);	// �̿���� �˻� - (�̿� ����� ����� ���ϰ� �ִ°�?)
// �밢�� ���� ������ ��� (������ �̿�)���� 5������ ��尡 �̿��̴�.
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

// �߾� ��� ���� ��� ������ ��� �̿�.
// ������ ���۵Ǿ� ���� ������̸� ���� �̿���尡 �ȴ�.

// ��� ���� ��
// �� �� ��
// �� �� ��
// �� �� ��

// ���� ���� ��
// �� �� ��
// �� �� ��
// �� �� ��

// ���� ���� ��
// �� �� ��
// �� �� ��
// �� �� ��

// �»� ���� ��
// �� �� ��
// �� �� ��
// �� �� ��

NODE* Jump(int nx, int ny, int px, int py); // �����ϱ� - ��������? = ���ʿ��� ��带 ���ϰ� �ִ�. ��?
int* _jump(int x, int y, int px, int py);

bool IsWalkable(int x, int y);
bool CheckEndNode(int x, int y);
bool CheckStartNode(int x, int y);
void ClearSearchedPath();

// IsWalkable �� ��ũ�� �Լ��� �ٲ�� �Ѵ� (�ӵ��� ����)
// ����Լ� -> �ݺ���. 
// ���⺰�� �Լ��� ���´�.
// Ŭ������ �ٲٸ� ����...

// ���� Ÿ�� (��)
// ���� Ÿ�� (�̵� ����Ʈ)

// �� �� ��ü ���� (�Ӽ�����)�� �纻�� ������ �ְ� ����?

// pathfind (..., ..., struct *point); ���·� ����ü �迭�� ��θ� ��Ƽ� ����.
// ���� �׸���..

void backtrace(NODE* endNode)
{
	// �ϴ��� ������ -> ������� ���� �ߴ´�.
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
	// ������ ���� ��쿡�� 0 ó���Ͽ� �׸���� �� ���ٿ� ��ĥ������ ���� ����
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
	// ���Ե� ������ ���� �ϳ��� ������ true;
	// ������ false
	for (auto node : nodes)
	{
		if (g_Map[node.posY][node.posX].IsWall)
			return true;
	}

	return false;
}

bool bresenham(int _startX, int _startY, int _FinishX, int _FinishY, HDC MemDC)
{
	// H/W��(����)0�� 1���̷� ����
	// ó�� ���� ���� ���������� �Ѵ�.

	int x = _startX;
	int y = _startY;

	// W�� H�� ������.

	int W = _FinishX - _startX;
	int H = _FinishY - _startY;

	// �ʱⰪ
	int F = 2 * H - W;

	// �� �Ǻ��İ���
	int dF1 = 2 * H;
	int dF2 = 2 * (H - W);

	for (x = _startX; x <= _FinishX; ++x)
	{
		// ù ������ �׸���
		SetPixel(MemDC, x, y, COLOR_BROWN);
		//Rectangle(MemDC, _startX*32, _startY*32, _startX*32 + CELL_SIZE, _startY*32 + CELL_SIZE);

		// �ߴ����� 0���� ������ �׿� �´� �������� �Ǻ��� ����, y���� �״��
		if (F < 0)
		{
			F += dF1;
		}
		else          // �ߴ����� 0���� ũ�ų� ������ �׿� �´� �������� �Ǻ��� ����, y���� ����
		{
			++y;

			F += dF2;
		}
	}

	return true;
}

void drawline(int sx, int sy, int dx, int dy, HDC MemDC)
{
	// �극���� �˰����� �̿��Ͽ� �� ������ ������ �ߴ´�.

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

	// ������ ���⿡ ���� ���� ���ư� ��ǥ�� ����
	nx = (deltax > 0) ? 1 : ((deltax == 0) ? 0 : -1);
	ny = (deltay > 0) ? 1 : ((deltay == 0) ? 0 : -1);

	deltax = abs(deltax);
	deltay = abs(deltay);

	// x���� �� �� ��� 
	if (deltax > deltay)
	{
		distance = deltax;

		for (icount = distance; icount; icount--)
		{
			ierror += deltay;

			// �߰��� ó���� ��� �ؾߵǳ�...
			// �Ÿ��� �����϶� ���� y�� 1�� �÷� ���� �� �ε巴�� �������Ͻõ�.
			if (ierror >= distance / 2)
			{
				ierror -= deltax;
				sy += ny;
			}

			sx += nx;

			// �ȼ�����
			//SetPixel(MemDC, sx, sy, COLOR_BROWN);
			// �ڽ� �׷�����
			Rectangle(MemDC, sx * 32, sy * 32, sx * 32 + CELL_SIZE, sy * 32 + CELL_SIZE);
		}
	}
	else // y���� �� �� ���
	{
		distance = deltay;

		for (icount = distance; icount; icount--)
		{
			ierror += deltax;

			// �Ÿ��� �����϶� ���� x�� 1�� �÷� ���� �� �ε巴�� �������Ͻõ�.
			if (ierror >= distance / 2)
			{
				ierror -= deltay;
				sx += nx;
			}

			sy += ny;

			// �ȼ�����
			//SetPixel(MemDC, sx, sy, COLOR_BROWN);
			// �ڽ� �׷�����
			Rectangle(MemDC, sx * 32, sy * 32, sx * 32 + CELL_SIZE, sy * 32 + CELL_SIZE);
		}
	}
}

vector<NODE> GetTiles(int sx, int sy, int dx, int dy)
{
	// �극���� �˰����� �̿��Ͽ� �� ������ ������ �ߴ´�. �׸��� �������� Ÿ���� ���Ѵ�.
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

	// ������ ���⿡ ���� ���� ���ư� ��ǥ�� ����
	nx = (deltax > 0) ? 1 : ((deltax == 0) ? 0 : -1);
	ny = (deltay > 0) ? 1 : ((deltay == 0) ? 0 : -1);

	deltax = abs(deltax);
	deltay = abs(deltay);

	// x���� �� �� ��� 
	if (deltax > deltay)
	{
		distance = deltax;

		for (icount = distance; icount; icount--)
		{
			ierror += deltay;

			// �߰��� ó���� ��� �ؾߵǳ�...
			// �õ� 1 : �Ÿ��� �����϶� ���� y�� 1�� �÷� ���� �� �ε巴�� �������Ͻõ�.
			if (ierror >= distance / 2)
			{
				ierror -= deltax;
				sy += ny;
			}

			sx += nx;

			// �ڽ� �׷�����
			//Rectangle(MemDC, sx * 32, sy * 32, sx * 32 + CELL_SIZE, sy * 32 + CELL_SIZE);
			NODE node(sy, sx);
			TilesArr.push_back(node);
		}

		// �� �־����� ��ǥ���� ��� ���������� ��������.
		return TilesArr;
	}
	else // y���� �� �� ���
	{
		distance = deltay;

		for (icount = distance; icount; icount--)
		{
			ierror += deltax;

			// �Ÿ��� �����϶� ���� x�� 1�� �÷� ���� �� �ε巴�� �������Ͻõ�.
			if (ierror >= distance / 2)
			{
				ierror -= deltay;
				sx += nx;
			}

			sy += ny;

			// �ڽ� �׷�����
			//Rectangle(MemDC, sx * 32, sy * 32, sx * 32 + CELL_SIZE, sy * 32 + CELL_SIZE);
			NODE node(sy, sx);
			TilesArr.push_back(node);
		}

		return TilesArr;
	}
}

void optimizeLine_Inner(NODE* startNode, NODE* endNode, HDC MemDC)
{
	// EndNode�� null �̶�°��� ������ �����ߴٴ� �̾߱�
	if (endNode == NULL || startNode == NULL)
	{
		return;
	}

	// tempcode : endNode�� �θ� null �̶��� endNode�� startNode��°��̴�. ���������� ������ �� ��
	if (endNode->parent == NULL)
	{
		//OptimizedList.pop_front();
		return;
	}

	// �����Ҷ� ���۳��� �����带 �־��ش�. (�������� ��������)
	if (!FindNodeOnList(OptimizedList, *startNode))
	{
		OptimizedList.push_back(startNode);
	}

	if (!FindNodeOnList(OptimizedList, *endNode))
	{
		OptimizedList.push_back(endNode);
	}

	vector<NODE>Tiles = GetTiles(startNode->posX / CELL_SIZE, startNode->posY / CELL_SIZE,
		endNode->posX / CELL_SIZE, endNode->posY / CELL_SIZE); // ������ �� ������ ����(Ÿ����ǥ���� ����)

	// �����ִ��� ������ üũ
	if (CheckWallExist(Tiles))
	{
		// ���� ����
		// ���� �� ���������� 
		if (FindNodeOnList(OptimizedList, *startNode))
		{
			//OptimizedList.push_back(endNode);

			// ���� : tempcode: - ���������� üũ�ϴ� ��� ������ ��ŸƮ ����� �� ������ �񱳴�� ��ŸƮ ����
			// ������ �ʴ´�.
			//if (!CheckStartNode(endNode->posX, endNode->posY))
			{
				// ���� : tempcode
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
		// ���� ����
		// ���� �� ���������� 
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

		// �� ��������
		optimizeLine_Inner(startNode, endNode->parent, MemDC);
	}
}

// optimizeLine_Inner �Լ��� ���׼��� �Լ�
void optimizeLine_Inner2(NODE* startNode, NODE* endNode, NODE* removeNode)
{
	// EndNode�� null �̶�°��� ������ �����ߴٴ� �̾߱�
	if (endNode == NULL || startNode == NULL)
	{
		return;
	}

	// �����Ҷ� ���۳��� �����带 �־��ش�. (�������� ��������)
	if (!FindNodeOnList(OptimizedList, *endNode))
	{
		OptimizedList.push_back(endNode);
	}

	if (!FindNodeOnList(OptimizedList, *startNode))
	{
		OptimizedList.push_back(startNode);
	}

	// FIXME : �� ��ǥ�� ����� Ÿ�� Ž���� �ϸ� Ž���Ǵ� Ÿ���� �޶���. (���� �ּ�)
	// ������ �� ������ ����(Ÿ����ǥ���� ����)
	//vector<NODE>Tiles = GetTiles(startNode->posX / CELL_SIZE, startNode->posY / CELL_SIZE, endNode->posX / CELL_SIZE, endNode->posY / CELL_SIZE);
	vector<NODE>Tiles = GetTiles(endNode->posX / CELL_SIZE, endNode->posY / CELL_SIZE, startNode->posX / CELL_SIZE, startNode->posY / CELL_SIZE);

	// �����ִ��� ������ üũ
	if (CheckWallExist(Tiles))
	{
		// ���� ����

		// ������� �����ֱ�. (����ȭ ��忡�� ����)
		if (FindNodeOnList(OptimizedList, *removeNode))
		{
			OptimizedList.erase(find(OptimizedList.begin(), OptimizedList.end(), removeNode));
		}

		// ��� �Լ�ȣ��
		// ��ֹ��� �߰��� ��ģ��쿡�� 
		// �������� ���� �������� �θ�� �����.
		// �������� �����Ѵ�.
		// ���� ���� ���� ���� ���ŵ� ������ ����.(���� �߿�)(�׸� �׷����� �����ϱ�)
		optimizeLine_Inner2(startNode->parent, endNode, startNode->parent);
	}
	else
	{
		// ���� ����

		// ������� �����ֱ�. (����ȭ ��忡�� ����)
		if (FindNodeOnList(OptimizedList, *removeNode))
		{
			OptimizedList.erase(find(OptimizedList.begin(), OptimizedList.end(), removeNode));
		}

		// ��� �Լ�ȣ��
		// ��ֹ��� �߰��� ���°�쿡�� 
		// �������� �����Ѵ�.
		// �������� ���� ��������� ������带 ����Ű�� �Ͽ� ��͸� �¿��.
		// ���� ���� ������ �������̴�.(���� �߿�)(�׸� �׷����� �����ϱ�) - ���� ��͸� �¿�� �������� �ȴ�.
		optimizeLine_Inner2(startNode, endNode->parent, endNode);
	}
}

// ���� ����ȭ �Լ�
void optimizeLine()
{
	// �ʱⰪ�� �ִ´ٴ� ���.
	// ����Լ��� ȣ�� - 
	// �������� �Ǵ� ��� (����� EndNode�� ����ȭ ����� �������̴�)
	// �������� �Ǵ� ��� (������ �ٷ� ���� ��带 ����ȭ ��� Ž���� �������� �Ѵ�)
	optimizeLine_Inner2(PathNodes[0], PathNodes[0]->parent, PathNodes[0]->parent);

	// �� �̷� ������ �¿��°�?
	// ����Լ��� ȣ���ϸ� OptimizedList�� �ְ� ���� �Ͽ� ���� ����� ����� ���ĵǾ����� �ʴ�. ex) : (����, �߰�3, �߰�2, ����, �߰�1). �̷������� ����.
	// �׷��� ������ ���� �ʴ� ������ ��带 ���� ����Ʈ�� �ִ� ������ ���Ϳ� �����Ͽ� ��´�.
	// ���ĵ��� ���� ������ ���� ���� Ż�� �� Ǫ���Ͽ� ���� ���Ľ�Ŵ.
	// ���� �ڵ尡 �ƴϹǷ� �ٽ� �� �ʿ䰡 �ִ�.
	for (auto node : OptimizedList)
	{
		if (CheckStartNode(node->posX, node->posY))
		{
			continue;
		}

		Optimizes.push_back(node);
	}

	// �ļ� - ���۳�带 �������� �־� ���Ľ�Ŵ
	Optimizes.push_back(StartNode);
}