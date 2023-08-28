#include <iostream>
#include "Board.h"
#include "StartGame.h"

using namespace std;

int main() {
	cout << "�̷��� ũ�⸦ �Է��ϼ���. (Ȧ���� �Է�) : ";
	int size;
	cin >> size;

	Board board(size);

	while (true) {
		cout << "����� �Է��ϼ���. (1 : �̷� ���� ����, 2 : �ִܰŸ� Ȯ��, 3 : �̷� Ȯ��, 4 : �ִ� ��� Ȯ��, 0 : ����) : ";
		int order;
		cin >> order;

		StartGame startGame(&board);

		if (order == 1) {
			system("cls");
			startGame.playGame();
		}
		else if (order == 2) board.bfs(true);
		else if (order == 3) board.Draw();
		else if (order == 4) board.search(0, 0, 0);
		else if (order == 0) break;
	}

	cout << "�̷� ���� ����\n";
}