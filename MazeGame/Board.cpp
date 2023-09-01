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
	
	cout << endl;
	Draw(false);
}

void Board::Initialize()
{
	srand((unsigned int)time(NULL));

	if (_size % 2 == 0) {
		cout << "홀수를 입력해주세요.\n\n";
		return;
	}

	//isWall 배열 false로 초기화
	isWall = new bool* [_size];
	for (int i = 0; i < _size; i++) {
		isWall[i] = new bool[_size] {};
	}

	//행이나 열이 짝수면 벽으로 설정
	for (int i = 0; i < _size; i++) {
		for (int j = 0; j < _size; j++) {
			if (i % 2 == 0 || j % 2 == 0) isWall[i][j] = true;
		}
	}

	//SideWinder 미로 생성 알고리즘
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

//미로 출력(현재 위치)
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

			if (isWall[i][j]) print_wall();
			else cout << "  ";
		}
		cout << endl;
	}
	
	if (!empty) {
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 14);
		
		cout << "(현재 위치 : ";
		print_player(14);
		
		cout << ", 출구 : ";
		print_exit(14);
		
		cout << ", 벽 : ";
		print_wall(14);

		cout << ")\n";

		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 15);
	}
	cout << "\n";
}

int Board::getSize() { return _size; }
bool** Board::getIsWall() { return isWall; }
int Board::getEndRow() { return endRow; }
int Board::getEndCol() { return endCol; }
int Board::getBFSCount() { return _count; }

//최단 거리 확인
//매개변수(시작 행, 시작 열, 콘솔에 출력 여부)
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
			if (printDis) cout << "\n최단 거리 : " << cur._vis << "\n\n";
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
	cout << "목표 지점까지 갈 수 없습니다.\n\n";

	for (int i = 0; i < _size; i++) delete[] vis[i];
	delete[] vis;

	return -1;
}

//최단 경로 확인
//매개 변수(행, 열, 검색 횟수, 처음 행, 처음 열, 처음 검색 횟수)
//처음 행과 처음 열과 처음 검색 횟수는 게임 중간에 최단 경로를 확인하기 위한 매개 변수
bool Board::search(bool isAI, int row, int col, int count, int startRow, int startCol, int startCount)
{
	if (row == endRow && col == endCol) {
		//autoPath를 이용한 경로 출력
		if(!isAI) findPath(startRow, startCol, startCount);
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
		if (search(isAI, r, c, count + 1, startRow, startCol, startCount)) return true;
	}

	return false;
}

void Board::AIEscape()
{
	ini_for_search();
	search(true);
	system("cls");

	//커서 지우기
	CONSOLE_CURSOR_INFO cursor_info = { 1, };
	cursor_info.bVisible = FALSE;
	SetConsoleCursorInfo(GetStdHandle(STD_OUTPUT_HANDLE), &cursor_info);

	Draw(true);
	Sleep(AI_MOVE_SECOND * 8); //1초 정지

	COORD cursor;
	cursor.X = 0; //시작 열
	cursor.Y = 0; //시작 행
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), cursor);
	print_player(11);
	
	Sleep(AI_MOVE_SECOND);

	int oldRow = 0, oldCol = 0;
	//AI 탈출 시작
	for (int i = 0; i < _count; i++) {
		cursor.X = oldCol; //지워질 열
		cursor.Y = oldRow; //지워질 행
		SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), cursor);
		cout << "  ";

		cursor.X = autoPath[i][1] * 2; //새로 그려질 열
		cursor.Y = autoPath[i][0]; //새로 그려질 행
		SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), cursor);
		print_player();

		oldRow = autoPath[i][0];
		oldCol = autoPath[i][1] * 2;

		Sleep(AI_MOVE_SECOND);
	}
	Sleep(AI_MOVE_SECOND * 4);

	//다시 출구 그리기
	Sleep(AI_MOVE_SECOND);
	cursor.X = endCol * 2;
	cursor.Y = endRow;
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), cursor);
	print_exit();

	//커서 위치 미로 아래로 이동
	cursor.X = 0;
	cursor.Y = _size + 1;
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), cursor);

	Sleep(AI_MOVE_SECOND); cout << "***";
	Sleep(AI_MOVE_SECOND); print_color_str(" 탈", 12);
	Sleep(AI_MOVE_SECOND); print_color_str(" 출", 14);
	Sleep(AI_MOVE_SECOND); print_color_str(" 성", 10);
	Sleep(AI_MOVE_SECOND); print_color_str(" 공 ", 9);
	Sleep(AI_MOVE_SECOND); cout << "***\n\n";
	Sleep(AI_MOVE_SECOND); cout << "3초 뒤 시작 화면으로 이동";
	Sleep(AI_MOVE_SECOND * 24);

	system("cls");

	//커서 나타내기
	cursor_info.bVisible = TRUE;
	SetConsoleCursorInfo(GetStdHandle(STD_OUTPUT_HANDLE), &cursor_info);
}

//시작 위치 설정
void Board::setStart() {
	isWall[0][0] = false;
	isWall[0][1] = false;
}

//최단 경로 출력
void Board::findPath(int pathRow, int pathCol, int cur_count)
{
	cout << "\n[경로 찾기]\n";

	for (int i = cur_count; i < _count; i++) {
		int curRow = autoPath[i][0], curCol = autoPath[i][1];
		
		if (curRow == pathRow - 1) cout << "↑";
		else if (curRow == pathRow + 1) cout << "↓";
		else if (curCol == pathCol - 1) cout << "←";
		else if (curCol == pathCol + 1) cout << "→";

		pathRow = curRow;
		pathCol = curCol;
	}

	cout << "\n\n";
}

//방문 표시 초기화
void Board::ini_vis()
{
	_vis = new bool* [_size];
	for (int i = 0; i < _size; i++) {
		_vis[i] = new bool[_size] {};
	}
}

//저장된 경로 초기화
void Board::ini_autoPath()
{
	autoPath = new int* [_count];
	for (int i = 0; i < _count; i++) {
		autoPath[i] = new int[2]{ 0,0 };
	}
}

//경로 탐색을 위해 저장되어 있는 값들 초기화
void Board::ini_for_search()
{
	ini_vis();
	ini_autoPath();
}

//플레이어 그리기
void Board::print_player(int after_color)
{
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 11);
	cout << "◈";
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), after_color);
}

//출구 그리기
void Board::print_exit(int after_color)
{
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 10);
	cout << "□";
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), after_color);
}

void Board::print_wall(int after_color) {
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 12);
	cout << "■";
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), after_color);
}

//색 있는 글자 출력
void Board::print_color_str(string str, int color, int after_color) {
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), color);
	cout << str;
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), after_color);
}

Board::~Board() {
	for (int i = 0; i < _count; i++) delete[] autoPath[i];
	delete[] autoPath;

	for (int i = 0; i < _size; i++) delete[] isWall[i];
	delete[] isWall;

	for (int i = 0; i < _size; i++) delete[] _vis[i];
	delete[] _vis;
}
