#include "Board.h"

Board::Board(int size)
{
	dx = new int[4]{ -1, 1, 0, 0 };
	dy = new int[4]{ 0, 0, -1, 1 };
	_size = size;
	endRow = -1;
	endCol = -1;
	_count = 0;
	Initialize();

	ini_autoPath();
	ini_vis();
	Draw();
}

void Board::Initialize()
{
	srand((unsigned int)time(NULL));

	if (_size % 2 == 0) {
		cout << "Ȧ���� �Է����ּ���.\n\n";
		return;
	}

	//isWall �迭 false�� �ʱ�ȭ
	isWall = new bool* [_size];
	for (int i = 0; i < _size; i++) {
		isWall[i] = new bool[_size] {};
	}

	//���̳� ���� ¦���� ������ ����
	for (int i = 0; i < _size; i++) {
		for (int j = 0; j < _size; j++) {
			if (i % 2 == 0 || j % 2 == 0) isWall[i][j] = true;
		}
	}

	//SideWinder �̷� ���� �˰���
	for (int i = 0; i < _size; i++) {
		int count = 1;
		for (int j = 0; j < _size; j++) {
			if (i % 2 == 0 || j % 2 == 0) continue;
			if (i == _size - 2 && j == _size - 2) continue;

			if (i == _size - 2) {
				isWall[i][j + 1] = false;
				continue;
			}
			if (j == _size - 2) {
				isWall[i + 1][j] = false;
				continue;
			}

			int road = rand() % 2;
			if (road == 0) {
				isWall[i][j + 1] = false;
				count++;
			}
			else {
				int random = rand() % count;
				isWall[i + 1][j - random * 2] = false;
				count = 1;
			}
		}
	}

	setStart();
	while (true) {
		endRow = rand() % _size;
		endCol = rand() % _size;

		if (!isWall[endRow][endCol] && (_count = bfs(0, 0)) > _size) break;
	}
}

//�̷� ���(�⺻)
void Board::Draw()
{
	cout << endl;

	for (int i = 0; i < _size; i++) {
		for (int j = 0; j < _size; j++) {
			if (i == endRow && j == endCol) {
				cout << "��";
				continue;
			}
			if (isWall[i][j]) cout << "��";
			else cout << "  ";
		}
		cout << endl;
	}
	cout << "\n";
}

//�̷� ���(���� ��ġ)
void Board::Draw(int curRow, int curCol)
{
	cout << endl;

	for (int i = 0; i < _size; i++) {
		for (int j = 0; j < _size; j++) {
			if (i == endRow && j == endCol) {
				cout << "��";
				continue;
			}
			if (i == curRow && j == curCol) {
				cout << "��";
				continue;
			}
			if (isWall[i][j]) cout << "��";
			else cout << "  ";
		}
		cout << endl;
	}
	cout << "\n";
}

int Board::getSize() { return _size; }
bool** Board::getIsWall() { return isWall; }
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

			if (row < 0 || col < 0 || row >= _size || col >= _size || vis[row][col] || isWall[row][col]) continue;

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
//�Ű� ����(��, ��, �˻� Ƚ��, ó�� ��, ó�� ��, ó�� �˻� Ƚ��)
//ó�� ��� ó�� ���� ó�� �˻� Ƚ���� ���� �߰��� �ִ� ��θ� Ȯ���ϱ� ���� �Ű� ����
bool Board::search(int row, int col, int count, int startRow, int startCol, int startCount)
{
	if (row == endRow && col == endCol) {
		//autoPath�� �̿��� ��� ���
		findPath(startRow, startCol, startCount);
		return true;
	}

	if (_count <= count) return false;
	_vis[row][col] = true;

	for (int i = 0; i < 4; i++) {
		int r = row + dx[i];
		int c = col + dy[i];

		if (r < 0 || c < 0 || r >= _size || c >= _size || isWall[r][c] || _vis[r][c]) continue;

		autoPath[count][0] = r;
		autoPath[count][1] = c;
		if (search(r, c, count + 1, startRow, startCol, startCount)) return true;
	}

	return false;
}

//���� ��ġ ����
void Board::setStart() {
	isWall[0][0] = false;
	isWall[0][1] = false;
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

	ini_autoPath();
	ini_vis();
}

Board::~Board() {
	for (int i = 0; i < _count; i++) delete[] autoPath[i];
	delete[] autoPath;

	for (int i = 0; i < _size; i++) delete[] isWall[i];
	delete[] isWall;

	for (int i = 0; i < _size; i++) delete[] _vis[i];
	delete[] _vis;
}

void Board::ini_vis()
{
	_vis = new bool* [_size];
	for (int i = 0; i < _size; i++) {
		_vis[i] = new bool[_size] {};
	}
}

void Board::ini_autoPath()
{
	autoPath = new int* [_count];
	for (int i = 0; i < _count; i++) {
		autoPath[i] = new int[2]{ 0,0 };
	}
}