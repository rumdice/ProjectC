// Lecture36AStarAlgorithmGDI.cpp: 응용 프로그램의 진입점을 정의합니다.
//

#include "stdafx.h"
#include "Lecture36AStarAlgorithmGDI.h"

int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
	_In_opt_ HINSTANCE hPrevInstance,
	_In_ LPWSTR    lpCmdLine,
	_In_ int       nCmdShow)
{
	UNREFERENCED_PARAMETER(hPrevInstance);
	UNREFERENCED_PARAMETER(lpCmdLine);

	LoadStringW(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
	LoadStringW(hInstance, IDC_LECTURE36ASTARALGORITHMGDI, szWindowClass, MAX_LOADSTRING);
	MyRegisterClass(hInstance);

	if (!InitInstance(hInstance, nCmdShow))
	{
		return FALSE;
	}

	MSG msg;
	while (GetMessage(&msg, nullptr, 0, 0))
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}

	return (int)msg.wParam;
}

ATOM MyRegisterClass(HINSTANCE hInstance)
{
	WNDCLASSEXW wcex;

	wcex.cbSize = sizeof(WNDCLASSEX);

	wcex.style = CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc = WndProc;
	wcex.cbClsExtra = 0;
	wcex.cbWndExtra = 0;
	wcex.hInstance = hInstance;
	wcex.hIcon = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_LECTURE36ASTARALGORITHMGDI));
	wcex.hCursor = LoadCursor(nullptr, IDC_ARROW);
	wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
	wcex.lpszMenuName = MAKEINTRESOURCEW(IDC_LECTURE36ASTARALGORITHMGDI);
	wcex.lpszClassName = szWindowClass;
	wcex.hIconSm = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

	return RegisterClassExW(&wcex);
}

BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
	hInst = hInstance;

	HWND hWnd = CreateWindowW(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT, 0, WINDOW_WIDTH, WINDOW_HIGHT, nullptr, nullptr, hInstance, nullptr);

	if (!hWnd)
	{
		return FALSE;
	}

	ShowWindow(hWnd, nCmdShow);
	UpdateWindow(hWnd);

	return TRUE;
}

HBITMAP MemBit;
void Ready(HWND hWnd)
{
	RECT winRect;
	GetClientRect(hWnd, &winRect);

	HBITMAP hOldBmp;
	HDC hdc;
	HDC MemDC;
	hdc = GetDC(hWnd);
	HBRUSH MyBrush, OldBlush;
	HPEN MyPen, OldPen;

	if (MemBit)
		DeleteObject(MemBit);

	MemBit = CreateCompatibleBitmap(hdc, winRect.right, winRect.bottom);
	MemDC = CreateCompatibleDC(hdc);
	hOldBmp = (HBITMAP)SelectObject(MemDC, MemBit);

	PatBlt(MemDC, 0, 0, winRect.right, winRect.bottom, WHITENESS);

	MyPen = CreatePen(PS_SOLID, 1, COLOR_GRID_LINE);
	OldPen = (HPEN)SelectObject(MemDC, MyPen);

	// 격자를 그린다.
	// 32 * 32 사이즈로 그려짐
	for (int y = 0; y < winRect.bottom; y = y + CELL_SIZE)
	{
		MoveToEx(MemDC, 0, y, NULL);
		LineTo(MemDC, winRect.right, y);
	}
	for (int x = 0; x < winRect.right; x = x + CELL_SIZE)
	{
		MoveToEx(MemDC, x, 0, NULL);
		LineTo(MemDC, x, winRect.bottom);
	}

	SelectPen(MemDC, OldPen);
	DeletePen(MyPen);

	for (int y = 0; y < MAX_BLOCK_COUNT_HIGHT; y++)
	{
		for (int x = 0; x < MAX_BLOCK_COUNT_WIDTH; x++)
		{
			if (g_Map[y][x].IsWall == true)
			{
				MyBrush = CreateSolidBrush(COLOR_WALL);
				OldBlush = (HBRUSH)SelectObject(MemDC, MyBrush);

				int iLeft = g_Map[y][x].posX;
				int iTop = g_Map[y][x].posY;

				Rectangle(MemDC, iLeft, iTop, iLeft + CELL_SIZE, iTop + CELL_SIZE);
				SelectObject(MemDC, OldBlush);
				DeleteBrush(MyBrush);
			}
			else if (g_Map[y][x].IsStartPoint)
			{
				MyBrush = CreateSolidBrush(COLOR_START_POINT);
				OldBlush = (HBRUSH)SelectObject(MemDC, MyBrush);

				int iLeft = g_Map[y][x].posX;
				int iTop = g_Map[y][x].posY;

				Rectangle(MemDC, iLeft, iTop, iLeft + CELL_SIZE, iTop + CELL_SIZE);
				SelectObject(MemDC, OldBlush);
				DeleteBrush(MyBrush);
			}
			else if (g_Map[y][x].IsEndPoint)
			{
				MyBrush = CreateSolidBrush(COLOR_END_POINT);
				OldBlush = (HBRUSH)SelectObject(MemDC, MyBrush);

				int iLeft = g_Map[y][x].posX;
				int iTop = g_Map[y][x].posY;

				Rectangle(MemDC, iLeft, iTop, iLeft + CELL_SIZE, iTop + CELL_SIZE);
				SelectObject(MemDC, OldBlush);
				DeleteBrush(MyBrush);
			}
		}
	}

	for (auto node : OpenList)
	{
		MyBrush = CreateSolidBrush(COLOR_OPENLIST);
		OldBlush = (HBRUSH)SelectObject(MemDC, MyBrush);

		int iLeft = node->posX;
		int iTop = node->posY;

		Rectangle(MemDC, iLeft, iTop, iLeft + CELL_SIZE, iTop + CELL_SIZE);
		SelectObject(MemDC, OldBlush);
		DeleteBrush(MyBrush);
	}

	for (auto node : CloseList)
	{
		MyBrush = CreateSolidBrush(COLOR_CLOSELIST);
		OldBlush = (HBRUSH)SelectObject(MemDC, MyBrush);

		int iLeft = node->posX;
		int iTop = node->posY;

		Rectangle(MemDC, iLeft, iTop, iLeft + CELL_SIZE, iTop + CELL_SIZE);
		SelectObject(MemDC, OldBlush);
		DeleteBrush(MyBrush);
	}

	// 결과로 나온 Path 경로 색칠
	for (auto node : PathList)
	{
		MyPen = CreatePen(PS_SOLID, 2, COLOR_PATHLIST);
		OldPen = (HPEN)SelectObject(MemDC, MyPen);

		int iLeft = node->posX + (CELL_SIZE / 2);
		int iTop = node->posY + (CELL_SIZE / 2);

		int iLeft2 = node->parent->posX + (CELL_SIZE / 2);
		int iTop2 = node->parent->posY + (CELL_SIZE / 2);

		MoveToEx(MemDC, iLeft, iTop, NULL);
		LineTo(MemDC, iLeft2, iTop2);

		SelectPen(MemDC, OldPen);
		DeletePen(MyPen);
	}

	SelectObject(MemDC, hOldBmp);
	DeleteDC(MemDC);
	ReleaseDC(hWnd, hdc);
}

void DrawBitMap(HDC hdc, int x, int y, HBITMAP hBit)
{
	HDC MemDC;
	HBITMAP OldBitmap;
	int bx, by;
	BITMAP bit;

	MemDC = CreateCompatibleDC(hdc);
	OldBitmap = (HBITMAP)SelectObject(MemDC, hBit);

	GetObject(hBit, sizeof(BITMAP), &bit);
	bx = bit.bmWidth;
	by = bit.bmHeight;

	BitBlt(hdc, x, y, bx, by, MemDC, 0, 0, SRCCOPY);

	SelectObject(MemDC, OldBitmap);
	DeleteDC(MemDC);
}

void InitMap()
{
	for (int y = 0; y < MAX_BLOCK_COUNT_HIGHT; y++)
	{
		for (int x = 0; x < MAX_BLOCK_COUNT_WIDTH; x++)
		{
			g_Map[y][x].posX = x * CELL_SIZE;
			g_Map[y][x].posY = y * CELL_SIZE;
			g_Map[y][x].IsWall = false;
		}
	}

	g_Map[12][12].IsStartPoint = true;
	StartNode = &g_Map[12][12];

	g_Map[12][36].IsEndPoint = true;
	EndNode = &g_Map[12][36];
}

void StartFindPath(NODE* startNode, NODE *endNode)
{
	Clear();

	InitWallList();

	OpenList.push_back(startNode);

	NODE *current = nullptr; 
	while (!OpenList.empty())
	{
		// F값 기준으로 정렬이 필요함 - (우선순위가 높은 녀석으로 다음 노드를 잡아야 하기 때문)
		current = *OpenList.begin();
		for (auto node : OpenList)
		{
			if (node->F <= current->F)
			{
				current = node;
			}
		}

		// 현재 노드 위치가 EndPos(목적지)라면 종료
		// 매 루프마다 비교함수 콜을 때리느니 그냥 단순 비교...(속도)
		if (current->posX == endNode->posX && current->posY == endNode->posY)
		{
			OpenList.erase(find(OpenList.begin(), OpenList.end(), current));
			CloseList.erase(find(CloseList.begin(), CloseList.end(), StartNode));

			break;
		}

		// 현재 노드는 클로즈 리스트에 넣는다.
		CloseList.push_back(current);

		// 오픈 리스트에서 현재노드는 제거한다.
		OpenList.erase(find(OpenList.begin(), OpenList.end(), current));

		// 8 방향의 서브노드를 구한다.
		NODE* subNodes = GetSubNodes(current);
	}

	// Path 경로를 저장한다.
	while (current != nullptr)
	{
		PathList.push_back(current);
		current = current->parent;
	}

	// 스타트 노드와 pathList에서 제거 (표기문제)
	PathList.erase(find(PathList.begin(), PathList.end(), startNode));
}

NODE* FindNodeOnList(list<NODE*>nodeList, NODE pNode)
{
	for (auto node : nodeList)
	{
		if (node->posX == pNode.posX && node->posY == pNode.posY)
		{
			return node;
		}
	}
	return nullptr;
}

NODE* GetSubNodes(NODE* currentNode)
{
	// 8개 노드의 좌표 생성
	// 중점 노드를 기반으로 x,y 좌표를 +- 하여 8방향의 노드를 알아낸다.
	int posArrX[8] = {
		currentNode->posX,				// 상
		currentNode->posX - CELL_SIZE,	// 좌
		currentNode->posX + CELL_SIZE,	// 우
		currentNode->posX,				// 하

		currentNode->posX - CELL_SIZE,	// 좌상
		currentNode->posX + CELL_SIZE,	// 우상
		currentNode->posX - CELL_SIZE,	// 좌하
		currentNode->posX + CELL_SIZE,	// 우하
	};

	int posArrY[8] = {
		currentNode->posY - CELL_SIZE,	// 상
		currentNode->posY,				// 좌
		currentNode->posY,				// 우
		currentNode->posY + CELL_SIZE,	// 하

		currentNode->posY - CELL_SIZE,	// 좌상
		currentNode->posY - CELL_SIZE,	// 우상
		currentNode->posY + CELL_SIZE,	// 좌하
		currentNode->posY + CELL_SIZE,	// 우하
	};

	// 8개의 서브노드들 정보(좌표)를 돌면서 currentNode에 정보 저장.
	for (int i = 0; i < 8; i++)
	{
		NODE newNode(posArrY[i], posArrX[i]);

		if (FindNodeOnList(CloseList, newNode))
		{
			continue;
		}

		if (FindNodeOnList(WallList, newNode))
		{
			continue;
		}

		// 대각선 노드일 경우 가중치를 준다 (비용)
		int totalCost = currentNode->G + ((i < 4) ? 10 : 14);

		// 찾은 서브 노드가 이미 오픈리스트에 담겨있던 놈인가?
		NODE *successor = FindNodeOnList(OpenList, newNode);

		// 없는 놈인경우 완전 새로운 서브노드 생성
		if (successor == nullptr)
		{
			successor = new NODE(newNode.posY, newNode.posX);
			successor->parent = currentNode; // 맞는지 검증 필요

			successor->G = totalCost;
			successor->H = (int)Heuristic(successor->posX, successor->posY, EndNode->posX, EndNode->posY) / g_weight;
			successor->F = successor->G + successor->H;

			// 새로운 자식노드를 OpenList에 추가
			OpenList.push_back(successor);
		}
		else if (totalCost < successor->G)
		{
			successor->parent = currentNode;
			successor->G = totalCost;
		}
	}

	return NULL;
}

void InitWallList()
{
	for (int y = 0; y < MAX_BLOCK_COUNT_HIGHT; y++)
	{
		for (int x = 0; x < MAX_BLOCK_COUNT_WIDTH; x++)
		{
			if (g_Map[y][x].IsWall == true)
			{
				NODE *node = &g_Map[y][x];
				WallList.push_back(node);
			}
		}
	}
}

void ClearWallList()
{
	for (int y = 0; y < MAX_BLOCK_COUNT_HIGHT; y++)
	{
		for (int x = 0; x < MAX_BLOCK_COUNT_WIDTH; x++)
		{
			if (g_Map[y][x].IsWall == true)
			{
				g_Map[y][x].IsWall = false;
			}
		}
	}

	WallList.clear();
}

void ClearStartNode()
{
	for (int y = 0; y < MAX_BLOCK_COUNT_HIGHT; y++)
	{
		for (int x = 0; x < MAX_BLOCK_COUNT_WIDTH; x++)
		{
			if (g_Map[y][x].IsStartPoint)
			{
				g_Map[y][x].IsStartPoint = false;
			}
		}
	}
}

void ClearEndNode()
{
	for (int y = 0; y < MAX_BLOCK_COUNT_HIGHT; y++)
	{
		for (int x = 0; x < MAX_BLOCK_COUNT_WIDTH; x++)
		{
			if (g_Map[y][x].IsEndPoint)
			{
				g_Map[y][x].IsEndPoint = false;
			}
		}
	}
}

void Reset()
{
	ClearStartNode();
	ClearEndNode();

	// Start 지점 초기값 설정
	g_Map[12][12].IsStartPoint = true;
	StartNode = &g_Map[12][12];

	// End 지점 초기값 설정
	g_Map[12][36].IsEndPoint = true;
	EndNode = &g_Map[12][36];

	// 벽 삭제
	ClearWallList();

	// 오픈 리스트 삭제
	OpenList.clear();

	// 클로즈 리스트 삭제
	CloseList.clear();

	// 패쓰 리트트 삭제
	PathList.clear();
}

void Clear()
{
	OpenList.clear();
	CloseList.clear();
	PathList.clear();
}

LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	PAINTSTRUCT ps;
	HDC hdc;
	HMENU hMenu = GetMenu(hWnd);

	switch (message)
	{
	case WM_COMMAND:
	{
		int wmId = LOWORD(wParam);

		switch (wmId)
		{
		case IDM_EXIT:
			DestroyWindow(hWnd);
			break;
		case IDM_START_SEARCH:
			StartFindPath(StartNode, EndNode);
			break;
		case IDM_EDIT_BLOCK:
			if (IsEditBlockMode)
			{
				CheckMenuItem(hMenu, IDM_EDIT_BLOCK, MF_UNCHECKED);
				IsEditBlockMode = false;
			}
			else
			{
				CheckMenuItem(hMenu, IDM_EDIT_BLOCK, MF_CHECKED);
				IsEditBlockMode = true;
			}
			break;
		case IDM_WEIGHT_1:
			CheckMenuItem(hMenu, IDM_WEIGHT_5, MF_UNCHECKED);
			CheckMenuItem(hMenu, IDM_WEIGHT_1, MF_CHECKED);
			g_weight = 1;
			break;
		case IDM_WEIGHT_5:
			CheckMenuItem(hMenu, IDM_WEIGHT_1, MF_UNCHECKED);
			CheckMenuItem(hMenu, IDM_WEIGHT_5, MF_CHECKED);
			g_weight = 5;
			break;
		case IDM_RESET:
			Reset();
			break;
		case IDM_CLEAR:
			Clear();
			break;
		default:
			return DefWindowProc(hWnd, message, wParam, lParam);
		}
	}
	break;
	case WM_CREATE:
	{
		InitMap();
	}
	break;
	case WM_PAINT:
	{
		hdc = BeginPaint(hWnd, &ps);
		DrawBitMap(hdc, 0, 0, MemBit);
		EndPaint(hWnd, &ps);

		Ready(hWnd);
		InvalidateRect(hWnd, NULL, FALSE);
	}
	break;
	case WM_LBUTTONDOWN:
	{
		m_ptMousePos.x = LOWORD(lParam);
		m_ptMousePos.y = HIWORD(lParam);

		if (m_ptMousePos.x < WINDOW_WIDTH && m_ptMousePos.y < WINDOW_HIGHT)
		{
			// 블록 편집 모드일때만 허용
			if (IsEditBlockMode)
			{
				IsDrawWall = true;

				int iLeft = m_ptMousePos.x / CELL_SIZE;
				int iTop = m_ptMousePos.y / CELL_SIZE;

				if (g_Map[iTop][iLeft].IsStartPoint || g_Map[iTop][iLeft].IsEndPoint)
				{
					g_Map[iTop][iLeft].IsWall = false;
				}
				else
				{
					g_Map[iTop][iLeft].IsWall = true;
				}
			}
			else
			{
				ClearStartNode();

				int iLeft = m_ptMousePos.x / CELL_SIZE;
				int iTop = m_ptMousePos.y / CELL_SIZE;

				g_Map[iTop][iLeft].IsStartPoint = true;
				StartNode = &g_Map[iTop][iLeft];
			}
		}
	}
	break;
	case WM_MOUSEMOVE:
	{
		m_ptMousePos.x = LOWORD(lParam);
		m_ptMousePos.y = HIWORD(lParam);

		if (m_ptMousePos.x < WINDOW_WIDTH && m_ptMousePos.y < WINDOW_HIGHT)
		{
			if (IsEditBlockMode)
			{
				int iLeft = m_ptMousePos.x / CELL_SIZE;
				int iTop = m_ptMousePos.y / CELL_SIZE;

				if (IsDrawWall)
				{
					if (g_Map[iTop][iLeft].IsStartPoint || g_Map[iTop][iLeft].IsEndPoint)
					{
						g_Map[iTop][iLeft].IsWall = false;
					}
					else
					{
						g_Map[iTop][iLeft].IsWall = true;
					}
				}

				if (IsRemoveWall)
				{
					if (g_Map[iTop][iLeft].IsWall)
					{
						g_Map[iTop][iLeft].IsWall = false;
					}
				}
			}
		}
	}
	break;
	case WM_LBUTTONUP:
	{
		if (IsEditBlockMode)
		{
			IsDrawWall = false;
		}
	}
	break;
	case WM_RBUTTONDOWN:
	{
		m_ptMousePos.x = LOWORD(lParam);
		m_ptMousePos.y = HIWORD(lParam);

		if (m_ptMousePos.x < WINDOW_WIDTH && m_ptMousePos.y < WINDOW_HIGHT)
		{
			if (IsEditBlockMode)
			{
				IsRemoveWall = true;

				int iLeft = m_ptMousePos.x / CELL_SIZE;
				int iTop = m_ptMousePos.y / CELL_SIZE;

				if (g_Map[iTop][iLeft].IsWall)
				{
					g_Map[iTop][iLeft].IsWall = false;
				}
			}
			else
			{
				ClearEndNode();

				int iLeft = m_ptMousePos.x / CELL_SIZE;
				int iTop = m_ptMousePos.y / CELL_SIZE;

				g_Map[iTop][iLeft].IsEndPoint = true;
				EndNode = &g_Map[iTop][iLeft];
			}
		}
	}
	break;
	case WM_RBUTTONUP:
	{
		if (IsEditBlockMode)
		{
			IsRemoveWall = false;
		}
	}
	break;
	case WM_KEYDOWN:
	{
		switch (wParam)
		{
		case VK_SPACE:
			StartFindPath(StartNode, EndNode);
			break;
		case 0x52: // r key
			Reset();
			break;
		case 0x43: // c key
			Clear();
			break;
		}
	}
	break;
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	default:
		return DefWindowProc(hWnd, message, wParam, lParam);
	}
	return 0;
}
