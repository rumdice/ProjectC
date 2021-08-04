// stdafx.h : 자주 사용하지만 자주 변경되지는 않는
// 표준 시스템 포함 파일 또는 프로젝트 관련 포함 파일이
// 들어 있는 포함 파일입니다.
//

#pragma once

#include "targetver.h"

#define WIN32_LEAN_AND_MEAN             // 거의 사용되지 않는 내용은 Windows 헤더에서 제외합니다.
#define _WINSOCK_DEPRECATED_NO_WARNINGS
#define _CRT_SECURE_NO_WARNINGS

#include <windows.h>

#include <stdlib.h>
#include <malloc.h>
#include <memory.h>
#include <tchar.h>


#include <conio.h>
#include <windowsx.h>
#include <list>
#include <winsock2.h>
#include <assert.h>

#pragma comment (lib, "winmm.lib")	
#pragma comment (lib, "Imm32.lib")			// ImmAssociateContext 링크에러 수정하기 위해 lib 추가
#pragma comment (lib, "ws2_32.lib")

#include <mmsystem.h>					

using namespace std;
