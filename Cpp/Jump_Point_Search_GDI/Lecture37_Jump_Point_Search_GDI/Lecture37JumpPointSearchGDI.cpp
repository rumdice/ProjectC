// Lecture37JumpPointSearchGDI.cpp: 응용 프로그램의 진입점을 정의합니다.
//

#include "stdafx.h"
#include "Lecture37JumpPointSearchGDI.h"
#include "heap.h"
#include "neighbors.h"

int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
	_In_opt_ HINSTANCE hPrevInstance,
	_In_ LPWSTR    lpCmdLine,
	_In_ int       nCmdShow)
{
	UNREFERENCED_PARAMETER(hPrevInstance);
	UNREFERENCED_PARAMETER(lpCmdLine);

	// TODO: 여기에 코드를 입력합니다.

	// 전역 문자열을 초기화합니다.
	LoadStringW(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
	LoadStringW(hInstance, IDC_LECTURE37JUMPPOINTSEARCHGDI, szWindowClass, MAX_LOADSTRING);
	MyRegisterClass(hInstance);

	// 응용 프로그램 초기화를 수행합니다.
	if (!InitInstance(hInstance, nCmdShow))
	{
		return FALSE;
	}

	MSG msg;

	// 기본 메시지 루프입니다.
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
	wcex.hIcon = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_LECTURE37JUMPPOINTSEARCHGDI));
	wcex.hCursor = LoadCursor(nullptr, IDC_ARROW);
	wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
	wcex.lpszMenuName = MAKEINTRESOURCEW(IDC_LECTURE37JUMPPOINTSEARCHGDI);
	wcex.lpszClassName = szWindowClass;
	wcex.hIconSm = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

	return RegisterClassExW(&wcex);
}

BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
	hInst = hInstance; // 인스턴스 핸들을 전역 변수에 저장합니다.

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
		// 메뉴 선택을 구문 분석합니다.
		switch (wmId)
		{
		case IDM_EXIT:
			DestroyWindow(hWnd);
			break;
		case IDM_START_SEARCH:
			//FindPath(StartNode, EndNode);
			FindPath2(StartNode, EndNode);
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
		// TODO: 여기에 hdc를 사용하는 그리기 코드를 추가합니다.
		DrawBitMap(hdc, 0, 0, MemBit);
		EndPaint(hWnd, &ps);

		// 계속 그림
		Ready(hWnd);
		InvalidateRect(hWnd, NULL, FALSE);
	}
	break;
	case WM_LBUTTONDOWN:
	{
		m_ptMousePos.x = LOWORD(lParam);
		m_ptMousePos.y = HIWORD(lParam);

		// 화면 바깥에서 편집은 불가능하다.
		if (m_ptMousePos.x < WINDOW_WIDTH && m_ptMousePos.y < WINDOW_HIGHT)
		{
			// 블록 편집 모드일때만 허용
			if (IsEditBlockMode)
			{
				IsDrawWall = true;

				int iLeft = m_ptMousePos.x / CELL_SIZE;
				int iTop = m_ptMousePos.y / CELL_SIZE;

				// 스타트 점이거나 엔드 점이면 벽으로 칠하지 않음
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
				// 편집전 기존 스타트 노드는 날려주기
				ClearStartNode();

				// 블록 편집 모드가 아닌경우에는 StartNode 편집.
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

		// 화면 바깥에서 편집은 불가능하다.
		if (m_ptMousePos.x < WINDOW_WIDTH && m_ptMousePos.y < WINDOW_HIGHT)
		{
			if (IsEditBlockMode)
			{
				int iLeft = m_ptMousePos.x / CELL_SIZE;
				int iTop = m_ptMousePos.y / CELL_SIZE;

				if (IsDrawWall)
				{
					// 스타트 점이거나 엔드 점이면 벽으로 칠하지 않음
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
					// 이미 벽이면 도로 삭제함
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

		// 화면 바깥에서 편집은 불가능하다.
		if (m_ptMousePos.x < WINDOW_WIDTH && m_ptMousePos.y < WINDOW_HIGHT)
		{
			if (IsEditBlockMode)
			{
				IsRemoveWall = true;

				int iLeft = m_ptMousePos.x / CELL_SIZE;
				int iTop = m_ptMousePos.y / CELL_SIZE;

				// 이미 벽이면 도로 삭제함
				if (g_Map[iTop][iLeft].IsWall)
				{
					g_Map[iTop][iLeft].IsWall = false;
				}
			}
			else
			{
				// 편집전 기존 엔드 노드는 날려주기
				ClearEndNode();

				// 블록 편집 모드가 아닌경우에는 EndNode 재지정
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
		// 검색 시작 - 그리기
		switch (wParam)
		{
		case VK_SPACE:
			//FindPath(StartNode, EndNode);
			FindPath2(StartNode, EndNode);
			break;
		case 0x52: // r key
			Reset();
			break;
		case 0x43: // c key
			Clear();
			break;
		case 0x56: // v key
			//GetLine(StartNode->posX, StartNode->posY, EndNode->posX, EndNode->posY);
			optimizeLine();
			break;
		case 0x54: // t key
			GetTile(StartNode->posX, StartNode->posY, EndNode->posX, EndNode->posY);
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

void Ready(HWND hWnd)
{
	// 예제와 달리 윈도우 크기만큼 정확하게 메모리DC 생성하기 위함
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

	//메모리 비트멥을 만들고 메모리 dc에 선택
	MemBit = CreateCompatibleBitmap(hdc, winRect.right, winRect.bottom);
	MemDC = CreateCompatibleDC(hdc);
	hOldBmp = (HBITMAP)SelectObject(MemDC, MemBit);

	PatBlt(MemDC, 0, 0, winRect.right, winRect.bottom, WHITENESS);

	// 펜 선택
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

	// 이전 펜 선택
	SelectPen(MemDC, OldPen);
	// 펜 삭제
	DeletePen(MyPen);

	// 맵을 삥삥 돌며 그린다
	for (int y = 0; y < MAX_BLOCK_COUNT_HIGHT; y++)
	{
		for (int x = 0; x < MAX_BLOCK_COUNT_WIDTH; x++)
		{
			if (g_Map[y][x].IsWall == true)
			{
				// 브러쉬 선택
				MyBrush = CreateSolidBrush(COLOR_WALL);
				OldBlush = (HBRUSH)SelectObject(MemDC, MyBrush);

				int iLeft = g_Map[y][x].posX;
				int iTop = g_Map[y][x].posY;
				Rectangle(MemDC, iLeft, iTop, iLeft + CELL_SIZE, iTop + CELL_SIZE);

				// 이전 브러쉬 선텍
				SelectObject(MemDC, OldBlush);
				// 브러쉬 삭제
				DeleteBrush(MyBrush);
			}
			else if (g_Map[y][x].IsStartPoint)
			{
				// 브러쉬 선택
				MyBrush = CreateSolidBrush(COLOR_START_POINT);
				OldBlush = (HBRUSH)SelectObject(MemDC, MyBrush);

				int iLeft = g_Map[y][x].posX;
				int iTop = g_Map[y][x].posY;
				Rectangle(MemDC, iLeft, iTop, iLeft + CELL_SIZE, iTop + CELL_SIZE);

				// 이전 브러쉬 선텍
				SelectObject(MemDC, OldBlush);
				// 브러쉬 삭제
				DeleteBrush(MyBrush);
			}
			else if (g_Map[y][x].IsEndPoint)
			{
				// 브러쉬 선택
				MyBrush = CreateSolidBrush(COLOR_END_POINT);
				OldBlush = (HBRUSH)SelectObject(MemDC, MyBrush);

				int iLeft = g_Map[y][x].posX;
				int iTop = g_Map[y][x].posY;
				Rectangle(MemDC, iLeft, iTop, iLeft + CELL_SIZE, iTop + CELL_SIZE);

				// 이전 브러쉬 선텍
				SelectObject(MemDC, OldBlush);
				// 브러쉬 삭제
				DeleteBrush(MyBrush);
			}
		}
	}

	// 체크목록 색칠
	for (int y = 0; y < MAX_BLOCK_COUNT_HIGHT; y++)
	{
		for (int x = 0; x < MAX_BLOCK_COUNT_WIDTH; x++)
		{
			if (g_Map[y][x].checked)
			{
				// 브러쉬 선택
				MyBrush = CreateSolidBrush(COLOR_SEARCHED_LIST);
				OldBlush = (HBRUSH)SelectObject(MemDC, MyBrush);

				int iLeft = x * CELL_SIZE;
				int iTop = y * CELL_SIZE;
				Rectangle(MemDC, iLeft, iTop, iLeft + CELL_SIZE, iTop + CELL_SIZE);

				// 이전 브러쉬 선텍
				SelectObject(MemDC, OldBlush);
				// 브러쉬 삭제
				DeleteBrush(MyBrush);
			}
		}
	}

	// 오픈리스트 색칠
	for (auto node : OpenList)
	{
		if (node != NULL)
		{
			if (node->IsStartPoint)
				continue;

			if (node->IsEndPoint)
				continue;

			// 브러쉬 선택
			MyBrush = CreateSolidBrush(COLOR_OPENLIST);
			OldBlush = (HBRUSH)SelectObject(MemDC, MyBrush);

			int iLeft = node->posX;
			int iTop = node->posY;
			Rectangle(MemDC, iLeft, iTop, iLeft + CELL_SIZE, iTop + CELL_SIZE);

			// 이전 브러쉬 선텍
			SelectObject(MemDC, OldBlush);
			// 브러쉬 삭제
			DeleteBrush(MyBrush);

		}
	}

	// 클로즈리스트 색칠
	//for (auto node : CloseList)
	//{
	//	// 브러쉬 선택
	//	MyBrush = CreateSolidBrush(COLOR_CLOSELIST);
	//	OldBlush = (HBRUSH)SelectObject(MemDC, MyBrush);

	//	int iLeft = node->posX;
	//	int iTop = node->posY;
	//	Rectangle(MemDC, iLeft, iTop, iLeft + CELL_SIZE, iTop + CELL_SIZE);

	//	// 이전 브러쉬 선텍
	//	SelectObject(MemDC, OldBlush);
	//	// 브러쉬 삭제
	//	DeleteBrush(MyBrush);
	//}

	// 최적화 옵션을 키면 최적 경로의 타일을 찾는다.
	if (OptimizeFlag)
	{
		//optimizeLine(MemDC);

		/*for (auto node : OptimizedList)
		{
			if (CheckStartNode(node->posX,node->posY))
			{
				continue;
			}

			if (CheckStartNode(node->posX, node->posY))
			{
				continue;
			}

			OptimizedList.
			Optimizes.push_back(node);
		}*/
	}

	// 두 점간 직선 타일 표기옵션을 구하면 오렌지 색으로 구한다.
	if (DrawTileFlag)
	{
		int iLeft = StartNode->posX + (CELL_SIZE / 2);
		int iTop = StartNode->posY + (CELL_SIZE / 2);

		int iLeft2 = EndNode->posX + (CELL_SIZE / 2);
		int iTop2 = EndNode->posY + (CELL_SIZE / 2);

		// 브러쉬 선택
		MyBrush = CreateSolidBrush(COLOR_ORANGE);
		OldBlush = (HBRUSH)SelectObject(MemDC, MyBrush);

		drawline(iLeft / 32, iTop / 32, iLeft2 / 32, iTop2 / 32, MemDC);

		// 브러쉬 삭제
		SelectObject(MemDC, OldBlush);
		DeleteBrush(MyBrush);
	}

	// 최적화 경로 노드 칠하기
	for (auto node : OptimizedList)
	{
		// 브러쉬 선택
		MyBrush = CreateSolidBrush(COLOR_CLOSELIST);
		OldBlush = (HBRUSH)SelectObject(MemDC, MyBrush);

		int iLeft = node->posX;
		int iTop = node->posY;
		Rectangle(MemDC, iLeft, iTop, iLeft + CELL_SIZE, iTop + CELL_SIZE);

		// 이전 브러쉬 선텍
		SelectObject(MemDC, OldBlush);
		// 브러쉬 삭제
		DeleteBrush(MyBrush);
	}

	// 결과로 나온 Path 경로 색칠
	for (auto node : PathList)
	{
		// 아니면 그냥 펜으로 연결선 그리기

		// 펜 선택
		MyPen = CreatePen(PS_SOLID, 2, COLOR_PATHLIST);
		OldPen = (HPEN)SelectObject(MemDC, MyPen);

		int iLeft = node->posX + (CELL_SIZE / 2);
		int iTop = node->posY + (CELL_SIZE / 2);

		int iLeft2 = node->parent->posX + (CELL_SIZE / 2);
		int iTop2 = node->parent->posY + (CELL_SIZE / 2);

		// 잠깐 주석처리
		MoveToEx(MemDC, iLeft, iTop, NULL);
		LineTo(MemDC, iLeft2, iTop2);

		// 이전 펜 선택
		SelectPen(MemDC, OldPen);
		// 펜 삭제
		DeletePen(MyPen);
	}

	// 최적화 경로 Path 그리기
	if (Optimizes.size() > 0)
	{
		for (int i = 0; i < Optimizes.size(); i++)
		{
			// 펜 선택
			MyPen = CreatePen(PS_SOLID, 2, COLOR_BROWN);
			OldPen = (HPEN)SelectObject(MemDC, MyPen);

			int iLeft = Optimizes[i]->posX + (CELL_SIZE / 2);
			int iTop = Optimizes[i]->posY + (CELL_SIZE / 2);

			int iLeft2 = 0;
			int iTop2 = 0;

			if (i < Optimizes.size()-1)
			{
				iLeft2 = Optimizes[i + 1]->posX + (CELL_SIZE / 2);
				iTop2 = Optimizes[i + 1]->posY + (CELL_SIZE / 2);
			}
			else 
			{
				iLeft2 = StartNode->posX + (CELL_SIZE / 2);
				iTop2 = StartNode->posY + (CELL_SIZE / 2);
			}

			MoveToEx(MemDC, iLeft, iTop, NULL);
			LineTo(MemDC, iLeft2, iTop2);

			// 이전 펜 선택
			SelectPen(MemDC, OldPen);
			// 펜 삭제
			DeletePen(MyPen);
		}
	}

	// 마무리
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
	// 맵을 삥삥 돌며 초기화
	for (int y = 0; y < MAX_BLOCK_COUNT_HIGHT; y++)
	{
		for (int x = 0; x < MAX_BLOCK_COUNT_WIDTH; x++)
		{
			g_Map[y][x].posX = x * CELL_SIZE;
			g_Map[y][x].posY = y * CELL_SIZE;
			g_Map[y][x].IsWall = false;
		}
	}

	// Start 지점 초기값 설정
	g_Map[DEFAULT_START_POS_Y][DEFAULT_START_POS_X].IsStartPoint = true;
	StartNode = &g_Map[DEFAULT_START_POS_Y][DEFAULT_START_POS_X];

	// End 지점 초기값 설정
	g_Map[DEFAULT_END_POS_Y][DEFAULT_END_POS_X].IsEndPoint = true;
	EndNode = &g_Map[DEFAULT_END_POS_Y][DEFAULT_END_POS_X];
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
	// 어차피 좌표 검사.
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

		// 그 노드가 이미 닫힌 리스트에 있는가?
		if (FindNodeOnList(CloseList, newNode))
		{
			continue;
		}

		// 그 노드가 충돌체랑 부딛히나?
		// 충돌체 리스트에 있는 녀석이면 패스
		if (FindNodeOnList(WallList, newNode))
		{
			continue;
		}

		/// ?? 무슨 의미의 코드지? 이 코드의 의미 확실히 밝혀 내기
		// 대각선 노드일 경우 가중치를 준다 (비용)
		int totalCost = currentNode->G + ((i < 4) ? 10 : 14);

		// 찾은 서브 노드가 이미 오픈리스트에 담겨있던 놈인가?
		NODE *successor = FindNodeOnList(OpenList, newNode);

		// 없는 놈인경우 완전 새로운 서브노드 생성
		if (successor == nullptr)
		{
			successor = new NODE(newNode.posY, newNode.posX);
			successor->parent = currentNode; // 맞는지 검증 필요

			// 어떻게 가중치를 알아내지? - 의 질문에 대한 답변 부분
			successor->G = totalCost;
			successor->H = (int)Heuristic(successor->posX, successor->posY, EndNode->posX, EndNode->posY) / g_weight;
			successor->F = successor->G + successor->H;

			// 새로운 자식노드를 OpenList에 추가
			OpenList.push_back(successor);
		}
		else if (totalCost < successor->G)// 기존에 있는 노드와 비용 검사? - 이부분이 제대로 동작 할까??
		{
			successor->parent = currentNode;
			successor->G = totalCost;
		}
	}

	//return nodeArr;
	return NULL;
}

void InitWallList()
{
	// 맵을 삥삥 돌며 
	for (int y = 0; y < MAX_BLOCK_COUNT_HIGHT; y++)
	{
		for (int x = 0; x < MAX_BLOCK_COUNT_WIDTH; x++)
		{
			// 벽인경우
			if (g_Map[y][x].IsWall == true)
			{
				// 벽 리스트에 넣는다
				NODE *node = &g_Map[y][x];
				WallList.push_back(node);
			}
		}
	}
}

void ClearWallList()
{
	// 맵을 삥삥 돌며 
	for (int y = 0; y < MAX_BLOCK_COUNT_HIGHT; y++)
	{
		for (int x = 0; x < MAX_BLOCK_COUNT_WIDTH; x++)
		{
			// 벽인경우
			if (g_Map[y][x].IsWall == true)
			{
				// 벽 리스트 정보 클리어
				g_Map[y][x].IsWall = false;
			}
		}
	}

	// 월 리스트도 클리어
	WallList.clear();
}

void ClearSearchedPath()
{
	// 맵을 삥삥 돌며 
	for (int y = 0; y < MAX_BLOCK_COUNT_HIGHT; y++)
	{
		for (int x = 0; x < MAX_BLOCK_COUNT_WIDTH; x++)
		{
			if (g_Map[y][x].checked)
			{
				g_Map[y][x].checked = false;
			}
		}
	}
}

void ClearStartNode()
{
	// 맵을 삥삥 돌며 
	for (int y = 0; y < MAX_BLOCK_COUNT_HIGHT; y++)
	{
		for (int x = 0; x < MAX_BLOCK_COUNT_WIDTH; x++)
		{
			// StartNode의 경우 클리어
			if (g_Map[y][x].IsStartPoint)
			{
				g_Map[y][x].IsStartPoint = false;
			}
		}
	}
}

void ClearEndNode()
{
	// 맵을 삥삥 돌며 
	for (int y = 0; y < MAX_BLOCK_COUNT_HIGHT; y++)
	{
		for (int x = 0; x < MAX_BLOCK_COUNT_WIDTH; x++)
		{
			// EndNode의 경우 클리어
			if (g_Map[y][x].IsEndPoint)
			{
				g_Map[y][x].IsEndPoint = false;
			}
		}
	}
}

void Reset()
{
	// 완전 리셋 함수 - 초기 상태로 돌린다

	// 시작노드와 EndNode를 초기 위치로 옮긴다.
	ClearStartNode();
	ClearEndNode();

	// Start 지점 초기값 설정
	g_Map[DEFAULT_START_POS_Y][DEFAULT_START_POS_X].IsStartPoint = true;
	StartNode = &g_Map[DEFAULT_START_POS_Y][DEFAULT_START_POS_X];

	// End 지점 초기값 설정
	g_Map[DEFAULT_END_POS_Y][DEFAULT_END_POS_X].IsEndPoint = true;
	EndNode = &g_Map[DEFAULT_END_POS_Y][DEFAULT_END_POS_X];

	// 벽 삭제
	ClearWallList();

	// 오픈 리스트 삭제
	OpenList.clear();

	// 클로즈 리스트 삭제
	CloseList.clear();

	// 패쓰 리트트 삭제
	// 라인 그린거 표시 안되게 하기
	PathList.clear();

	ClearSearchedPath();

	// 최적화 노드 클리어
	OptimizeFlag = false;
	DrawTileFlag = false;
	PathNodes.clear();
	OptimizedList.clear();
	Optimizes.clear();
}

void Clear()
{
	// 재시작시 활용 함수 - 벽, 시작, 종점 유지하고 찾은 경로만 싹 클리어한다
	OpenList.clear();
	CloseList.clear();
	PathList.clear();

	// 최적화 노드 클리어
	OptimizeFlag = false;
	DrawTileFlag = false;
	PathNodes.clear();
	OptimizedList.clear();
	Optimizes.clear();
}

void FindPath(NODE* startNode, NODE *endNode)
{
	// 반복 수행의 경우가 있으니 리스트를 비울것.
	Clear();

	// 시작전에 벽정보 블럭을 벽 리스트에 넣어준다.
	InitWallList();

	// 시작 노드를 오픈리스트에 넣을것.
	OpenList.push_back(startNode);

	// 시작노드의 opened 속성을 true 로 해주자.
	startNode->opened = true;

	NODE *current = nullptr; // 현재 노드의 위치
	NODE *activeNode = nullptr;

	// 현재 노드를 오픈리스트에서 꺼넨다.
	current = *OpenList.begin();

	// 기존 astar 코드
	while (!OpenList.empty())
	{
		//activeNode = current;
		// 현재 노드를 오픈리스트에서 꺼넨다.
		current = *OpenList.begin();

		// 오픈 리스트에서 현재노드는 제거한다.
		OpenList.erase(find(OpenList.begin(), OpenList.end(), current));

		// 현재 노드는 클로즈 리스트에 넣는다.
		CloseList.push_back(current);
		//current->closed = true;
		//activeNode->closed = true;

		/// 요건 이제 못쓴다?
		/// ??? 무슨 의미가 있는 루프인가?
		/*for (auto node : OpenList)
		{
		if (node->F <= current->F)
		{
		current = node;
		}
		}*/

		// 현재 노드 위치가 EndPos(목적지)라면 종료
		// 매 루프마다 비교함수 콜을 때리느니 그냥 단순 비교...(속도)
		if (current->posX == endNode->posX && current->posY == endNode->posY)
		{
			// 임시코드 - 찾았으면 찾은 노드는 openList에서 제거
			//.erase(find(OpenList.begin(), OpenList.end(), current));

			// 임시코드 - 찾았으면 스타트 노드는 closeList에서 제거
			//CloseList.erase(find(CloseList.begin(), CloseList.end(), StartNode));

			break;
		}

		// 오픈 리스트에서 현재노드는 제거한다.
		//OpenList.erase(find(OpenList.begin(), OpenList.end(), current));

		// 현재 노드는 클로즈 리스트에 넣는다.
		//CloseList.push_back(current);
		//current->closed = true;

		//// 8 방향의 서브노드를 구한다.
		//GetSubNodes(current);

		// GetSubNodes 역활을 하는 함수 (JPS 알고리즘)
		IdentifySuccessors(current);
	}

	// Path 경로를 저장한다.
	while (current != nullptr)
	{
		PathList.push_back(current);
		current = current->parent;
	}

	//스타트 노드와 pathList에서 제거 (표기문제)
	PathList.erase(find(PathList.begin(), PathList.end(), startNode));
}

// FindPath의 버그 수정 함수
void FindPath2(NODE* startNode, NODE *endNode)
{
	// 반복 수행의 경우가 있으니 리스트를 비울것.
	Clear();

	// 시작전에 벽정보 블럭을 벽 리스트에 넣어준다.
	InitWallList();

	open_list *head = ol_new();
	open_list *current = head;
	NODE *activeNode;
	int counter = 0;

	current = ol_insert_right(current, startNode);

	startNode->opened = true;

	head = ol_listsort(head);
	current = head->left;

	OpenList.push_back(startNode);

	while (head != current) // head가 가리키는 리스트, 즉 ol(openlist)가 빌때까지 반복
	{
		activeNode = current->list_node;
		current = ol_del_free(current);
		activeNode->closed = true;

		// 성공자 탐색
		_identifySuccessors(activeNode, current, endNode);
		head = ol_listsort(head);
		current = head->right;

		// tempcode:
		OpenList.push_back(current->list_node);

		// tempcode:
		// 중간에 아무것도 없으면 여기 안걸림
		if (current->list_node != NULL)
		{
			if (current->list_node->posX == endNode->posX && current->list_node->posY == endNode->posY)
			{
				//ol_clean(head);

				// 도착한노드가 엔드노드임을 표기하기위해 넣은 코드
				current->list_node->IsEndPoint = true;

				// 도착했으면 pathlist에 넣어본다.
				backtrace(current->list_node);

				break;
			}
		}
		else
		{
			//ol_clean(head);
			OpenList.push_back(EndNode);

			// 아무것도 없는경우 일단은 선긋기를 위해 pathlist 에 도착지를 넣는다.
			makepath(StartNode, EndNode);
		}

		//counter++;
		//if (counter >= 5000)
		//{
		//	ol_clean(head);
		//	//return NULL;
		//}
	}

	// 오픈리스트들을 배열에 담자..
	for (auto node : PathList) // end부터 들어감.
	{
		PathNodes.push_back(node);
	}

}

void IdentifySuccessors(NODE* currentNode)
{
	vector<NODE>neighbors = FindNeighbors(currentNode);

	NODE neighbor;

	NODE* jumpPoint = new NODE();
	for (unsigned int i = 0; i < neighbors.size(); i++)
	{
		neighbor = neighbors[i];

		jumpPoint = Jump(neighbor.posX, neighbor.posY, currentNode->posX, currentNode->posY);
		if (jumpPoint)
		{
			// 노드리턴의 경우
			int jx = jumpPoint->posX;
			int jy = jumpPoint->posY;

			// 찾은 노드가 클로즈 리스트에 있는가?
			NODE jNode;
			jNode = *jumpPoint;
			jNode.G = currentNode->G;

			if (FindNodeOnList(CloseList, jNode))
			{
				continue;
			}
			/*if (g_Map[jy / CELL_SIZE][jx / CELL_SIZE].closed)
			{
				continue;
			}*/

			// g를 구하는 방식이 좀...달랐던가?
			int d = 0;
			//int d = Heuristic(jx, jy, currentNode->posX, currentNode->posY);

			//// 전체 크기가 온경우 앞의 4개는 직선, 뒤의 4개는 대각선
			if (neighbors.size() == 8)
			{
				d = (i < 4) ? 10 : 14;
			}
			// 대각선 이웃을 구한경우 앞의 2개는 직선 이웃노드 뒤 3개는 대각선 이웃노드 
			if (neighbors.size() == 5)
			{
				d = (i < 2) ? 10 : 14;
			}
			// 직선 이웃을 구한경우 앞 1개는 직선 이웃노드 뒤 2개는 대각선 이웃노드
			if (neighbors.size() < 5)
			{
				d = (i < 1) ? 10 : 14;
			}
			int ng = currentNode->G + d;

			if (!FindNodeOnList(OpenList, jNode) || ng < jNode.G)
				// 오픈리스트에 있는거랑 오픈드랑은 다른 개념이다.
				//if(g_Map[jy / CELL_SIZE][jx / CELL_SIZE].opened || ng < jNode.G)
			{
				jumpPoint->G = ng;
				if (!jumpPoint->H)
					jumpPoint->H = Heuristic(jx, jy, EndNode->posX, EndNode->posY);
				jumpPoint->F = jumpPoint->G + jumpPoint->H;
				jumpPoint->parent = currentNode;

				if (!FindNodeOnList(OpenList, jNode)) // 이 조건식은 현재 프리패스나 마찬가지
				// 오픈리스트에 있는거랑 오픈드랑은 다른 개념이다.
				//if (g_Map[jy / CELL_SIZE][jx / CELL_SIZE].opened)
				{
					NODE* tempNode = new NODE();
					tempNode->G = ng;
					tempNode->H = jumpPoint->H;
					tempNode->F = jumpPoint->F;
					tempNode->parent = jumpPoint->parent;
					tempNode->posX = jumpPoint->posX;
					tempNode->posY = jumpPoint->posY;
					tempNode->opened = true;

					g_Map[jy / CELL_SIZE][jx / CELL_SIZE].opened = true;

					// 결국 이게 많이 호출되서 노드들이 생성된다.
					OpenList.push_back(tempNode);
					//delete tempNode;
				}
			}
		}
	}
}

// IdentifySuccessors 의 버그 수정 함수
void _identifySuccessors(NODE* activeNode, open_list* current, NODE* endNode)
{
	int endX = endNode->posX;
	int endY = endNode->posY;
	int *jumpPoint;

	neighbor_xy_list *neighbors_head = _findNeighbors(activeNode);
	neighbor_xy_list *neighbors_current = neighbors_head;

	while (neighbors_head != (neighbors_current = neighbors_current->right))
	{
		jumpPoint = _jump(neighbors_current->x, neighbors_current->y, activeNode->posX, activeNode->posY);
		{
			if (jumpPoint != NULL)
			{
				int jx, jy, d, ng;

				jx = jumpPoint[0];
				jy = jumpPoint[1];

				free(jumpPoint);
				malloc_count--; /* [ Malloc Count ] */

				NODE jumpNode = getNodeAt(jx, jy);
				if (jumpNode.closed) {
					continue;
				}

				d = euclidean(abs(jx - activeNode->posX), abs(jy - activeNode->posY));
				ng = activeNode->G + d;

				if (!jumpNode.opened || ng < jumpNode.G)
				{
					jumpNode.G = ng;
					if (!jumpNode.H)
						jumpNode.H = manhattan(abs(jx - endX), abs(jy - endY));

					jumpNode.F = jumpNode.G + jumpNode.H;
					jumpNode.parent = activeNode;

					if (!jumpNode.opened) {

						// tempcode:
						NODE* tempNode = new NODE();
						tempNode->G = ng;
						tempNode->H = jumpNode.H;
						tempNode->F = jumpNode.F;
						tempNode->parent = jumpNode.parent;
						tempNode->posX = jumpNode.posX;
						tempNode->posY = jumpNode.posY;
						tempNode->opened = true;
						current = ol_insert_right(current, tempNode);

						//current = ol_insert_right(current, &jumpNode);
						jumpNode.opened = true;
					}
					else {
						ol_listsort(current->right);
					}
				}
			}
		}
	}
	neighbor_xy_clean(neighbors_head);
}

vector<NODE> FindNeighbors(NODE* currentNode)
{
	vector<NODE>neighbors;

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

	if (currentNode->parent) // 부모노드가 있는 경우에는 대각선과 직선별로 리턴해주는 이웃이 다르다.
	{
		int px = currentNode->parent->posX;
		int py = currentNode->parent->posY;

		int dx = (currentNode->posX - px) / max(abs(currentNode->posX - px), CELL_SIZE);
		int dy = (currentNode->posY - py) / max(abs(currentNode->posY - py), CELL_SIZE);

		// stack overflow 에러의 원인으로 짐작
		// 이곳에서 dx, dy가 1이 나와서 아래 새로운 탐색 노드의 좌표를 1씩 더함.
		// 이곳을 호출하는 부분에서 좌표를 1씩 검색하니 jump 함수의 호출 횟수가 너무 높아진것이 아닐까...
		// 그래서 일단은 1이 나왔을 경우 cell_size를 곱해봄
		if (dx == 1 || dx == -1)
		{
			dx = dx * CELL_SIZE;
		}

		if (dy == 1 || dy == -1)
		{
			dy = dy * CELL_SIZE;
		}

		// 대각선 검색
		if (dx != 0 && dy != 0)
		{
			if (IsWalkable(currentNode->posX, currentNode->posY + dy))
			{
				NODE newNode(currentNode->posY + dy, currentNode->posX); // 상 혹은 좌 (음수가 들어올 수 있음)
				neighbors.push_back(newNode);
			}

			if (IsWalkable(currentNode->posX + dx, currentNode->posY))
			{
				NODE newNode(currentNode->posY, currentNode->posX + dx); // 우 혹은 좌(음수)
				neighbors.push_back(newNode);
			}

			if (IsWalkable(currentNode->posX + dx, currentNode->posY + dy))
			{
				NODE newNode(currentNode->posY + dy, currentNode->posX + dx); // 우상
				neighbors.push_back(newNode);
			}

			if (IsWalkable(currentNode->posX - dx, currentNode->posY))
			{
				NODE newNode(currentNode->posY + dy, currentNode->posX - dx); // 좌상
				neighbors.push_back(newNode);
			}

			if (IsWalkable(currentNode->posX, currentNode->posY - dy))
			{
				NODE newNode(currentNode->posY - dy, currentNode->posX + dx); // 우하
				neighbors.push_back(newNode);
			}
		}
		// 수평/수직 검색
		else
		{
			if (dx == 0)
			{
				if (IsWalkable(currentNode->posX, currentNode->posY + dy))
				{
					NODE newNode(currentNode->posY + dy, currentNode->posX); // 직선
					neighbors.push_back(newNode);
				}

				if (IsWalkable(currentNode->posX + CELL_SIZE, currentNode->posY))
				{
					NODE newNode(currentNode->posY + dy, currentNode->posX + CELL_SIZE); // 대각선
					neighbors.push_back(newNode);
				}

				if (IsWalkable(currentNode->posX - CELL_SIZE, currentNode->posY))
				{
					NODE newNode(currentNode->posY + dy, currentNode->posX - CELL_SIZE); // 대각선
					neighbors.push_back(newNode);
				}
			}
			else
			{
				if (IsWalkable(currentNode->posX + dx, currentNode->posY))
				{
					NODE newNode(currentNode->posY, currentNode->posX + dx); // 직선
					neighbors.push_back(newNode);
				}

				if (IsWalkable(currentNode->posX, currentNode->posY + CELL_SIZE))
				{
					NODE newNode(currentNode->posY + CELL_SIZE, currentNode->posX + dx); // 대각선
					neighbors.push_back(newNode);
				}

				if (IsWalkable(currentNode->posX, currentNode->posY - CELL_SIZE))
				{
					NODE newNode(currentNode->posY - CELL_SIZE, currentNode->posX + dx); // 대각선
					neighbors.push_back(newNode);
				}
			}
		}
	}
	else
	{
		// 8방향의 모든 노드를 이웃목록에 담아서 준다.
		for (int i = 0; i < 8; i++)
		{
			NODE newNode(posArrY[i], posArrX[i]);
			neighbors.push_back(newNode);
		}
	}

	return neighbors;
}

struct neighbor_xy_list *_findNeighbors(NODE* activeNode)
{
	NODE* parent = activeNode->parent;
	int x = activeNode->posX;
	int y = activeNode->posY;
	int px, py, dx, dy;

	neighbor_xy_list *head = neighbor_xy_new();
	neighbor_xy_list *current = head;

	NODE* neighborNode;
	neighbor_list *neighborNodes_head;
	neighbor_list *neighborNodes_current;

	if (parent)
	{
		px = parent->posX;
		py = parent->posY;

		/*dx = (x - px) / max(abs(x - px), 1);
		dy = (y - py) / max(abs(y - py), 1);*/

		dx = (activeNode->posX - px) / max(abs(activeNode->posX - px), CELL_SIZE);
		dy = (activeNode->posY - py) / max(abs(activeNode->posY - py), CELL_SIZE);

		// stack overflow 에러의 원인으로 짐작
		// 이곳에서 dx, dy가 1이 나와서 아래 새로운 탐색 노드의 좌표를 1씩 더함.
		// 이곳을 호출하는 부분에서 좌표를 1씩 검색하니 jump 함수의 호출 횟수가 너무 높아진것이 아닐까...
		// 그래서 일단은 1이 나왔을 경우 cell_size를 곱해봄
		if (dx == 1 || dx == -1)
		{
			dx = dx * CELL_SIZE;
		}

		if (dy == 1 || dy == -1)
		{
			dy = dy * CELL_SIZE;
		}

		/* Diagonals */
		if (dx != 0 && dy != 0) {
			if (IsWalkable(x, (y + dy))) {
				current = neighbor_xy_insert_right(current, x, (y + dy));
			}
			if (IsWalkable((x + dx), y)) {
				current = neighbor_xy_insert_right(current, (x + dx), y);
			}
			if (IsWalkable(x, (y + dy)) || IsWalkable((x + dx), y)) {
				current = neighbor_xy_insert_right(current, (x + dx), (y + dy));
			}
			if (!IsWalkable((x - dx), y) && IsWalkable(x, (y + dy))) {
				current = neighbor_xy_insert_right(current, (x - dx), (y + dy));
			}
			if (!IsWalkable(x, (y - dy)) && IsWalkable((x + dx), y)) {
				current = neighbor_xy_insert_right(current, (x + dx), (y - dy));
			}

			/* Horizontal / Vertical */
		}
		else {
			if (dx == 0) {
				if (IsWalkable(x, (y + dy))) {
					if (IsWalkable(x, (y + dy))) {
						current = neighbor_xy_insert_right(current, x, (y + dy));
					}
					if (!IsWalkable((x + CELL_SIZE), y)) {
						current = neighbor_xy_insert_right(current, (x + CELL_SIZE), (y + dy));
					}
					if (!IsWalkable((x - CELL_SIZE), y)) {
						current = neighbor_xy_insert_right(current, (x - CELL_SIZE), (y + dy));
					}
				}
			}
			else {
				if (IsWalkable((x + dx), y)) {
					if (IsWalkable((x + dx), y)) {
						current = neighbor_xy_insert_right(current, (x + dx), y);
					}
					if (!IsWalkable(x, (y + CELL_SIZE))) {
						current = neighbor_xy_insert_right(current, (x + dx), (y + CELL_SIZE));
					}
					if (!IsWalkable(x, (y - CELL_SIZE))) {
						current = neighbor_xy_insert_right(current, (x + dx), (y - CELL_SIZE));
					}
				}
			}
		}
	}
	else {
		neighborNodes_head = getNeighbors(activeNode);
		neighborNodes_current = neighborNodes_head;
		while (neighborNodes_head != (neighborNodes_current = neighborNodes_current->right)) {
			neighborNode = neighborNodes_current->neighbor_node;
			current = neighbor_xy_insert_right(current, neighborNode->posX, neighborNode->posY);
		}
		clean_neighbor_list(neighborNodes_head);
	}

	return head;
}

struct neighbor_list *new_neighbor_list()
{
	struct neighbor_list *newlist = (struct neighbor_list *) malloc(sizeof(struct neighbor_list));
	malloc_count++; /* [ Malloc Count ] */
	newlist->right = newlist;
	newlist->left = newlist;
	newlist->neighbor_node = NULL;
	return newlist;
};

struct neighbor_list *getNeighbors(NODE* nd)
{
	int x = nd->posX;
	int y = nd->posY;

	neighbor_list *head = new_neighbor_list();
	neighbor_list *current = head;

	bool d0 = false;
	bool d1 = false;
	bool d2 = false;
	bool d3 = false;

	/* UP */
	if (IsWalkable(x, y - CELL_SIZE)) {
		NODE* nNode = new NODE(y - CELL_SIZE, x);
		current = insert_right(current, nNode);
		d0 = d1 = true;

		// tempcode:
		/*int py = y - CELL_SIZE;
		int px = x;
		paintcheckNode(px, py);*/
	}

	/* RIGHT */
	if (IsWalkable(x + CELL_SIZE, y)) {
		NODE* nNode = new NODE(y, x + CELL_SIZE);
		current = insert_right(current, nNode);
		d1 = d2 = true;

		// tempcode:
		/*int py = y;
		int px = x + CELL_SIZE;
		paintcheckNode(px, py);*/
	}

	/* DOWN */
	if (IsWalkable(x, y + CELL_SIZE)) {
		NODE* nNode = new NODE(y + CELL_SIZE, x);
		current = insert_right(current, nNode);
		d2 = d3 = true;

		// tempcode:
		/*int py = y + CELL_SIZE;
		int px = x;
		paintcheckNode(px, py);*/
	}

	/* LEFT */
	if (IsWalkable(x - CELL_SIZE, y)) {
		NODE* nNode = new NODE(y, x - CELL_SIZE);
		current = insert_right(current, nNode);
		d3 = d0 = true;

		// tempcode:
		/*int py = y;
		int px = x - CELL_SIZE;
		paintcheckNode(px, py);*/
	}

	/* UP + LEFT */
	if (d0 && IsWalkable(x - CELL_SIZE, y - CELL_SIZE)) {
		NODE* nNode = new NODE(y - CELL_SIZE, x - CELL_SIZE);
		current = insert_right(current, nNode);

		// tempcode:
		/*int py = y - CELL_SIZE;
		int px = x - CELL_SIZE;
		paintcheckNode(px, py);*/
	}

	/* UP + RIGHT */
	if (d1 && IsWalkable(x + CELL_SIZE, y - CELL_SIZE)) {
		NODE* nNode = new NODE(y - CELL_SIZE, x + CELL_SIZE);
		current = insert_right(current, nNode);

		// tempcode:
		/*int py = y - CELL_SIZE;
		int px = x + CELL_SIZE;
		paintcheckNode(px, py);*/
	}

	/* DOWN + RIGHT */
	if (d2 && IsWalkable(x + CELL_SIZE, y + CELL_SIZE)) {
		NODE* nNode = new NODE(y + CELL_SIZE, x + CELL_SIZE);
		current = insert_right(current, nNode);

		// tempcode:
		/*int py = y + CELL_SIZE;
		int px = x + CELL_SIZE;
		paintcheckNode(px, py);*/
	}

	/* DOWN + LEFT */
	if (d3 && IsWalkable(x - CELL_SIZE, y + CELL_SIZE)) {
		NODE* nNode = new NODE(y + CELL_SIZE, x - CELL_SIZE);
		current = insert_right(current, nNode);

		// tempcode:
		/*int py = y + CELL_SIZE;
		int px = x - CELL_SIZE;
		paintcheckNode(px, py);*/
	}

	return head;
}

struct neighbor_list *insert_right(neighbor_list *list, NODE* data)
{
	neighbor_list *newlist = (neighbor_list *)malloc(sizeof(neighbor_list));
	malloc_count++;         /* [ Malloc Count ] */
	newlist->neighbor_node = data;
	newlist->left = list;
	newlist->right = list->right;
	list->right = newlist;
	newlist->right->left = newlist;
	return newlist;
}


NODE* Jump(int x, int y, int px, int py)
{
	int dx = x - px;
	int dy = y - py;

	// 이웃노드가 접근 가능한 공간인지 판단.
	if (!IsWalkable(x, y))
	{
		return NULL;
	}

	// 이웃노드가 EndNode인가?
	if (CheckEndNode(x, y))
	{
		// 이웃노드를 리턴
		NODE* nNode = new NODE(y, x);
		return nNode;
	}

	// 강제된 이웃을 체크한다.
	// 대각선
	if (dx != 0 && dy != 0)
	{
		if (IsWalkable(x - dx, y + dy) && !IsWalkable(x - dx, y) ||
			IsWalkable(x + dx, y - dy) && !IsWalkable(x, y - dy))
		{
			// 이웃노드를 리턴
			NODE* nNode = new NODE(y, x);
			return nNode;
		}

		// 대각선으로 이동할때 수직/수평 점프 포인트를 반드시 체크
		// 512, 416일때 스택이 정리되는게 아니라 요게 콜 되버림
		// 스텍이 정리되는것은 맞는데 이곳에서 호출됨.
		//if (Jump(x + dx, y, x, y) /*|| Jump(x, y + dy, x, y)*/)
		//{
		//	// 이 부분이 문제의 핵심은 맞는듯.
		//	NODE* nNode = new NODE(y, x);
		//	return nNode;
		//}

		//if (Jump(x, y + dy, x, y))
		//{
		//	// 이 부분이 문제의 핵심은 맞는듯.
		//	NODE* nNode = new NODE(y, x);
		//	return nNode;
		//}
	}
	// 수평/수직
	else
	{
		if (dx != 0) // x 이동중
		{
			if (IsWalkable(x + dx, y + CELL_SIZE) && !IsWalkable(x, y + CELL_SIZE) ||
				IsWalkable(x + dx, y - CELL_SIZE) && !IsWalkable(x, y - CELL_SIZE))
			{
				// 이웃노드를 리턴
				NODE* nNode = new NODE(y, x);
				return nNode;
			}
		}
		// y 이동
		else
		{
			if (IsWalkable(x + CELL_SIZE, y + dy) && !IsWalkable(x + CELL_SIZE, y) ||
				IsWalkable(x - CELL_SIZE, y + dy) && !IsWalkable(x - CELL_SIZE, y))
			{
				// 이웃노드를 리턴
				NODE* nNode = new NODE(y, x);
				return nNode;
			}
		}
	}

	// 추가 로직 수정
	if (dx != 0 && dy != 0)
	{
		NODE* jxNode = Jump(x + dx, y, x, y);
		NODE* jyNode = Jump(x, y + dy, x, y);

		if (jxNode || jyNode)
		{
			if (jxNode)
				delete jxNode;

			if (jxNode)
				delete jyNode;
		}

		NODE* nNode = new NODE(y, x);
		return nNode;
	}

	//return Jump(x + dx, y + dy, x, y);

	if (IsWalkable(x + dx, y) || IsWalkable(x, (y + dy)))
	{
		return Jump(x + dx, y + dy, x, y);
	}
	else
	{
		return NULL;
	}

}

// Jump함수의 버그 수정 함수 (매우 중요)
int* _jump(int x, int y, int px, int py)
{
	int dx = x - px;
	int dy = y - py;
	int *jx, *jy;

	// tempcode:
	paintcheckNode(x, y);

	if (!IsWalkable(x, y))
	{
		return NULL;
	}
	else if (CheckEndNode(x, y))
	{
		int *i = (int *)malloc(2 * sizeof(int));
		i[0] = x;
		i[1] = y;

		return i;
	}
	/*else if (getNodeAt(x, y) == EndNode)
	{
		int *i = (int *)malloc(2 * sizeof(int));
		i[0] = x;
		i[1] = y;
		return i;
	}*/

	// 대각선
	if (dx != 0 && dy != 0) {
		if ((IsWalkable((x - dx), (y + dy)) && !IsWalkable((x - dx), y)) ||
			(IsWalkable((x + dx), (y - dy)) && !IsWalkable(x, (y - dy)))) {
			int *i = (int *)malloc(2 * sizeof(int));
			malloc_count++;         /* [ Malloc Count ] */
			i[0] = x;
			i[1] = y;

			return i;
		}
	}
	else {
		if (dx != 0) {
			if ((IsWalkable((x + dx), (y + CELL_SIZE)) && !IsWalkable(x, (y + CELL_SIZE))) ||
				(IsWalkable((x + dx), (y - CELL_SIZE)) && !IsWalkable(x, (y - CELL_SIZE)))) {
				int *i = (int *)malloc(2 * sizeof(int));
				malloc_count++;         /* [ Malloc Count ] */
				i[0] = x;
				i[1] = y;

				return i;
			}
		}
		else {
			if ((IsWalkable((x + CELL_SIZE), (y + dy)) && !IsWalkable((x + CELL_SIZE), y)) ||
				(IsWalkable((x - CELL_SIZE), (y + dy)) && !IsWalkable((x - CELL_SIZE), y))) {
				int *i = (int *)malloc(2 * sizeof(int));
				malloc_count++;         /* [ Malloc Count ] */
				i[0] = x;
				i[1] = y;

				return i;
			}
		}
	}

	// 잘못된 점프노드가 생성되는 부분을 해결한 곳으로 추정되는 코드
	// 이미 존재하는 좌표라면 중복체크하여 해제해야 한다.
	if (dx != 0 && dy != 0) {
		jx = _jump((x + dx), y, x, y);
		jy = _jump(x, (y + dy), x, y);

		if (jx || jy) {
			int *i;

			if (jx) {
				free(jx);
				malloc_count--; /* [ Malloc Count ] */
			}
			if (jy) {
				free(jy);
				malloc_count--; /* [ Malloc Count ] */
			}

			i = (int *)malloc(2 * sizeof(int));
			malloc_count++; /* [ Malloc Count ] */
			i[0] = x;
			i[1] = y;

			return i;
		}
	}

	// 갈수 있는 곳인지를 체크하여 점프를 재귀 호출해야 한다.
	if (IsWalkable((x + dx), y) || IsWalkable(x, (y + dy))) {
		return _jump((x + dx), (y + dy), x, y);
	}
	else {
		return NULL;
	}

}

bool IsWalkable(int x, int y)
{
	int posX = x / CELL_SIZE;
	int posY = y / CELL_SIZE;

	// 음수가 들어온 경우에는 0 처리
	if (posX < 0)
	{
		posX = 0;
	}

	if (posY < 0)
	{
		posY = 0;
	}

	// 벽인가?
	if (g_Map[posY][posX].IsWall == true)
	{
		return false;
	}

	// 맵을 벗어 났는가? (가로)
	if (x < 0 || x > WINDOW_WIDTH)
	{
		return false;
	}

	// 맵을 벗어 났는가? (높이)
	if (y < 0 || y > WINDOW_HIGHT)
	{
		return false;
	}

	return true;
}

bool CheckEndNode(int x, int y)
{
	int posX = x / CELL_SIZE;
	int posY = y / CELL_SIZE;

	if (posX < 0)
	{
		posX = 0;
	}

	if (posY < 0)
	{
		posY = 0;
	}

	// EndPoint 인가?
	if (g_Map[posY][posX].IsEndPoint == true)
	{
		return true;
	}

	return false;
}

bool CheckStartNode(int x, int y)
{
	int posX = x / CELL_SIZE;
	int posY = y / CELL_SIZE;

	if (posX < 0)
	{
		posX = 0;
	}

	if (posY < 0)
	{
		posY = 0;
	}

	// EndPoint 인가?
	if (g_Map[posY][posX].IsStartPoint == true)
	{
		return true;
	}

	return false;
}

void GetLine(int startX, int startY, int endX, int endY)
{
	OptimizeFlag = (OptimizeFlag) ? false : true;
}

void GetTile(int startX, int startY, int endX, int endY)
{
	DrawTileFlag = (DrawTileFlag) ? false : true;
}