#include <stdio.h>
#include <Windows.h>
#include <stdlib.h>
#include <conio.h>

// 빈칸( 노말 ) if(combatMap[i][j] <= NORMAL) □
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

int warMap[3][3];
// 0 0    0 1    0 2
// 1 0    1 1    1 2
// 2 0    2 1    2 2

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
int getAT[3][3] = {
	{0,1,2},
	{3,4,5},
	{6,7,8}
};

//		컴뱃 영역을 워 위치에 지정		탐색 위치 지정 / 북서 위치 고정에 쓰임
COORD NWcombatWarTable[10] = {
	{ 0, 0 }, { 0, 3 }, { 0, 6 },
	{ 3, 0 }, { 3, 3 }, { 3, 6 },
	{ 6, 0 }, { 6, 3 }, { 6, 6 },
	{ 0, 0 }
};

//0 0    0 3    0 6
//3 0    3 3    3 6
//6 0    6 3    6 6

//			"							동남 위치 고정에 쓰임
COORD SEcombatWarTable[10] = {
	{ 2, 2 }, { 2, 5 }, { 2, 8 },
	{ 5, 2 }, { 5, 5 }, { 5, 8 },
	{ 8, 2 }, { 8, 5 }, { 8, 8 },
	{ 8, 8 }
};
//2 2    2 5    2 8
//5 2    5 5    5 8
//8 2    8 5    8 8
COORD getCursor[3][3] = {
	{{1,1}, {1,4}, {1,7}},
	{{4,1}, {4,4}, {4,7}},
	{{7,1}, {7,4}, {7,7}}
};
// 1 1    1 4    1 7
// 4 1    4 4    4 7
// 7 1    7 4    7 7

int curX, curY;			// 입력때 조건에 따라 변동되고, 출력때 이를 감지해서 출력을 바꿈
int player;						// 0이면 RED이고 1이면 BLUE임
int playerColor[2] = { 3, 0 };	// RED이면 3을 더해줘서 색을 맞춰야함
int ATfield;	// 0~8까지는 각 번호, 9는 제한 없음

///

int Check(int* map, int n, int x, int y);
void gotoxy(int x, int y);
void printScreen();
int input();
void mapReset();
int startGame();

int main() {
	int i, j, k, n;
	int choose = 1, repeat;
	CONSOLE_CURSOR_INFO Cursor;
	Cursor.bVisible = 0;
	Cursor.dwSize = 1;
	SetConsoleCursorInfo(GetStdHandle(STD_OUTPUT_HANDLE), &Cursor);

	while (1) {
		system("cls");
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

		for (repeat = 0; !repeat;) {
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
			case ' ':
				if (choose) for(;!repeat;)repeat = startGame();		// 시작
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
}

int Check(int* map, int n, int x, int y) {
	int i;
	// 1. Forward slash Checking
	if (*(map + CAR(x + 2, y, n)) == *(map + CAR(x + 1, y + 1, n)) && *(map + CAR(x + 1, y + 1, n)) == *(map + CAR(x, y + 2, n)) && *(map +  CAR(x + 1,y + 1,n)) != 0) return *(map + CAR(x + 1, y + 1, n));
	// 2. Backslash Checking
	if (*(map + CAR(x, y, n)) == *(map + CAR(x + 1, y + 1, n)) && *(map + CAR(x, y, n)) == *(map + CAR(x + 2, y + 2, n)) && *(map + CAR(x, y, n)) != 0) return *(map + CAR(x, y, n));
	// 3. Horizontal Checking
	// 4. Verical Checking
	for (i = 0; i < 3; i++) {
		if (*(map + CAR(x + i, y, n)) == *(map + CAR(x + i, y + 1, n)) && *(map + CAR(x + i, y, n)) == *(map + CAR(x + i, y + 2, n)) && *(map + CAR(x + i, y, n)) != 0) return *(map + CAR(x + i, y, n));
		//if (combatMap[x][y + i] == combatMap[x + 1][y + i] && combatMap[x][y + i] == combatMap[x + 2][y + i])
		if (*(map + CAR(x, y + i, n)) == *(map + CAR(x + 1, y + i, n)) && *(map + CAR(x, y + i, n)) == *(map + CAR(x + 2, y + i, n)) && *(map + CAR(x, y + i, n)) != 0) return *(map + CAR(x, y + i, n));
	}
	return 0;
}

void gotoxy(int x, int y) {
	COORD pos = { x, y };
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), pos);
}

void printScreen() {
	int i, j, k, go_y = 0;

	for (i = 0; i < 9; i++) {		// 맵의 Y 축
		NEWLINE	// 이제 \n를 대신할 것

			if ((i / 3) && !(i % 3)) {	// 수평축 UI
				printf("─ ─ ─ ┼ ─ ─ ─ ┼ ─ ─ ─");
				NEWLINE
			}

		for (j = 0; j < 9; j++) {	// 맵의 X 축
			if ((j / 3) && !(j % 3)) printf("│ ");	// 수직축 UI

			// 출력 형식의 변동이 있음
			// 1. 커서 위치 확인시 이에 맞는 것 출력
			// 1-1. 일반커서, 커서가 말과 겹친 것, 커서가 땅과 겹친 것
			if (curX == i && curY == j) {
				if (warMap[curX/3][curY/3])		// 이미 먹힌 땅이라면?
				{
					SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), warMap[curX/3][curY/3]);
					printf("◎");

				}
				else if (combatMap[i][j] <= NORMAL)	// 일반커서
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
			else if (warMap[i / 3][j / 3]) {
				SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), warMap[i / 3][j / 3]);
				printf("▣");
			}
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

int input() {
	switch (getch()) {
	case 72:
	case 'w': curX -= curX > NWcombatWarTable[ATfield].X ? 1 : 0;
		break;
	case 75:
	case 'a': curY -= curY > NWcombatWarTable[ATfield].Y ? 1 : 0;
		break;
	case 80:
	case 's': curX += curX < SEcombatWarTable[ATfield].X ? 1 : 0;
		break;
	case 77:
	case 'd': curY += curY < SEcombatWarTable[ATfield].Y ? 1 : 0;
		break;
	case ' ':
		// 잠겨있으면 하면 안됨
		if (warMap[curX/3][curY/3]);
		else if (combatMap[curX][curY] <= NORMAL) {
			combatMap[curX][curY] = BLUE + playerColor[player];
			player = !player;

			// 게임 끝났는지 검사하고
			// combatMap의 9개를 다 검사한 후 이를 적용시켜 warMap을 검사해야함
				// 1. 변화가 일어난 combatMap만 검사	-> warMap 영향 조건

			warMap[curX/3][curY/3] = Check(combatMap, COMBAT, NWcombatWarTable[(curX / 3) * 3 + (curY / 3)].X, NWcombatWarTable[(curX / 3) * 3 + (curY / 3)].Y);
			// (curX/3)*3+(curY/3)
				// 2. warMap 검사	-> 엔딩 조건
				// 엔딩조건이 좀 바뀌었는데, input함수가 void형이 아닌 int형이 되어서 결과를 출력해줘야함

			if (warMap[curX % 3][curY % 3]) {	// 이미 차지 되어있다면 자유모드 돌입
				ATfield = 9;
				curX = curY = 4;
			}
			else {
				ATfield = getAT[curX % 3][curY % 3]; // 테이블이 해당 ATfield값 뱉어내기
				curX = getCursor[curX % 3][curY % 3].X; // 테이블이 해당 좌표 이동 시키기
				curY = getCursor[curX % 3][curY % 3].Y;
			}
			return Check(warMap, WAR, 0, 0);
		}
		break;
	}
	return 0;
}

void mapReset() {
	int i, j;

	for (i = 0; i < 9; i++) for (j = 0; j < 9; j++) combatMap[i][j] = 0;
	for (i = 0; i < 3; i++) for (j = 0; j < 3; j++) warMap[i][j] = 0;
}

int startGame() {
	int choose = 1, ending = 0;
	system("cls");
	curX = 4; curY = 4;
	ATfield = 9;
	player = 0;
	
	// 본게임에서는 넣어둬야하지만 아직은 시험단계니 안넣을까 하는데
	mapReset();

	while (!ending) {
		printScreen();	// 상태 출력 함수
		ending = input();		// 입력 처리 & 검사 함수
	}
	printScreen();

	
	if (ending == RED) {
		gotoxy(49, 6);
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), RED);
		printf("RED WIN!");
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), NORMAL);
	}
	else {
		gotoxy(48, 6);
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), BLUE);
		printf("BLUE WIN!");
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), NORMAL);
	}
	

	//printf("메인 메뉴로\n");	// 출력 위치 정해야함		ㅇㅇㅇ WIN! 도 띄워야해서 좀 애매함
	//printf("다시 플레이\n");
	while (1) {
		if (choose) {
			SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), WHITE);
			gotoxy(40, 8);
			printf("메인 메뉴◀    다시 플레이        ");
		}
		else {
			SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), WHITE);
			gotoxy(40, 8);
			printf("메인 메뉴      다시 플레이◀");
		}

		switch (getch()) {
		case ' ':
			if (choose) return 1;
			else return 0;
			break;
		case 75:
		case 'a':
		case 77:
		case 'd':
			choose = !choose;
			break;
		default:break;
		}
	}
}
