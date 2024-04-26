#pragma once
#ifndef __TESTBOARD_H__
#define __TESTBOARD_H__
#define TestRandomMod 100

#include <iostream>
#include <ctime>
#include <queue>
#include <map>
#include <climits>
#include <vector>

using namespace std;

class TestBoard {
	enum RoadType
	{
		Wall,
		Road
	};

	struct Pos {
		int row, col;

		Pos() { row = -1, col = -1; }
		Pos(int row, int col) : row(row), col(col) {}

		bool operator<(Pos o) const {
			if (row == o.row) return col > o.col;
			return row > o.row;
		}
	};

	struct EdgeInfo
	{
		Pos pos;
		int cost;

		EdgeInfo(Pos pos, int cost) : pos(pos), cost(cost) {}

		bool operator<(EdgeInfo o) const { return cost > o.cost; }
	};

private:
	int _size;
	bool** _road;
	map<Pos, bool> _visRoad;
	map<Pos, int> _minCost;
	map<Pos, vector<EdgeInfo>> _edge;

	void Initialize();
	void makeEdge();
	void makeRoad();
	int getRand(int r = TestRandomMod);

public:
	TestBoard(int);
	~TestBoard();
	void Draw();
};

#endif __TESTBOARD_H__