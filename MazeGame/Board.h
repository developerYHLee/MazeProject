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
	void findPath();
public:
	Board(int size);

	void Initialize();
	void Draw();
	void Draw(int, int);
	int getSize();
	bool** getIsWall();
	int getEndRow();
	int getEndCol();
	int bfs(bool printDis);
	bool search(int row, int col, int count);
	~Board();
};

#endif // !__BOARD_H__