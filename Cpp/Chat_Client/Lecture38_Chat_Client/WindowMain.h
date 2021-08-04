#pragma once

#include "Define.h"

extern HWND			g_hWndLobby;
extern HINSTANCE	g_hInstance;
extern BOOL			g_bActiveApp;

//HINSTANCE hInst;                                // 현재 인스턴스입니다.
WCHAR szTitle[MAX_LOADSTRING];                  // 제목 표시줄 텍스트입니다.
WCHAR szWindowClass[MAX_LOADSTRING];            // 기본 창 클래스 이름입니다.

ATOM                MyRegisterClass(HINSTANCE hInstance);
BOOL                InitInstance(HINSTANCE, int);
LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);
BOOL CALLBACK DialogProc(HWND hWnd, UINT iMsg, WPARAM wParam, LPARAM lParam);
BOOL CALLBACK LobbyDialogProc(HWND hWnd, UINT iMsg, WPARAM wParam, LPARAM lParam);
BOOL CALLBACK RoomDialogProc(HWND hWnd, UINT iMsg, WPARAM wParam, LPARAM lParam);

wchar_t		g_szIP[16];
wchar_t		g_szPORT[5];
wchar_t		g_userName[15];
DWORD		g_userID = 0;
