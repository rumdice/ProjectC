#pragma once

#include "resource.h"

#define MAX_LOADSTRING 100

HINSTANCE hInst;                               
WCHAR szTitle[MAX_LOADSTRING];                 
WCHAR szWindowClass[MAX_LOADSTRING];           

ATOM                MyRegisterClass(HINSTANCE hInstance);
BOOL                InitInstance(HINSTANCE, int);
LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);

POINT					m_ptMousePos;					


#define COLOR_WALL				RGB(51, 51, 51)			
#define COLOR_START_POINT		RGB(0, 255, 0)			
#define COLOR_END_POINT			RGB(255, 0, 0)			
#define COLOR_OPENLIST			RGB(0, 0, 255)			
#define COLOR_CLOSELIST			RGB(255, 255, 0)		
#define COLOR_PATHLIST			RGB(255, 0, 255)		
#define COLOR_GRID_LINE			RGB(204, 204, 204)		

#define CELL_SIZE				32				
#define WINDOW_WIDTH			1440			
#define WINDOW_HIGHT			900				
#define MAX_BLOCK_COUNT_WIDTH	44				
#define MAX_BLOCK_COUNT_HIGHT	26				

struct NODE
{
	int posX;
	int posY;

	NODE *parent;
	int H;
	int G;
	int F;

	bool IsWall;
	bool IsStartPoint;
	bool IsEndPoint;

	NODE()
	{

	}

	NODE(int y, int x)
	{
		posY = y;
		posX = x;
	}
};

NODE *StartNode;
NODE *EndNode;

list<NODE*>OpenList;
list<NODE*>CloseList;

list<NODE*>WallList; 
list<NODE*>PathList; 

NODE g_Map[MAX_BLOCK_COUNT_HIGHT][MAX_BLOCK_COUNT_WIDTH]; 
bool IsDrawWall;
bool IsRemoveWall;

bool IsEditBlockMode;

void InitMap();
void StartFindPath(NODE* startNode, NODE *endNode);
NODE* GetSubNodes(NODE* currentNode);						
NODE* FindNodeOnList(list<NODE*>nodeList, NODE pNode);
void InitWallList();

void ClearStartNode();
void ClearEndNode();

void Reset();
void Clear();


int Heuristic(int posX, int posY, int endX, int endY)
{
	return static_cast<int>((abs(static_cast<double>(posX - endX)) + abs(static_cast<double>(posY - endY))));
}

// PathFinder랑 반대긴 한대 결국
// H 값이 낮을 수록 유두리 있는거고 H값이 높을수록 빡빡하게 나온다.
// 목적지 위주일 수록 지그재그로 나온다.
int g_weight = 1; // 1 : 빡빡하게 탐색 : 목적지 위주 // 5 : 유두리있게 탐색 (넓직하게)