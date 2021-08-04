#pragma once

#define COLOR_WALL				RGB(51, 51, 51)			// dark gray 0x333333
#define COLOR_START_POINT		RGB(0, 255, 0)			// green
#define COLOR_END_POINT			RGB(255, 0, 0)			// red
#define COLOR_OPENLIST			RGB(0, 0, 255)			// blue
#define COLOR_CLOSELIST			RGB(255, 255, 0)		// yellow
#define COLOR_PATHLIST			RGB(255, 0, 255)		// pupple
#define COLOR_GRID_LINE			RGB(150, 150, 150)		// gray
#define COLOR_BROWN				RGB(139, 69, 19)		// brown 0x8B4513
#define COLOR_ORANGE			RGB(255, 153, 51)		// orange ff9933
#define COLOR_SEARCHED_LIST		RGB(204, 204, 204)		// light gray 0xcccccc

#define CELL_SIZE				32				// 한칸에 32 * 32 짜리로 그림 (픽셀단위)	
#define WINDOW_WIDTH			1440			// 윈도우 넓이
#define WINDOW_HIGHT			900				// 윈도우 높이
#define MAX_BLOCK_COUNT_WIDTH	44				// = 1440 / 32 (화면 가로 전체 고정픽셀 / 셀 한칸 픽샐)
#define MAX_BLOCK_COUNT_HIGHT	26				// = 900 / 32  (화면 세로 전체 고정픽셀 / 셀 한칸 픽샐)

#define DEFAULT_START_POS_X		12				// 리셋시 스타트 x좌표
#define DEFAULT_START_POS_Y		12				// 리셋시 스타트 y좌표

#define DEFAULT_END_POS_X		24				// 리셋시 스타트 x좌표
#define DEFAULT_END_POS_Y		12				// 리셋시 스타트 y좌표

struct NODE
{
	int posX;
	int posY;

	NODE *parent;
	int H;
	int G;
	int F;

	bool IsWall = false;
	bool IsStartPoint = false;
	bool IsEndPoint = false;

	// 디폴트 구조체 생성자
	NODE()
	{

	}
	// 구조체 생성자.
	NODE(int y, int x)
	{
		posY = y;
		posX = x;
	}

	// 추가 속성들
	bool opened = false;
	bool closed = false;
	bool walkable = false;

	bool checked = false;
};


struct open_list 
{
	struct open_list *left;
	struct open_list *right;
	NODE *list_node;
};

struct neighbor_xy_list 
{
	struct neighbor_xy_list *left;
	struct neighbor_xy_list *right;
	int x;
	int y;
};

struct neighbor_list 
{
	struct neighbor_list *left;
	struct neighbor_list *right;
	NODE* neighbor_node;
};
