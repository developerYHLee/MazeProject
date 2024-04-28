#pragma once
#ifndef __TESTBOARD_H__
#define __TESTBOARD_H__
#define TestRandomMod 100

#include <iostream>
#include <ctime>
#include <queue>
#include <climits>
#include <algorithm>
#include <vector>

using namespace std;

class TestBoard {
	enum class ERoadType
	{
		Wall,
		Road
	};

	struct EdgeInfo
	{
		int row, col, cost;

		EdgeInfo(int row, int col, int cost) : row(row), col(col), cost(cost) {}

		bool operator<(const EdgeInfo& o) const { return cost > o.cost; }
	};

private:
	int _size, ** _minCost;
	bool** _road, ** _visRoad;
	vector<EdgeInfo>** _edge;

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