#include "TestBoard.h"

TestBoard::TestBoard(int size)
{
	_size = size;

	Initialize();

	cout << "\n";
	Draw();
	cout << "\n";
}

void TestBoard::Initialize()
{
	_road = new bool* [_size];
	for (int i = 0; i < _size; i++) {
		_road[i] = new bool[_size] {RoadType::Wall};
		
		if (i % 2 == 0) continue;

		for (int j = 1; j < _size; j += 2) _road[i][j] = RoadType::Road;
	}
	
	for (int i = 1; i < _size; i += 2) {
		for (int j = 1; j < _size; j += 2) {
			_visRoad[Pos(i, j)] = false;
			_minCost[Pos(i, j)] = INT_MAX;
		}
	}
	
	makeEdge();
	makeRoad();
}

void TestBoard::makeEdge()
{
	for (int row = 1; row < _size; row += 2) {
		for (int col = 1; col < _size; col += 2) {
			if (row < _size - 2) {
				int r = getRand();
				Pos u(row, col);
				Pos v(row + 2, col);
				_edge[u].push_back(EdgeInfo(v, r));
				_edge[v].push_back(EdgeInfo(u, r));
			}

			if (col < _size - 2) {
				int r = getRand();
				Pos u(row, col);
				Pos v(row, col + 2);
				_edge[u].push_back(EdgeInfo(v, r));
				_edge[v].push_back(EdgeInfo(u, r));
			}
		}
	}
}

void TestBoard::makeRoad()
{
	priority_queue<EdgeInfo> PQ;
	PQ.push(EdgeInfo(Pos(1, 1), 0));
	_minCost[Pos(1, 1)] = 0;

	map<Pos, Pos> parent;
	parent[Pos(1, 1)] = Pos(1, 1);

	while (!PQ.empty()) {
		EdgeInfo node = PQ.top();
		PQ.pop();

		Pos curPos = node.pos;
		int curCost = node.cost;

		if (_visRoad[curPos]) continue;
		_visRoad[curPos] = true;

		int row = (parent[curPos].row + curPos.row) / 2;
		int col = (parent[curPos].col + curPos.col) / 2;
		_road[row][col] = RoadType::Road;

		for (const EdgeInfo& next : _edge[curPos]) {
			Pos nextPos = next.pos;
			int nextCost = next.cost;

			if (_visRoad[nextPos] || _minCost[nextPos] < curCost) continue;

			parent[nextPos] = curPos;
			_minCost[nextPos] = nextCost;

			PQ.push(next);
		}
	}
}

int TestBoard::getRand(int r)
{
	return rand() % r;
}

void TestBoard::Draw()
{
	for (int i = 0; i < _size; i++) {
		for (int j = 0; j < _size; j++) {
			if (!_road[i][j]) cout << "бс";
			else cout << "  ";
		}
		cout << "\n";
	}
}

TestBoard::~TestBoard()
{
	for (int i = 0; i < _size; i++) delete[] _road[i];
	delete[] _road;
}