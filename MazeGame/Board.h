#pragma once
#ifndef __BOARD_H__
#define __BOARD_H__
#include <iostream>
#include <cstdlib>
#include <ctime>
#include <queue>
#include <vector>

using namespace std;

class Board
{
private:
	int* dx, * dy;
	struct Path {
		int _row, _col, _vis;

		Path(int row, int col, int vis) {
			_row = row;
			_col = col;
			_vis = vis;
		}
	};

	int _size, endRow, endCol, _count;
	bool** isWall, ** _vis;
	int** autoPath;

	void setStart();
	void findPath(int, int, int);
public:
	Board(int size);

	void Initialize();
	void Draw();
	void Draw(int, int);
	int getSize();
	bool** getIsWall();
	int getEndRow();
	int getEndCol();
	int getBFSCount();
	int bfs(int row, int col, bool printDis = false);
	bool search(int row = 0, int col = 0, int count = 0, int startRow = 0, int startCol = 0, int startCount = 0);
	~Board();

private:
	void ini_vis();
	void ini_autoPath();
};

#endif // !__BOARD_H__