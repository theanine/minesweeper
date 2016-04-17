#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#define GRID_WIDTH_SIZE   10
#define GRID_HEIGHT_SIZE  10
#define BOMB_PERCENT      10     // in percentage of entire grid (truncated)
#define BOMB_COUNT        (GRID_WIDTH_SIZE * GRID_HEIGHT_SIZE * BOMB_PERCENT / 100)
	
#define SUCCESS           0
#define FAILURE           -1

typedef struct cell {
	bool flagged;
	bool revealed;
	bool bomb;
	int bomb_count;
} cell_t;

cell_t sweeper_grid[GRID_WIDTH_SIZE][GRID_HEIGHT_SIZE];
int bombs_flagged;
int flag_count;
int revealed_cells;
bool lost;

typedef enum mouseevent {
	MOUSEEVENT_LEFT,
	MOUSEEVENT_RIGHT,
} mouseevent_t;

void init_grid(void)
{
	srand(time(NULL));
	memset(sweeper_grid, 0, sizeof(sweeper_grid));
	lost           = false;
	bombs_flagged  = 0;
	flag_count     = 0;
	revealed_cells = 0;
}

void get_mousepos(int* x, int* y)
{
	// returns (x,y) grid coordinates
}

mouseevent_t get_mouseevent(void)
{
	// returns mouse event somehow
}

int place_bomb(int x, int y)
{
	if (sweeper_grid[x][y].bomb)
		return FAILURE;
	sweeper_grid[x][y].bomb = true;
	
	for (int i=x-1; i<=x+1; i++) {
		for (int j=y-1; j<=y+1; j++) {
			if (i < 0 || i >= GRID_WIDTH_SIZE)
				continue;
			if (j < 0 || j >= GRID_HEIGHT_SIZE)
				continue;
			sweeper_grid[i][j].bomb_count++;
		}
	}
	
	return SUCCESS;
}

void toggle_flag(int x, int y)
{
	sweeper_grid[x][y].flagged = !sweeper_grid[x][y].flagged;
	
	if (sweeper_grid[x][y].flagged)
		flag_count++;
	else
		flag_count--;
	
	if (sweeper_grid[x][y].bomb) {
		if (sweeper_grid[x][y].flagged)
			bombs_flagged++;
		else
			bombs_flagged--;
	}
}

void generate_grid(void)
{
	for (int i=0; i<BOMB_COUNT; i++) {
		while (1) {
			int x = rand() % GRID_WIDTH_SIZE;
			int y = rand() % GRID_HEIGHT_SIZE;
			
			if (place_bomb(x, y) == SUCCESS)
				break;
		}
	}
}

void reveal_square(int x, int y)
{
	if (!sweeper_grid[x][y].revealed) {
		sweeper_grid[x][y].revealed = true;
		revealed_cells++;
	}
	
	if (sweeper_grid[x][y].bomb) {
		lost = true;
		return;
	}
	
	if (sweeper_grid[x][y].bomb_count == 0) {
		for (int i=x-1; i<=x+1; i++) {
			for (int j=y-1; j<=y+1; j++) {
				if (i < 0 || i >= GRID_WIDTH_SIZE)
					continue;
				if (j < 0 || j >= GRID_HEIGHT_SIZE)
					continue;
				if (!sweeper_grid[i][j].revealed)
					reveal_square(i, j);
			}
		}
	}
}

void handle_leftclick(int x, int y)
{
	reveal_square(x, y);
}

void handle_rightclick(int x, int y)
{
	toggle_flag(x, y);
}

void determine_win(void)
{
	if (lost)
		graphics_lost();
	
	if (flag_count == BOMB_COUNT && bombs_flagged == BOMB_COUNT)
		graphics_win();
	
	if (revealed_cells == (GRID_WIDTH_SIZE * GRID_HEIGHT_SIZE) - BOMB_COUNT)
		graphics_win();
}

void main(void)
{
	init_grid();
	generate_grid();
	
	while (1) {
		mouseevent_t event = get_mouseevent();
		
		int x, y;
		get_mousepos(&x, &y);
		
		switch (event) {
			case MOUSEEVENT_LEFT:
				handle_leftclick(x, y);
				break;
			case MOUSEEVENT_RIGHT:
				handle_rightclick(x, y);
				break;
			default:
				printf("Unhandled mouse event.\n");
				assert(false);
		}
		
		determine_win();
	}
}