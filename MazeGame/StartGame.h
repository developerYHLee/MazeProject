#pragma once

#ifndef __STARTGAME_H__
#define __STARTGAME_H__

#include <iostream>
#include <conio.h>
#include <Windows.h>

#include "Board.h"

using std::cout;

class StartGame
{
private:
	struct Path {
		int _row, _col, _vis;

		Path(int row, int col, int vis) {
			_row = row;
			_col = col;
			_vis = vis;
		}

		void change(int row, int col, int vis) {
			_row = row;
			_col = col;
			_vis = vis;
		}
	};

	Board* _board;
	bool** isWall;
	int _size, endRow, endCol;
	int* move(int order, int row, int col, int vis);
	int _input(bool);
	void _input_backspace(int);
	void printArrow(int i);

public:
	StartGame(Board* board);
	void playGame();
};


#endif // ! __STARTGAME_H__