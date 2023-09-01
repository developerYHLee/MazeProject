#include <iostream>
#include <string>
#include "Board.h"
#include "StartGame.h"

using namespace std;

int main() {
	SetConsoleTitle(TEXT("MAZE GAME OF MEMORY")); //콘솔 제목 지정
	//system("mode con:cols=150 lines=50"); //콘솔 크기 지정
	
	cout << "미로의 크기를 입력하세요. (3보다 큰 홀수만 입력) : ";
	
	int size, score = INT_MAX;
	cin >> size;

	Board board(size);
	while (true) {
		cout << "명령을 입력하세요. (1 : 게임 시작, 2 : 최단 거리 확인, 3 : 미로 확인, 4 : 최단 경로 찾기, 5 : 최단 기록 확인, 6 : AI 탈출 보기, 0 : 종료) : ";
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
		else if (order == 5) cout << "\n최단 기록 : " << (score == INT_MAX ? "X" : to_string(score) + "번") << "\n\n";
		else if (order == 6) board.AIEscape();
		else if (order == 0) break;
		else cout << "명령어를 다시 입력하세요.\n\n";
	}

	cout << "미로 게임 종료\n";
}