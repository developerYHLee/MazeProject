#include <iostream>
#include <string>
#include "Board.h"
#include "StartGame.h"

using namespace std;

int main() {
	SetConsoleTitle(TEXT("MAZE GAME OF MEMORY")); //�ܼ� ���� ����
	//system("mode con:cols=150 lines=50"); //�ܼ� ũ�� ����
	
	cout << "�̷��� ũ�⸦ �Է��ϼ���. (3���� ū Ȧ���� �Է�) : ";
	
	int size, score = INT_MAX;
	cin >> size;

	Board board(size);
	while (true) {
		cout << "����� �Է��ϼ���. (1 : ���� ����, 2 : �ִ� �Ÿ� Ȯ��, 3 : �̷� Ȯ��, 4 : �ִ� ��� ã��, 5 : �ִ� ��� Ȯ��, 6 : AI Ż�� ����, 0 : ����) : ";
		int order;
		cin >> order;

		StartGame startGame(&board);
		if (order == 1) {
			system("cls");

			int cur_score = startGame.playGame();
			score = min(score, cur_score);
		}
		else if (order == 2) board.bfs(0, 0, true);
		else if (order == 3) {
			cout << endl;
			board.Draw(false);
		}
		else if (order == 4) {
			board.ini_for_search();
			board.search();
		}
		else if (order == 5) cout << "\n�ִ� ��� : " << (score == INT_MAX ? "X" : to_string(score) + "��") << "\n\n";
		else if (order == 6) board.AIEscape();
		else if (order == 0) break;
		else cout << "��ɾ �ٽ� �Է��ϼ���.\n\n";
	}

	cout << "�̷� ���� ����\n";
}