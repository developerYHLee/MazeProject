#include "Board.h"

Board::Board(int size)
{
	_size = size;
	
	srand((unsigned int)time(NULL));
	Initialize();

	cout << endl;
	Draw(false);
}

void Board::Initialize()
{
	endRow = -1;
	endCol = -1;
	_count = 0;

	dx = new int[4]{ -1, 1, 0, 0 };
	dy = new int[4]{ 0, 0, -1, 1 };

	_wall = new bool* [_size];
	for (int i = 0; i < _size; i++) {
		_wall[i] = new bool[_size];
		fill_n(_wall[i], _size, true);

		if (i % 2 == 0) continue;

		for (int j = 1; j < _size; j += 2) _wall[i][j] = WallType::Road;
	}

	for (int i = 1; i < _size; i += 2) {
		for (int j = 1; j < _size; j += 2) {
			_visRoad[Pos(i, j)] = false;
			_minCost[Pos(i, j)] = INT_MAX;
		}
	}

	makeEdge();
	makeRoad();

	setStartPos();
	setExit();
}

void Board::makeEdge()
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

void Board::makeRoad()
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
		_wall[row][col] = WallType::Road;

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

void Board::setExit() {
	while (true) {
		endRow = getRand(_size);
		endCol = getRand(_size);

		if (_wall[endRow][endCol] == WallType::Road && (_count = bfs(0, 0)) > _size) break;
	}
}

//�̷� ���(���� ��ġ)
void Board::Draw(bool empty, int curRow, int curCol)
{
	for (int i = 0; i < _size; i++) {
		for (int j = 0; j < _size; j++) {
			if (i == endRow && j == endCol) {
				print_exit();
				continue;
			}
			if (!empty && i == curRow && j == curCol) {
				print_player();
				continue;
			}

			if (_wall[i][j] == WallType::Wall) print_wall();
			else cout << "  ";
		}
		cout << endl;
	}
	
	if (!empty) {
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 14);
		
		cout << "(���� ��ġ : ";
		print_player(14);
		
		cout << ", �ⱸ : ";
		print_exit(14);
		
		cout << ", �� : ";
		print_wall(14);

		cout << ")\n";

		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 15);
	}
	cout << "\n";
}

int Board::getSize() { return _size; }
bool** Board::getWall() { return _wall; }
int Board::getEndRow() { return endRow; }
int Board::getEndCol() { return endCol; }
int Board::getBFSCount() { return _count; }

//�ִ� �Ÿ� Ȯ��
//�Ű�����(���� ��, ���� ��, �ֿܼ� ��� ����)
int Board::bfs(int startRow, int startCol, bool printDis) {
	bool** vis = new bool* [_size];
	for (int i = 0; i < _size; i++) {
		vis[i] = new bool[_size] {};
	}

	queue<Path> Q;
	Q.push(Path(startRow, startCol, 0));
	vis[startRow][startCol] = true;

	while (!Q.empty()) {
		Path cur = Q.front();
		Q.pop();

		if (cur._row == endRow && cur._col == endCol) {
			if (printDis) cout << "\n�ִ� �Ÿ� : " << cur._vis << "\n\n";
			return cur._vis;
		}

		for (int i = 0; i < 4; i++) {
			int row = cur._row + dx[i];
			int col = cur._col + dy[i];

			if (row < 0 || col < 0 || row >= _size || col >= _size || vis[row][col] || _wall[row][col] == WallType::Wall) continue;

			Q.push(Path(row, col, cur._vis + 1));
			vis[row][col] = true;
		}
	}
	cout << "��ǥ �������� �� �� �����ϴ�.\n\n";

	for (int i = 0; i < _size; i++) delete[] vis[i];
	delete[] vis;

	return -1;
}

//�ִ� ��� Ȯ��
//�Ű� ����(�ڵ� Ž�� ����, ��, ��, �˻� Ƚ��, ó�� ��, ó�� ��, ó�� �˻� Ƚ��)
//ó�� ��� ó�� ���� ó�� �˻� Ƚ���� ���� �߰��� �ִ� ��θ� Ȯ���ϱ� ���� �Ű� ����
bool Board::search(bool isAI, int row, int col, int count, int startRow, int startCol, int startCount)
{
	if (row == endRow && col == endCol) {
		//autoPath�� �̿��� ��� ���
		if(!isAI) findPath(startRow, startCol, startCount);
		return true;
	}

	if (_count <= count) return false;
	_vis[row][col] = true;

	for (int i = 0; i < 4; i++) {
		int r = row + dx[i];
		int c = col + dy[i];

		if (r < 0 || c < 0 || r >= _size || c >= _size || _vis[r][c] || _wall[r][c] == WallType::Wall) continue;

		autoPath[count][0] = r;
		autoPath[count][1] = c;
		if (search(isAI, r, c, count + 1, startRow, startCol, startCount)) return true;
	}

	return false;
}

void Board::AIEscape()
{
	ini_for_search();
	search(true);
	system("cls");

	//Ŀ�� �����
	CONSOLE_CURSOR_INFO cursor_info = { 1, };
	cursor_info.bVisible = FALSE;
	SetConsoleCursorInfo(GetStdHandle(STD_OUTPUT_HANDLE), &cursor_info);

	Draw(true);
	Sleep(AI_MOVE_SECOND * 8); //1�� ����

	COORD cursor;
	cursor.X = 0; //���� ��
	cursor.Y = 0; //���� ��
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), cursor);
	print_player(11);
	
	Sleep(AI_MOVE_SECOND);

	int oldRow = 0, oldCol = 0;
	//AI Ż�� ����
	for (int i = 0; i < _count; i++) {
		cursor.X = oldCol; //������ ��
		cursor.Y = oldRow; //������ ��
		SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), cursor);
		cout << "  ";

		cursor.X = autoPath[i][1] * 2; //���� �׷��� ��
		cursor.Y = autoPath[i][0]; //���� �׷��� ��
		SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), cursor);
		print_player();

		oldRow = autoPath[i][0];
		oldCol = autoPath[i][1] * 2;

		Sleep(AI_MOVE_SECOND);
	}
	Sleep(AI_MOVE_SECOND * 4);

	//�ٽ� �ⱸ �׸���
	Sleep(AI_MOVE_SECOND);
	cursor.X = endCol * 2;
	cursor.Y = endRow;
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), cursor);
	print_exit();

	//Ŀ�� ��ġ �̷� �Ʒ��� �̵�
	cursor.X = 0;
	cursor.Y = _size + 1;
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), cursor);

	Sleep(AI_MOVE_SECOND); cout << "***";
	Sleep(AI_MOVE_SECOND); print_color_str(" Ż", 12);
	Sleep(AI_MOVE_SECOND); print_color_str(" ��", 14);
	Sleep(AI_MOVE_SECOND); print_color_str(" ��", 10);
	Sleep(AI_MOVE_SECOND); print_color_str(" �� ", 9);
	Sleep(AI_MOVE_SECOND); cout << "***\n\n";
	Sleep(AI_MOVE_SECOND); cout << "3�� �� ���� ȭ������ �̵�";
	Sleep(AI_MOVE_SECOND * 24);

	system("cls");

	//Ŀ�� ��Ÿ����
	cursor_info.bVisible = TRUE;
	SetConsoleCursorInfo(GetStdHandle(STD_OUTPUT_HANDLE), &cursor_info);
}

//���� ��ġ ����
void Board::setStartPos() {
	_wall[0][0] = WallType::Road;
	_wall[0][1] = WallType::Road;
}

//�ִ� ��� ���
void Board::findPath(int pathRow, int pathCol, int cur_count)
{
	cout << "\n[��� ã��]\n";

	for (int i = cur_count; i < _count; i++) {
		int curRow = autoPath[i][0], curCol = autoPath[i][1];
		
		if (curRow == pathRow - 1) cout << "��";
		else if (curRow == pathRow + 1) cout << "��";
		else if (curCol == pathCol - 1) cout << "��";
		else if (curCol == pathCol + 1) cout << "��";

		pathRow = curRow;
		pathCol = curCol;
	}

	cout << "\n\n";
}

//�湮 ǥ�� �ʱ�ȭ
void Board::ini_vis()
{
	_vis = new bool* [_size];
	for (int i = 0; i < _size; i++) {
		_vis[i] = new bool[_size] {};
	}
}

//����� ��� �ʱ�ȭ
void Board::ini_autoPath()
{
	autoPath = new int* [_count];
	for (int i = 0; i < _count; i++) {
		autoPath[i] = new int[2]{ 0,0 };
	}
}

int Board::getRand(int r)
{
	return rand() % r;
}

//��� Ž���� ���� ����Ǿ� �ִ� ���� �ʱ�ȭ
void Board::ini_for_search()
{
	ini_vis();
	ini_autoPath();
}

//�÷��̾� �׸���
void Board::print_player(int after_color)
{
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 11);
	cout << "��";
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), after_color);
}

//�ⱸ �׸���
void Board::print_exit(int after_color)
{
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 10);
	cout << "��";
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), after_color);
}

void Board::print_wall(int after_color) {
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 12);
	cout << "��";
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), after_color);
}

//�� �ִ� ���� ���
void Board::print_color_str(string str, int color, int after_color) {
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), color);
	cout << str;
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), after_color);
}

Board::~Board() {
	for (int i = 0; i < _count; i++) delete[] autoPath[i];
	delete[] autoPath;

	for (int i = 0; i < _size; i++) delete[] _wall[i];
	delete[] _wall;

	for (int i = 0; i < _size; i++) delete[] _vis[i];
	delete[] _vis;
}
