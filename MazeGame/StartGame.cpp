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

void StartGame::playGame()
{
	int getLocate = 1;
	cout << "�̷� ã�� ����!\n��ġ �˸� ������ " << getLocate << "�� �ֽ��ϴ�.\n���� ��ġ�� �˰� ������ 0�� �Է��ϼ���.\n\n";
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

		int order = _input(false);

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

		//�������̸� ���� ����Ǿ� �ִ� ������ ���� �ֽ�ȭ
		if (pathCount > 1) P->change(curRow, curCol, order);

		//�̵�
		int* path;
		path = move(order, curRow, curCol, vis);
		curRow = path[0];
		curCol = path[1];
		vis = path[2];

		count++;
		if (curRow == endRow && curCol == endCol) {
			cout << "�̷θ� �������Խ��ϴ�!\n";
			break;
		}

		delete path;
	}

	cout << "�̷θ� Ż���ϴµ� " << count << "�� �õ��߽��ϴ�.\n\n";

	delete P;
}

//eraseMap�� true�� ���� �� ���� ����, false�� �̵� ���� �Է�
int StartGame::_input(bool eraseMap)
{
	int order = -1;

	while (true) {
		char ch = _getch();

		if (eraseMap) {
			if (ch != 13) continue;

			system("cls");
			return 1;
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
	}

	return order;
}

void StartGame::_input_backspace(int order)
{
	//�ƹ��͵� �ԷµǾ� ���� ���� ����
	if (order == -1) return;

	//0�� �Էµ�����
	if (order == 9) cout << "\b \b";
	//����Ű�� �Էµ�����
	else cout << "\b\b  \b\b";
}