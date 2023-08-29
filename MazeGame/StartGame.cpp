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
		if (row < 0 || isWall[row][col]) cout << "���� �̵��� �� �����ϴ�. �ٽ� �����ϼ���.\n\n";
		else {
			cout << "���� �̵�!\n\n";
			res[0] = row;
			res[2] = 1;
		}
	}
	else if (order == 1) {
		row++;
		if (row >= _size || isWall[row][col]) cout << "�Ʒ��� �̵��� �� �����ϴ�. �ٽ� �����ϼ���.\n\n";
		else {
			cout << "�Ʒ��� �̵�!\n\n";
			res[0] = row;
			res[2] = 0;
		}
	}
	else if (order == 2) {
		col--;
		if (col < 0 || isWall[row][col]) cout << "�������� �̵��� �� �����ϴ�. �ٽ� �����ϼ���.\n\n";
		else {
			cout << "�������� �̵�!\n\n";
			res[1] = col;
			res[2] = 3;
		}
	}
	else if (order == 3) {
		col++;
		if (col >= _size || isWall[row][col]) cout << "���������� �̵��� �� �����ϴ�. �ٽ� �����ϼ���.\n\n";
		else {
			cout << "���������� �̵�!\n\n";
			res[1] = col;
			res[2] = 2;
		}
	}
	else {
		cout << "�̻��� ���� ���Խ��ϴ�.\n\n";
	}

	return res;
}

void StartGame::printArrow(int i) {
	if (i == 0) cout << " ��";
	else if (i == 1) cout << " ��";
	else if (i == 2) cout << " ��";
	else cout << " ��";
}

int StartGame::playGame()
{
	int getLocate = 2, getPath = 1;
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 14); //��Ʈ ����� ����
	cout << "**�̷� ���� START**\n\n";
	cout << 
		"*************************\n" <<
		"*      Chance Ƚ��      *\n" <<
		"*   0 : ��ġ �˸� " << getLocate << "��   *\n" <<
		"*   1 : ��� Ȯ�� " << getPath <<"��   *\n" <<
		"*************************\n\n";
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 15); //��Ʈ �Ͼ�� ����
	
	Path* P = new Path(0, 0, 0);

	int dx[] = { -1, 1, 0, 0 }, dy[] = { 0,0,-1,1 };

	int curRow = 0, curCol = 0, vis = 0, count = 0;
	while (true) {
		cout << "�̵� ������ ����(";

		int pathCount = 0, printCount = 0;
		//�̵� ���� ��� Ž��
		for (int i = 0; i < 4; i++) {
			int row = curRow + dx[i];
			int col = curCol + dy[i];

			if (row < 0 || col < 0 || row >= _size || col >= _size || isWall[row][col]) continue;

			if (printCount != 0) cout << ",";
			printArrow(i);
			printCount++;

			if (vis == i) cout << "(��� ������ ��)";
			else pathCount++;
		}

		if (pathCount == 0) {
			if (curRow == 0 && curCol == 0) {
				cout << " ) :\n���� �����Դϴ�.\n\n";
				vis = 0;
				
				continue;
			}

			cout << " )\n";
			cout << "���� �������� ���ư��� �մϴ�!\n";

			curRow = P->_row;
			curCol = P->_col;
			vis = P->_vis;

			cout << "������� ���ư����ϴ�.\n\n";

			continue;
		}
		else cout << " ) : ";

		int order = _input();

		//���� ��ġ �˸� ���� ��� ��
		if (order == 9) {
			if (getLocate == 0) {
				cout << "��ġ �˸� ������ �̹� ��� ����߽��ϴ�.\n\n";
				continue;
			}
			getLocate--;

			//�̷� ���(���� ��ġ)
			_board->Draw(curRow, curCol);
			cout << "���� ��ġ(��) : " << curRow << "�� " << curCol << "��\n\n";
			cout << "���� ��ġ �˸� ���� : " << getLocate << "\n���� �� ���� ����";
			
			//���� �Է� ������ �ܼ�â ���� ����
			_input(true);
			continue;
		}
		else if (order == 10) {
			if (getPath == 0) {
				cout << "��� Ȯ�� ������ �̹� ��� ����߽��ϴ�.\n\n";
				continue;
			}
			getPath--;

			//��� ���
			int cur_count = _board->getBFSCount() - _board->bfs(curRow, curCol);
			_board->search(curRow, curCol, cur_count, curRow, curCol, cur_count);
			cout << "���� ��� Ȯ�� ���� : " << getPath << "\n���� �� ��� Ȯ�� ����";
			
			//���� �Է� ������ �ܼ�â ���� ����
			_input(true);
			continue;
		}


		//�������̸� ���� ����Ǿ� �ִ� ������ ���� �ֽ�ȭ
		if (pathCount > 1) P->change(curRow, curCol, order);

		//�̵�
		int* path;
		path = move(order, curRow, curCol, vis);
		curRow = path[0];
		curCol = path[1];
		vis = path[2];

		count++;
		if (curRow == endRow && curCol == endCol) break;

		delete path;
	}

	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 14); //��Ʈ ����� ����
	cout << "�����մϴ�!\n";
	cout << "�̷θ� �������Խ��ϴ�!\n";
	cout << "�̷θ� Ż���ϴµ� " << count << "�� �õ��߽��ϴ�.\n\n";
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 15); //��Ʈ ����� ����
	
	cout << "\n���� �� ���� ȭ������ �̵�";
	_input(true);

	delete P;

	return count;
}

//eraseMap�� true�� ���� �� ���� ����, false�� �̵� ���� �Է�
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

		//���͸� �Է¹ް� �̵� ����� ��
		if (ch == 13 && order != -1) {
			cout << endl;
			break;
		}
		//�齺���̽�
		else if (ch == 8) {
			_input_backspace(order);
			order = -1;
		}
		//��
		else if (ch == 72) {
			_input_backspace(order);
			order = 0;
			cout << "��";
		}
		//��
		else if (ch == 75) {
			_input_backspace(order);
			order = 2;
			cout << "��";
		}
		//��
		else if (ch == 77) {
			_input_backspace(order);
			order = 3;
			cout << "��";
		}
		//��
		else if (ch == 80) {
			_input_backspace(order);
			order = 1;
			cout << "��";
		}
		//���� ��ġ ����
		else if (ch == '0') {
			_input_backspace(order);
			order = 9;
			cout << "0";
		}
		//��� ã�� ����
		else if (ch == '1') {
			_input_backspace(order);
			order = 10;
			cout << "1";
		}
	}

	return order;
}

//�ܼ�â�� ������ ��µǾ� �ִ� ���ڸ� ����
void StartGame::_input_backspace(int order)
{
	//�ƹ��͵� �ԷµǾ� ���� ���� ����
	if (order == -1) return;

	//0�� �Էµ�����
	if (order == 9 || order == 10) cout << "\b \b";
	//����Ű�� �Էµ�����
	else cout << "\b\b  \b\b";
}