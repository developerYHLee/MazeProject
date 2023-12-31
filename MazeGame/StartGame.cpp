#include "StartGame.h"

StartGame::StartGame(Board* board)
{
	_board = board;
	isWall = board->getIsWall();
	_size = board->getSize();
	endRow = board->getEndRow();
	endCol = board->getEndCol();
}

int* StartGame::move(int order, int row, int col, int vis) {
	int* res = new int[3];
	
	res[0] = row;
	res[1] = col;
	res[2] = vis;

	if (order == 0) {
		row--;
		if (row < 0 || isWall[row][col]) cout << "위로 이동할 수 없습니다. 다시 선택하세요.\n\n";
		else {
			cout << "위로 이동!\n\n";
			res[0] = row;
			res[2] = 1;
		}
	}
	else if (order == 1) {
		row++;
		if (row >= _size || isWall[row][col]) cout << "아래로 이동할 수 없습니다. 다시 선택하세요.\n\n";
		else {
			cout << "아래로 이동!\n\n";
			res[0] = row;
			res[2] = 0;
		}
	}
	else if (order == 2) {
		col--;
		if (col < 0 || isWall[row][col]) cout << "왼쪽으로 이동할 수 없습니다. 다시 선택하세요.\n\n";
		else {
			cout << "왼쪽으로 이동!\n\n";
			res[1] = col;
			res[2] = 3;
		}
	}
	else if (order == 3) {
		col++;
		if (col >= _size || isWall[row][col]) cout << "오른쪽으로 이동할 수 없습니다. 다시 선택하세요.\n\n";
		else {
			cout << "오른쪽으로 이동!\n\n";
			res[1] = col;
			res[2] = 2;
		}
	}
	else {
		cout << "이상한 값이 들어왔습니다.\n\n";
	}

	return res;
}

void StartGame::printArrow(int i) {
	if (i == 0) cout << " ↑";
	else if (i == 1) cout << " ↓";
	else if (i == 2) cout << " ←";
	else cout << " →";
}

int StartGame::playGame()
{
	int getLocate = 2, getPath = 1;
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 14); //폰트 노란색 변경
	cout << "**미로 게임 START**\n\n";
	cout << 
		"*************************\n" <<
		"*      Chance 횟수      *\n" <<
		"*   0 : 위치 알림 " << getLocate << "번   *\n" <<
		"*   1 : 경로 확인 " << getPath <<"번   *\n" <<
		"*************************\n\n";
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 15); //폰트 하얀색 변경
	
	Path* P = new Path(0, 0, 0);

	int dx[] = { -1, 1, 0, 0 }, dy[] = { 0,0,-1,1 };

	int curRow = 0, curCol = 0, vis = 0, count = 0;
	while (true) {
		cout << "이동 가능한 방향(";

		int pathCount = 0, printCount = 0;
		//이동 가능 행렬 탐색
		for (int i = 0; i < 4; i++) {
			int row = curRow + dx[i];
			int col = curCol + dy[i];

			if (row < 0 || col < 0 || row >= _size || col >= _size || isWall[row][col]) continue;

			if (printCount != 0) cout << ",";
			printArrow(i);
			printCount++;

			if (vis == i) cout << "(방금 지나온 길)";
			else pathCount++;
		}

		if (pathCount == 0) {
			if (curRow == 0 && curCol == 0) {
				cout << " ) :\n시작 지점입니다.\n\n";
				vis = 0;
				
				continue;
			}

			cout << " )\n";
			cout << "길이 막혔으니 돌아가야 합니다!\n";

			curRow = P->_row;
			curCol = P->_col;
			vis = P->_vis;

			cout << "갈림길로 돌아갔습니다.\n\n";

			continue;
		}
		else cout << " ) : ";

		int order = _input();

		//현재 위치 알림 찬스 사용 시
		if (order == 9) {
			if (getLocate == 0) {
				cout << "위치 알림 찬스를 이미 모두 사용했습니다.\n\n";
				continue;
			}
			getLocate--;

			//미로 출력(현재 위치)
			cout << endl;
			_board->Draw(false, curRow, curCol);
			cout << "현재 위치(";
			_board->print_player();
			cout << ") : " << curRow << "행 " << curCol << "열\n\n";
			cout << "남은 위치 알림 서비스 : " << getLocate << "\n엔터 시 지도 삭제";
			
			//엔터 입력 받으면 콘솔창 내용 지움
			_input(true);
			continue;
		}
		else if (order == 10) {
			if (getPath == 0) {
				cout << "경로 확인 찬스를 이미 모두 사용했습니다.\n\n";
				continue;
			}
			getPath--;


			//경로 출력을 위한 값 초기화
			_board->ini_for_search();

			//경로 출력
			int cur_count = _board->getBFSCount() - _board->bfs(curRow, curCol);
			_board->search(false, curRow, curCol, cur_count, curRow, curCol, cur_count);
			cout << "남은 경로 확인 서비스 : " << getPath << "\n엔터 시 경로 확인 종료";
			
			//엔터 입력 받으면 콘솔창 내용 지움
			_input(true);
			continue;
		}


		//갈림길이면 현재 저장되어 있는 갈림길 정보 최신화
		if (pathCount > 1) P->change(curRow, curCol, order);

		//이동
		int* path;
		path = move(order, curRow, curCol, vis);

		//길에 가로막힘
		if (curRow == path[0] && curCol == path[1]) {
			delete path;
			continue;
		}

		curRow = path[0];
		curCol = path[1];
		vis = path[2];

		count++;
		if (curRow == endRow && curCol == endCol) break;

		delete path;
	}

	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 14); //폰트 노란색 변경
	cout << "축하합니다!\n";
	cout << "미로를 빠져나왔습니다!\n";
	cout << "미로를 탈출하는데 " << count << "번 시도했습니다.\n\n";
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 15); //폰트 노란색 변경
	
	cout << "\n엔터 시 시작 화면으로 이동";
	_input(true);

	delete P;

	return count;
}

//eraseMap이 true면 엔터 시 지도 지움, false면 이동 방향 입력
int StartGame::_input(bool eraseMap)
{
	int order = -1;

	while (true) {
		char ch = _getch();

		if (eraseMap) {
			if (ch == 13 || ch == 10) {
				system("cls");
				return 1;
			}
		}

		//엔터를 입력받고 이동 명령일 때
		if (ch == 13 && order != -1) {
			cout << endl;
			break;
		}
		//백스페이스
		else if (ch == 8) {
			_input_backspace(order);
			order = -1;
		}
		//상
		else if (ch == 72) {
			_input_backspace(order);
			order = 0;
			cout << "↑";
		}
		//좌
		else if (ch == 75) {
			_input_backspace(order);
			order = 2;
			cout << "←";
		}
		//우
		else if (ch == 77) {
			_input_backspace(order);
			order = 3;
			cout << "→";
		}
		//하
		else if (ch == 80) {
			_input_backspace(order);
			order = 1;
			cout << "↓";
		}
		//현재 위치 찬스
		else if (ch == '0') {
			_input_backspace(order);
			order = 9;
			cout << "0";
		}
		//경로 찾기 찬스
		else if (ch == '1') {
			_input_backspace(order);
			order = 10;
			cout << "1";
		}
	}

	return order;
}

//콘솔창에 기존에 출력되어 있던 글자를 지움
void StartGame::_input_backspace(int order)
{
	//아무것도 입력되어 있지 않은 상태
	if (order == -1) return;

	//0이 입력됐을때
	if (order == 9 || order == 10) cout << "\b \b";
	//방향키가 입력됐을때
	else cout << "\b\b  \b\b";
}