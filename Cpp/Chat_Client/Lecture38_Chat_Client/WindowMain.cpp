#include "stdafx.h"
#include "WindowMain.h"
#include "resource.h"
#include "Network.h"
#include "Contents.h"

HWND		g_hWndLobby = NULL;
HWND		g_hWndRoom = NULL;
HINSTANCE	g_hInstance = NULL;


int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
	_In_opt_ HINSTANCE hPrevInstance,
	_In_ LPWSTR    lpCmdLine,
	_In_ int       nCmdShow)
{
	UNREFERENCED_PARAMETER(hPrevInstance);
	UNREFERENCED_PARAMETER(lpCmdLine);

	DialogBox(hInstance, MAKEINTRESOURCE(IDD_LOGIN), NULL, DialogProc);

	LoadStringW(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
	LoadStringW(hInstance, IDC_LECTURE38CHATCLIENT, szWindowClass, MAX_LOADSTRING);


	g_hWndLobby = CreateDialog(g_hInstance, MAKEINTRESOURCE(IDD_CHAT_LOBBY), NULL, LobbyDialogProc);
	ShowWindow(g_hWndLobby, nCmdShow);

	NetworkInit();
	
	NetworkPortOpen(g_hWndLobby, g_szIP, g_szPORT);

	MSG msg;

	BOOL bRet;
	while (bRet = GetMessage(&msg, nullptr, 0, 0) != 0)
	{
		if (bRet == -1)
		{
			break;
		}

		bRet = false;

		if (IsWindow(g_hWndLobby))
		{
			bRet |= IsDialogMessage(g_hWndLobby, &msg);
			OutputDebugString(L"Lobby Proc\n");
		}

		if (IsWindow(g_hWndRoom))
		{
			bRet |= IsDialogMessage(g_hWndRoom, &msg);
			OutputDebugString(L"Room Proc\n");
		}

		if (!bRet)
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}

	}

	NetworkRelease();

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
	wcex.hIcon = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_LECTURE38CHATCLIENT));
	wcex.hCursor = LoadCursor(nullptr, IDC_ARROW);
	wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
	wcex.lpszMenuName = MAKEINTRESOURCEW(IDC_LECTURE38CHATCLIENT);
	wcex.lpszClassName = szWindowClass;
	wcex.hIconSm = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

	return RegisterClassExW(&wcex);
}


BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
	g_hInstance = hInstance;

	g_hWndLobby = CreateWindowW(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT, 0, CW_USEDEFAULT, 0, nullptr, nullptr, hInstance, nullptr);

	if (!g_hWndLobby)
	{
		return FALSE;
	}

	ShowWindow(g_hWndLobby, nCmdShow);
	UpdateWindow(g_hWndLobby);

	return TRUE;
}


LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
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
		default:
			return DefWindowProc(hWnd, message, wParam, lParam);
		}
	}
	break;
	case WM_PAINT:
	{
		PAINTSTRUCT ps;
		HDC hdc = BeginPaint(hWnd, &ps);
		
		EndPaint(hWnd, &ps);
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


BOOL CALLBACK DialogProc(HWND hDlg, UINT iMsg, WPARAM wParam, LPARAM lParam)
{
	static HWND hEditBox;

	switch (iMsg)
	{
	case WM_INITDIALOG:
		memset(g_szIP, 0, 16);
		memset(g_szPORT, 0, 5);
		memset(g_userName, 0, 15);

		hEditBox = GetDlgItem(hDlg, IDC_ADDRESS);
		SetWindowText(hEditBox, L"127.0.0.1");

		hEditBox = GetDlgItem(hDlg, IDC_PORT);
		SetWindowText(hEditBox, L"6000");

		hEditBox = GetDlgItem(hDlg, IDC_USERNAME);
		SetWindowText(hEditBox, L"고갱님");

		return TRUE;

	case WM_COMMAND:
		switch (wParam)
		{
		case IDCANCEL:
			EndDialog(hDlg, FALSE);
			exit(1); 
			break;
		case ID_LOGIN:

			GetDlgItemText(hDlg, IDC_ADDRESS, g_szIP, 16);
			GetDlgItemText(hDlg, IDC_PORT, g_szPORT, 5);
			GetDlgItemText(hDlg, IDC_USERNAME, g_userName, 15);

			EndDialog(hDlg, TRUE);
			return TRUE;
			break;
		}
		break;
	}
	return FALSE;
}

BOOL CALLBACK LobbyDialogProc(HWND hDlg, UINT iMsg, WPARAM wParam, LPARAM lParam)
{
	static HWND hEditBox;

	switch (iMsg)
	{
	case WM_INITDIALOG:
		hEditBox = GetDlgItem(hDlg, IDC_USER_ID);
		TCHAR str[10];
		wsprintf(str, TEXT("%d"), g_userID);
		SetWindowText(hEditBox, str);

		hEditBox = GetDlgItem(hDlg, IDC_USER_NAME);
		SetWindowText(hEditBox, g_userName);
		return TRUE;

	case WM_COMMAND:
		switch (LOWORD(wParam))
		{
		case IDCANCEL:
			EndDialog(hDlg, FALSE);
			break;
		case WM_CLOSE:
			EndDialog(hDlg, FALSE);
			
			break;
		case ID_MAKE_ROOM:
			hEditBox = GetDlgItem(hDlg, IDC_ROOM_NAME);

			TCHAR roomName[15];
			GetWindowText(hEditBox, roomName, 15);
			MakeRoom(roomName);
			break;
		case IDC_ROOM_LIST:
			switch (HIWORD(wParam))
			{
			case LBN_DBLCLK:
				g_hWndRoom = CreateDialog(g_hInstance, MAKEINTRESOURCE(IDD_CHAT_ROOM), NULL, RoomDialogProc);
				ShowWindow(g_hWndRoom, 1);
				break;
			case LBN_SELCHANGE:
				break;
			}
		}
		break;
	case WM_SOCKET:
		if (!NetworkProcess(hDlg, iMsg, wParam, lParam))
		{
			MessageBox(hDlg, L"CONNECTION CLOSE", L"DISCONNECTED", MB_OK);
			exit(1);
		}
		break;
	}
	return FALSE;
}

BOOL CALLBACK RoomDialogProc(HWND hDlg, UINT iMsg, WPARAM wParam, LPARAM lParam)
{
	static HWND hEditBox;

	switch (iMsg)
	{
	case WM_INITDIALOG:
		return TRUE;

	case WM_COMMAND:
		switch (LOWORD(wParam))
		{
		case ID_ROOM_EXIT:
			EndDialog(hDlg, TRUE);
			break;
		}
		break;
	case WM_SOCKET:
		if (!NetworkProcess(hDlg, iMsg, wParam, lParam))
		{
			MessageBox(hDlg, L"CONNECTION CLOSE", L"DISCONNECTED", MB_OK);
			exit(1);
		}
		break;
	}
	return FALSE;
}

