#include <stdio.h>
#include <Windows.h>
#include <stdlib.h>
#include <conio.h>

// 빈칸( 노말 ) if(combatMap[i][j] == 7) □
#define NORMAL 7

// 빨강/파랑 말 else if(combatMap[i][j] < 13) 
//				SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), combatMap[i][j]);
//■
#define RED 12
#define BLUE 9
#define WHITE 15

// 빨강/파랑 땅	else if(combatMap[i][j] <17) .....combatMap[i][j]-4);
//▣
#define RED_AREA 16
#define BLUE_AREA 13

// 커서		등은 코드 안에서 처리하면 됨		-8
// ◆
#define RED_CURSOR 20
#define BLUE_CURSOR 17

// 커서 겹친것		도 위 코드 써서 처리하면 됨		-12
// ●
#define RED_DOUBLE_CURSOR 24
#define BLUE_DOUBLE_CURSOR 21

// 땅과 겹친것		도 같음				-16
// ◎
#define RED_AREA_CURSOR 28
#define BLUE_AREA_CURSOR 25


#define VER 'Y'	// 세로		Y 축
#define HOR 'X'	// 가로		X 축
#define CRO 'O'	// 십자		원점

#define WAR 3
#define COMBAT 9

#define NEWLINE { gotoxy(9, 3 + go_y * 1); go_y++; }

#define CAR(x,y,n) n * (x) + (y)			// 카르티전


/// 전역변수 선언 구역

int warMap[3][3];			// (0, 0)
int combatMap[9][9] = {	// (0, 0)		0 1 2		3 4 5		6 7 8
	{ 0, 0, 0, 12, 9, 0, 12, 12, 9 },
	{ 0, 0, 0, 12, 9, 0, 12, 12, 9 },
	{ 0, 0, 0, 12, 9, 0, 12, 12, 9 },

	{ 0, 0, 0, 12, 9, 0, 12, 12, 9 },
	{ 0, 0, 0, 12, 9, 0, 12, 12, 9 },
	{ 0, 0, 0, 12, 9, 0, 12, 12, 9 },

	{ 0, 0, 0, 12, 9, 0, 12, 12, 9 },
	{ 0, 0, 0, 12, 9, 0, 12, 12, 9 },
	{ 0, 0, 0, 12, 9, 0, 12, 12, 9 }
};








//		컴뱃 영역을 워 위치에 지정		탐색 위치 지정 / 북서 위치 고정에 쓰임
COORD NWcombatWarTable[10] = {
	{ 0, 0 }, { 0, 3 }, { 0, 6 },
	{ 3, 0 }, { 3, 3 }, { 3, 6 },
	{ 6, 0 }, { 6, 3 }, { 6, 6 }
};

//0 0    0 3    0 6
//3 0    3 3    3 6
//6 0    6 3    6 6

//			"							동남 위치 고정에 쓰임
COORD SEcombatWarTable[10] = {
	{ 2, 2 }, { 2, 5 }, { 2, 8 },
	{ 5, 2 }, { 5, 5 }, { 5, 8 },
	{ 8, 2 }, { 8, 5 }, { 8, 8 }
};
//2 2    2 5    2 8
//5 2    5 5    5 8
//8 2    8 5    8 8











int curX, curY;			// 입력때 조건에 따라 변동되고, 출력때 이를 감지해서 출력을 바꿈
int player;						// 0이면 RED이고 1이면 BLUE임
int playerColor[2] = { 3, 0 };	// RED이면 3을 더해줘서 색을 맞춰야함






int ATfield;	// 0~8까지는 각 번호, 9는 제한 없음







///


int Check(int* map, int n, int x, int y);
void gotoxy(int x, int y);
void printScreen();
void input();
void mapReset();


void startGame();



int main() {
	int i, j, k, n;
	int choose = 1;
	CONSOLE_CURSOR_INFO Cursor;
	Cursor.bVisible = 0;
	Cursor.dwSize = 1;
	SetConsoleCursorInfo(GetStdHandle(STD_OUTPUT_HANDLE), &Cursor);


	printf("\n"); SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), RED);
	printf("   @@@ @@@ @@@     @@@@@  @@@ @@   @@    @     @@@@@  @@@@@  "); printf("\n");
	printf("    @   @   @      @ @ @   @   @   @     @     @ @ @   @   @ "); printf("\n");
	printf("    @   @   @     @  @  @  @   @@ @@     @    @  @  @  @     "); printf("\n");
	printf("    @   @   @        @     @   @@ @@    @ @      @     @  @  "); printf("\n");
	printf("    @   @   @        @     @   @@ @@    @ @      @     @@@@  "); printf("\n"); SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), BLUE);
	printf("    @   @   @        @     @   @ @ @    @ @      @     @  @  "); printf("\n");
	printf("    @   @   @        @     @   @ @ @    @@@      @     @     "); printf("\n");
	printf("    @   @   @   @    @     @   @   @   @   @     @     @     "); printf("\n");
	printf("    @   @   @  @     @     @   @   @   @   @     @     @   @ "); printf("\n");
	printf("     @@@   @@@@@    @@@   @@@ @@@ @@@ @@@ @@@   @@@   @@@@@  "); printf("\n"); SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), WHITE);
	printf("\n");
	printf("\t\t\t\t\t\Tic Tac Toe"); printf("\n");
	printf("\n");
	printf("\n"); SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), NORMAL);
	printf("\t\t Press Enter to Start");
	printf("\n");
	printf("\n");
	printf("\t\t\tEND");

	while (1) {
		if (choose) {
			gotoxy(40, 15);
			printf("◀");
			gotoxy(30, 17);
			printf("    ");
		}
		else {
			gotoxy(30, 17);
			printf("◀");
			gotoxy(40, 15);
			printf("    ");
		}


		n = getch();
		switch (n) {
		case 13:
			if (choose) startGame();		// 시작
			else return 0;		// 종료
			break;
		case 72:
		case 'w':
		case 80:
		case 's':
			choose = !choose; break;
		default:break;
		}
	}

}

int Check(int* map, int n, int x, int y) {
	int i;
	// 1. Forward slash Checking
	if (*(map + CAR(x + 2, y, n)) == *(map + CAR(x + 1, y + 1, n)) && *(map + CAR(x + 1, y + 1, n)) == *(map + CAR(x, y + 2, n))) return *(map + CAR(x, y, n));
	// 2. Backslash Checking
	if (*(map + CAR(x, y, n)) == *(map + CAR(x + 1, y + 1, n)) && *(map + CAR(x, y, n)) == *(map + CAR(x + 2, y + 2, n))) return *(map + CAR(x, y, n));
	// 3. Horizontal Checking
	// 4. Verical Checking
	for (i = 0; i < 3; i++) {
		if (*(map + CAR(x + i, y, n)) == *(map + CAR(x + i, y + 1, n)) && *(map + CAR(x + i, y, n)) == *(map + CAR(x + i, y + 2, n))) return *(map + CAR(x + i, y, n));
		//if (combatMap[x][y + i] == combatMap[x + 1][y + i] && combatMap[x][y + i] == combatMap[x + 2][y + i])
		if (*(map + CAR(x, y + i, n)) == *(map + CAR(x + 1, y + i, n)) && *(map + CAR(x, y + i, n)) == *(map + CAR(x + 2, y + i, n))) return combatMap[x][y + i];
	}
}

void gotoxy(int x, int y) {
	COORD pos = { x, y };
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), pos);
}

void printScreen() {
	int i, j, k, go_y = 0;
	//system("cls");

	for (i = 0; i < 9; i++) {		// 맵의 Y 축
		NEWLINE	// 이제 \n를 대신할 것

			if ((i / 3) && !(i % 3)) {	// 수평축 UI
				//for (k = 1; k < 12; k++) {
				//	if ((k / 4) && !(k % 4)) printf("┼ ");
				//	else printf("─ ");
				//}
				////printf("\n");
				printf("─ ─ ─ ┼ ─ ─ ─ ┼ ─ ─ ─");
				NEWLINE
			}

		for (j = 0; j < 9; j++) {	// 맵의 X 축
			if ((j / 3) && !(j % 3)) printf("│ ");	// 수직축 UI


			// 출력 형식의 변동이 있음
			// 1. 커서 위치 확인시 이에 맞는 것 출력
			// 1-1. 일반커서, 커서가 말과 겹친 것, 커서가 땅과 겹친 것
			if (curX == i && curY == j) {
				if (combatMap[i][j] <= NORMAL)	// 일반커서
				{
					// 색 정하는 기준을 정해야함. 아마 전역변수가 편할듯		입력에서 정하게 되겠지
					SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), BLUE + playerColor[player]);
					printf("◆");
					SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), NORMAL);
				}
				else if (combatMap[i][j] < 13)	// 말과 겹침
				{
					SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), combatMap[i][j]);
					printf("●");
				}
				else if (combatMap[i][j] < 17)	// 땅과 겹침
				{
					SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), combatMap[i][j] - 4);
					printf("◎");
				}
				SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), NORMAL);
			}
			// 2. 일반 분지
			else if (combatMap[i][j] <= NORMAL) {
				SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), NORMAL);
				printf("□");
			}
			// 3. 말 영역
			else if (combatMap[i][j] < 13) {
				SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), combatMap[i][j]);
				printf("■");
			}
			// 4. 땅 영역
			else if (combatMap[i][j] < 17) {
				SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), combatMap[i][j] - 4);
				printf("▣");
			}
			SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), NORMAL);

			//if (!combatMap[i][j]) printf("□");
			//else if (combatMap[i][j] == BLUE) {
			//	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), BLUE);
			//	printf("■");
			//	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), NORMAL);
			//}
			//else if (combatMap[i][j] == RED) {
			//	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), RED);
			//	printf("■");
			//	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), NORMAL);
			//}
		}
		//printf("\n");
	}


	//getch();
	NEWLINE
		NEWLINE
		if (!player) {
			SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), RED);
			printf("    ▶RED");
			SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), WHITE);
			printf(" : ");
			SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), BLUE);
			printf("BLUE");
		}
		else {
			SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), RED);
			printf("     RED");
			SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), WHITE);
			printf(" : ");
			SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), BLUE);
			printf("BLUE◀");
		}
}

void input() {
	// 아직 자세히는 안함. 위치 고정이라던가 해야함

	switch (getch()) {
	case 72:
	case 'w': curX -= curX > 0 ? 1 : 0;
		break;
	case 75:
	case 'a': curY -= curY > 0 ? 1 : 0;
		break;
	case 80:
	case 's': curX += curX < 8 ? 1 : 0;
		break;
	case 77:
	case 'd': curY += curY < 8 ? 1 : 0;
		break;
	case ' ':
		if (combatMap[curX][curY] == 0 || combatMap[curX][curY] == NORMAL) {
			combatMap[curX][curY] = BLUE + playerColor[player];
			player = !player;
		}
		break;
	}
}

void mapReset() {
	int i, j;

	for (i = 0; i < 9; i++) for (j = 0; j < 9; j++) combatMap[i][j] = 0;
	for (i = 0; i < 3; i++) for (j = 0; j < 3; j++) warMap[i][j] = 0;
}

void startGame() {
	system("cls");
	curX = 4; curY = 4;

	// 본게임에서는 넣어둬야하지만 아직은 시험단계니 안넣을까 하는데
	//mapReset();

	while (1) {

		// 전의 상태를 따져서 커서의 위치를 옮겨놔야함
		// 맵출력
		printScreen();
		// 입력받고 / 입력되는지 검사하고
		// 입력 정상적으로 되면 가상의 맵의 커서위치를 각각 %3 하여 국지적으로 둔 위치를 찾고 그걸 warMap 좌표에 넣어 다음 고정 전투를 잡는다.

		// 플레이어 바뀌는건 어캐 구현할꺼임?
		// 그냥 while문 돌리면서 bool값 바꿀까?

		// 문제가 하나 생김. 그냥 combatMap에 커서값 넣고 그걸 출력하면 커서랑 겹쳐있는 곳은 값이 소멸됨.
		// 즉, 커서 위치를 printScreen에서 감지해서 값을 바꿔야함.
		// 이에 따라 생기는 문제 : 딱히 없어보이는데? 단지 출력처리에서 전역으로 선언한 커서의 위치를 찾으면 됨
		// 또는, 홀로그램 맵을 하나 만들어서 그 위에 출력처리를 다 한다음에 printScreen은 그걸 출력하기만 하면 됨.
		// 이에 따라 생기는 문제 : 홀로그램 맵 따로 만들고 printScreen 함수에 있는 combatMap을 다 바꿔야함

		input();
		// 입력을 함수로 만들까? 아니면 그냥 여기에 구현을 할까?
		// 1.함수 구현
		// 장점 : 정확히 역할을 나누기 때문에 보기 좋음
		// 단점 : 함수 호출에 의해 느려질 수 있음
		// 2. 내부 구현
		// 장점 : 당연히 함수 호출보다 속도가 빠름
		// 단점 : 함수로 나누는 것보다 주석으로만 설명 달아두면 가독성이 떨어짐


		// 게임 끝났는지 검사하고
		// combatMap의 9개를 다 검사한 후 이를 적용시켜 warMap을 검사해야함
		//getch();
	}
}

// 성현이
//void printMap() {
//	int i, j;
//	cnt = 0;
//	for (i = 0; i < 9; i++) {
//		for (j = 0; j < 9; j++) {
//			if (ttt[i][j] != 0) cnt++;
//			gotoxy((j + 1) * 2 + (j / 3) * 2, (i + 1) + i / 3);
//			if (TTT[i / 3][j / 3] != 0) {
//				if (j == x && i == y) printcolor("◆", (turn % 2 == 1 ? 4 : 3));
//				else printcolor("▣", (TTT[i / 3][j / 3] % 2 == 1 ? 4 : 3));
//			}
//			else if (j == x && i == y) {
//				if (ttt[i][j] == 0) printcolor("◆", (turn % 2 == 1 ? 4 : 3));
//				else printcolor("●", (ttt[y][x] == 1 ? 4 : 3));
//			}
//			else {
//				if (ttt[i][j] != 0) printcolor("■", (ttt[i][j] == 1 ? 4 : 3));
//				else if (pos == 0 || ((i >= (pos - 1) / 3 * 3 && i <= (pos - 1) / 3 * 3 + 2) && (j >= (pos - 1) % 3 * 3 && j <= (pos - 1) % 3 * 3 + 2))) printcolor("□", 8);
//				else printcolor("□", 15);
//			}
//			if (j == 2 || j == 5) printcolor("│ ", 15);
//		}
//		if (i == 2 || i == 5) printcolor("\n  ─ ─ ─ ┼ ─ ─ ─ ┼ ─ ─ ─ ", 15);
//	}
//	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 4);
//	printf("\n\n  %s %s %d", turn % 2 == 1 ? ":arrow_forward:" : "  ", p1, P1);
//	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 15);
//	printf(" : ");
//	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 3);
//	printf(" %d %s %s", P2, p2, turn % 2 == 0 ? ":arrow_backward:" : "  ");
//}