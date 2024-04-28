#include "TestBoard.h"

TestBoard::TestBoard(int size)
{
	_size = size;

	srand((unsigned int)time(NULL));
	Initialize();

	cout << "\n";
	Draw();
	cout << "\n";
}

void TestBoard::Initialize()
{
	_road = new bool* [_size];
	_visRoad = new bool* [_size];
	_minCost = new int* [_size];
	_edge = new vector<EdgeInfo>* [_size];

	for (int i = 0; i < _size; i++) {
		_road[i] = new bool[_size] {static_cast<bool>(ERoadType::Wall)};
		_visRoad[i] = new bool[_size] {false};

		_minCost[i] = new int[_size];
		fill_n(_minCost[i], _size, INT_MAX);

		_edge[i] = new vector<EdgeInfo>[_size];

		if (i % 2 == 0) continue;

		for (int j = 1; j < _size; j += 2) _road[i][j] = static_cast<bool>(ERoadType::Road);
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

				_edge[row][col].push_back(EdgeInfo(row + 2, col, r));
				_edge[row + 2][col].push_back(EdgeInfo(row, col, r));
			}

			if (col < _size - 2) {
				int r = getRand();

				_edge[row][col].push_back(EdgeInfo(row, col + 2, r));
				_edge[row][col + 2].push_back(EdgeInfo(row, col, r));
			}
		}
	}
}

void TestBoard::makeRoad()
{
	priority_queue<EdgeInfo> PQ;
	PQ.push(EdgeInfo(1, 1, 0));
	_minCost[1][1] = 0;

	pair<int, int>** parent = new pair<int, int>*[_size];
	for (int i = 0; i < _size; i++) parent[i] = new pair<int, int>[_size];

	parent[1][1] = { 1, 1 };

	while (!PQ.empty()) {
		EdgeInfo node = PQ.top();
		PQ.pop();

		int curRow = node.row, curCol = node.col, curCost = node.cost;

		if (_visRoad[curRow][curCol]) continue;
		_visRoad[curRow][curCol] = true;

		int row = (parent[curRow][curCol].first + curRow) / 2;
		int col = (parent[curRow][curCol].second + curCol) / 2;
		_road[row][col] = static_cast<bool>(ERoadType::Road);

		for (const EdgeInfo& next : _edge[curRow][curCol]) {
			int nextRow = next.row, nextCol = next.col, nextCost = next.cost;

			if (_visRoad[nextRow][nextCol] || _minCost[nextRow][nextCol] < nextCost) continue;

			parent[nextRow][nextCol] = { curRow, curCol };
			_minCost[nextRow][nextCol] = nextCost;

			PQ.push(next);
		}
	}

	for (int i = 0; i < _size; i++) delete[] parent[i];
	delete[] parent;
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

	for (int i = 0; i < _size; i++) delete[] _visRoad[i];
	delete[] _visRoad;

	for (int i = 0; i < _size; i++) delete[] _minCost[i];
	delete[] _minCost;

	for (int i = 0; i < _size; i++) delete[] _edge[i];
	delete[] _edge;
}