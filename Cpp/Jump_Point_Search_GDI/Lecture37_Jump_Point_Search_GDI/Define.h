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

#define CELL_SIZE				32				// ��ĭ�� 32 * 32 ¥���� �׸� (�ȼ�����)	
#define WINDOW_WIDTH			1440			// ������ ����
#define WINDOW_HIGHT			900				// ������ ����
#define MAX_BLOCK_COUNT_WIDTH	44				// = 1440 / 32 (ȭ�� ���� ��ü �����ȼ� / �� ��ĭ �Ȼ�)
#define MAX_BLOCK_COUNT_HIGHT	26				// = 900 / 32  (ȭ�� ���� ��ü �����ȼ� / �� ��ĭ �Ȼ�)

#define DEFAULT_START_POS_X		12				// ���½� ��ŸƮ x��ǥ
#define DEFAULT_START_POS_Y		12				// ���½� ��ŸƮ y��ǥ

#define DEFAULT_END_POS_X		24				// ���½� ��ŸƮ x��ǥ
#define DEFAULT_END_POS_Y		12				// ���½� ��ŸƮ y��ǥ

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

	// ����Ʈ ����ü ������
	NODE()
	{

	}
	// ����ü ������.
	NODE(int y, int x)
	{
		posY = y;
		posX = x;
	}

	// �߰� �Ӽ���
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
