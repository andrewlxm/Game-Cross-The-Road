#define _CRT_SECURE_NO_WARNINGS
#include<conio.h>
#include<iostream>
#include<Windows.h>
#include<thread>
#include<iomanip>
#include<string>
#include<stdio.h>
#include<vector>
#include<fstream>

using namespace std;

#define ARROW_UP 72
#define ARROW_DOWN 80
#define ARROW_LEFT 75
#define ARROW_RIGHT 77
#define SPACE 32
#define ENTER 13
#define ESC 27

//Console window
#define WIDTH 1488
#define HEIGHT 700
#define X_CENTER WIDTH / 16 + 2
#define Y_CENTER HEIGHT / 32

//Game
#define MAX_CAR 6
#define CAR_LENGTH 18
#define AMBULANCE_LENGTH 18
#define MAX_SPEED 3
#define CAR_HEIGH 5
#define AMBULANCE_HEIGH 5
#define PERSON_HEIGH 5
#define PERSON_LENGTH 7

int SCORE;
bool STOP = TRUE;
bool kt = false;
int DEMSTOP = 50;
int HEIGH_CONSOLE = 48, WIDTH_CONSOLE = 148;
int cnt = 0;//up speed 
char player_name[20];
char scorelist[20] = "s_list";
int Set[] = { 7,7,7,7,7 };
int cursor = 1;
int MOVING;//direction of man
int SPEED;// speed of car (level)
int CURRPOS[2];//position of man in previous level
bool STATE; // live/die
POINT* X; //array of MAX_CAR car
POINT Y; // this is man
POINT Z; //ambulance
POINT* T;// rand pos of car

//character
char CAR2[5][19] =
{
"   ________       ",
"  /_/__vJp_\\____ ",
" |_ ||_'._|_\\___\\",
"(________________|",
" '-(_)------(_)-' "
};
char CAR3[5][19] =
{
"       __________ ",
"    __//_pR0|CtT1|",
" __//__][___][__ |",
"(o  _ | - | _ | o|",
" '-(_)------(_)--'"
};
char AMBULANCE[5][19] =
{
" __________       ",
"|  __| |__ \\_    ",
"|  |__ __|   \\\__",
"|____|_|________o)",
"'--(_)-------(_)-'"
};
string PERSON[5] =
{
"OHCMUSS",
"  _o_ ",
" / | \\",
"  / \\",
"oo   oo"
};
string PERSON_SPACE[5] =
{
"       ",
"       ",
"       ",
"       ",
"       "
};
char TFLIGHT[5][7] =
{
" ____ ",
"[    ]",
"[    ]",
"[    ]",
" |__| "
};

//struct save name and score
typedef struct player
{
	char name[20];
	int score;
}pl;

//Struct menu
struct  MENU
{
	int options; // số chức năng của menu
	int x; // toa do cua diem bat dau 
	int y; // dau cua chuc nang dau tien
	int cursorColor = 7; // màu của con trỏ
};

//graphic func
void CreateConsoleWindow(int pWidth, int pHeight);
void GotoXY(int x, int y);
void FixConsoleWindow();
void set_color(int code);
void PrintText(string text, int color, int x, int y);
void PrintBigText(string filename, int color, int x, int y);

//Menu func
MENU mainMenu();
MENU loadingMenu();
MENU ESCMenu();
MENU yesNoMenu();
//run Menu
int SelectMenu(MENU menu);

//Gameplay func
void StartGame();
void PauseGame(HANDLE t);
void ExitGame(HANDLE t);
void Nocursortype();
void GabageCollect();
void ResetData();
void SubThread();

void DrawBoard(int x, int y, int width, int height);

void DrawCars();
void MoveCars();
void Erase0();
void EraseCars();

void DrawAmbulance();
void MoveAmbulance();
void EraseAmbulance();
void Erase0Am();

void Drawtflight();

bool CheckCurrpos();
bool IsImpact(const POINT& p);
void ProcessDead();
void ProcessFinish(POINT& p);
void DrawSticker(const POINT& p, string s[]);
void MoveRight();
void MoveLeft();
void MoveDown();
void MoveUp();

void Loadinggame();

void DrawScore(char* player);
void HelpInfo();
void SaveScoreList(char* playername);
void ReadScoreList();
void ShowRank();

void ReGame(char* player);
void SaveGame(char* player);

void CreateConsoleWindow(int pWidth, int pHeight)
{
	HWND consoleWindow = GetConsoleWindow();
	RECT r;
	HANDLE hConsole;
	hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
	SetConsoleTextAttribute(hConsole, 002);
	GetWindowRect(consoleWindow, &r);
	MoveWindow(consoleWindow, 0, 0, pWidth, pHeight, TRUE);
}
void GotoXY(int x, int y)
{
	COORD coord;
	coord.X = x;
	coord.Y = y;
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coord);
}
void FixConsoleWindow()
{
	HWND consoleWindow = GetConsoleWindow();
	LONG style = GetWindowLong(consoleWindow, GWL_STYLE);
	style = style & ~(WS_MAXIMIZEBOX) & ~(WS_THICKFRAME);
	SetWindowLong(consoleWindow, GWL_STYLE, style);
}
void set_color(int code)
{
	HANDLE color = GetStdHandle(STD_OUTPUT_HANDLE);
	SetConsoleTextAttribute(color, code);
}
void PrintText(string text, int color, int x, int y)
{
	GotoXY(x, y);
	set_color(color);
	cout << text;
	set_color(002);
}
void PrintBigText(string filename, int color, int x, int y)
{
	fstream textFile;
	textFile.open(filename.c_str(), fstream::in);
	string line;
	vector<string> line1;
	int tempY = y;
	while (getline(textFile, line)) line1.push_back(line);
	for (int i = 0; i < line1.size(); i++)
		PrintText(line1[i], color, x, y++);
	textFile.close();
}

MENU mainMenu()
{
	MENU menu;
	menu.options = 5;
	menu.x = X_CENTER;
	menu.y = Y_CENTER + 10;
	menu.cursorColor = 002;
	system("cls");
	PrintBigText("Text.txt", 23, 47, 10); //Print Logo of the game
	PrintText("*****   1. Play         *****", Set[cursor], menu.x - 5, menu.y); // Print dong 1 cua menu
	PrintText("*****   2. Load Game    *****", Set[cursor], menu.x - 5, menu.y + 1);// Print dong 2 cua menu
	PrintText("*****   3. High Score   *****", Set[cursor], menu.x - 5, menu.y + 2);// Print dong 3 cua menu
	PrintText("*****   4. Help         *****", Set[cursor], menu.x - 5, menu.y + 3);// Print dong 4 cua menu
	PrintText("*****   5. Exit         *****", Set[cursor], menu.x - 5, menu.y + 4);// Print dong 5 cua menu
	return menu;
}
int SelectMenu(MENU menu)
{
	//int cursor = 1;
	char key;
	PrintText("  +  ", menu.cursorColor, menu.x - 10, menu.y);
	do
	{
		key = _getch();
		if (key == ARROW_UP && cursor > 1)
		{
			PrintText("    ", menu.cursorColor, menu.x - 10, menu.y + cursor - 1);
			cursor--;
			PrintText("  +  ", menu.cursorColor, menu.x - 10, menu.y + cursor - 1);

		}
		else if (key == ARROW_DOWN && cursor < menu.options)
		{
			PrintText("    ", menu.cursorColor, menu.x - 10, menu.y + cursor - 1);
			cursor++;
			PrintText("  +  ", menu.cursorColor, menu.x - 10, menu.y + cursor - 1);
		}
		if (cursor == 1) {
			PrintText("*****   1. Play         *****", Set[12], menu.x - 5, menu.y);
			PrintText("*****   2. Load Game    *****", Set[1], menu.x - 5, menu.y + 1);// Print dong 2 cua menu
			PrintText("*****   3. High Score   *****", Set[1], menu.x - 5, menu.y + 2);// Print dong 3 cua menu
			PrintText("*****   4. Help         *****", Set[1], menu.x - 5, menu.y + 3);// Print dong 4 cua menu
			PrintText("*****   5. Exit         *****", Set[1], menu.x - 5, menu.y + 4);// Print dong 5 cua menu
		}
		if (cursor == 2) {
			
			PrintText("*****   1. Play         *****", Set[1], menu.x - 5, menu.y); // Print dong 1 cua menu
			PrintText("*****   2. Load Game    *****", Set[12], menu.x - 5, menu.y + 1);
			PrintText("*****   3. High Score   *****", Set[1], menu.x - 5, menu.y + 2);// Print dong 3 cua menu
			PrintText("*****   4. Help         *****", Set[1], menu.x - 5, menu.y + 3);// Print dong 4 cua menu
			PrintText("*****   5. Exit         *****", Set[1], menu.x - 5, menu.y + 4);// Print dong 5 cua menu
		}
		if (cursor == 3) {
			PrintText("*****   1. Play         *****", Set[1], menu.x - 5, menu.y); // Print dong 1 cua menu
			PrintText("*****   2. Load Game    *****", Set[1], menu.x - 5, menu.y + 1);// Print dong 2 cua menu
			PrintText("*****   3. High Score   *****", Set[12], menu.x - 5, menu.y + 2);
			PrintText("*****   4. Help         *****", Set[1], menu.x - 5, menu.y + 3);// Print dong 4 cua menu
			PrintText("*****   5. Exit         *****", Set[1], menu.x - 5, menu.y + 4);// Print dong 5 cua menu
		}
		if (cursor == 4) {
			PrintText("*****   1. Play         *****", Set[1], menu.x - 5, menu.y); // Print dong 1 cua menu
			PrintText("*****   2. Load Game    *****", Set[1], menu.x - 5, menu.y + 1);// Print dong 2 cua menu
			PrintText("*****   3. High Score   *****", Set[1], menu.x - 5, menu.y + 2);// Print dong 3 cua menu
			PrintText("*****   4. Help         *****", Set[12], menu.x - 5, menu.y + 3);
			PrintText("*****   5. Exit         *****", Set[1], menu.x - 5, menu.y + 4);// Print dong 5 cua menu
		}
		if (cursor == 5) {
			PrintText("*****   1. Play         *****", Set[1], menu.x - 5, menu.y); // Print dong 1 cua menu
			PrintText("*****   2. Load Game    *****", Set[1], menu.x - 5, menu.y + 1);// Print dong 2 cua menu
			PrintText("*****   3. High Score   *****", Set[1], menu.x - 5, menu.y + 2);// Print dong 3 cua menu
			PrintText("*****   4. Help         *****", Set[1], menu.x - 5, menu.y + 3);// Print dong 4 cua menu
			PrintText("*****   5. Exit         *****", Set[12], menu.x - 5, menu.y + 4);
		}
		

	} while (key != ENTER);
	return cursor;
}

void StartGame()
{
	system("cls");
	ResetData(); // Khởi tạo dữ liệu gốc
	DrawBoard(0, 0, WIDTH_CONSOLE, HEIGH_CONSOLE); // Vẽ màn hình game
	DrawScore(player_name);
	STATE = true;//Bắt đầu cho Thread chạy
}
void PauseGame(HANDLE t)
{
	SuspendThread(t);
}
void ExitGame(HANDLE t)
{
	system("cls");
	TerminateThread(t, 0);
	GabageCollect();
}
void Nocursortype()
{
	CONSOLE_CURSOR_INFO Info;
	Info.bVisible = FALSE;
	Info.dwSize = 20;
	SetConsoleCursorInfo(GetStdHandle(STD_OUTPUT_HANDLE), &Info);
}
void GabageCollect()
{
	delete[] X;
	delete[] T;
}
void ResetData()
{
	MOVING = ARROW_RIGHT;
	SPEED = 1;
	SCORE = 0;
	int tmp = (WIDTH_CONSOLE - 2) / PERSON_LENGTH;
	Y.x = (rand() % (tmp)) * PERSON_LENGTH + 1; Y.y = HEIGH_CONSOLE - 5;
	//cac vi tri ng co the xuat hien : 1+personlength*k
	if (X == NULL)
	{
		X = new POINT[MAX_CAR];
		for (int i = 0; i < MAX_CAR; i++)
		{
			int temp = (rand() % (WIDTH_CONSOLE - CAR_LENGTH)) + 1;
			X[i].x = temp;
			X[i].y = 1 + PERSON_HEIGH + 1 + (1 + CAR_HEIGH) * i;
		}
	}
	//player_nameơ20]=NULL;
	CURRPOS[0] = NULL; CURRPOS[1] = NULL;
	system("cls");
	//DrawBoard(0, 0, WIDTH_CONSOLE, HEIGH_CONSOLE);
}
void SubThread()
{
	while (1)
	{
		if (STATE)
		{
			switch (MOVING)
			{
			case ARROW_LEFT: MoveLeft(); break;
			case ARROW_RIGHT: MoveRight(); break;
			case ARROW_UP: MoveUp(); break;
			case ARROW_DOWN: MoveDown(); break;
			}
			MOVING = ' ';
			EraseCars();
			MoveCars();
			DrawCars();
			DrawBoard(0, 0, WIDTH_CONSOLE, HEIGH_CONSOLE);
			if (IsImpact(Y))
			{
				ProcessDead();
			}
			if (Y.y == 1)
			{
				if (CheckCurrpos()) ProcessDead();
				else
				{
					ProcessFinish(Y);
				}
				Sleep(10);
			}
		}
	}
}

void DrawBoard(int x, int y, int width, int height)
{
	GotoXY(x, y);
	cout << '.';// goc 1
	for (int i = 1; i < width; i++)cout << '_';
	cout << '.';//goc 2
	GotoXY(x, height + y);
	cout << '|';//goc 3
	for (int i = 1; i < width; i++)cout << '_';
	cout << '|';//goc4
	for (int i = y + 1; i < height + y; i++)
	{
		GotoXY(x, i); cout << '|';
		GotoXY(x + width, i); cout << '|';
	}
	for (int i = 1; i < HEIGH_CONSOLE / (CAR_HEIGH + 1); i++)
	{
		GotoXY(1, (CAR_HEIGH + 1) * i);
		for (int j = 1; j < width; j++)cout << '_';
	}
	GotoXY(0, 0);
	set_color(7);
}
void DrawCars()
{
	for (int i = 0; i < MAX_CAR; i += 2)
	{
		int x = X[i].x, y = X[i].y;
		set_color(10);
		for (int i = 0; i < CAR_HEIGH; i++)
		{
			GotoXY(x, y);
			cout << CAR3[i] << endl;
			y++;
		}
		set_color(7);
	}
	for (int i = 1; i < MAX_CAR; i += 2)
	{
		int x = X[i].x, y = X[i].y;
		set_color(11);
		for (int i = 0; i < CAR_HEIGH; i++)
		{
			GotoXY(x, y);
			cout << CAR2[i] << endl;
			y++;
		}
		set_color(7);
	}
}
void MoveCars()
{
	for (int i = 0; i < MAX_CAR; i += 2)
	{
		cnt = 0;
		do
		{
			cnt++;
			if (X[i].x == T[i].x && DEMSTOP > 0)
			{
				STOP = FALSE;
				DEMSTOP--;
			}
			else
			{
				X[i].x--;
			}
			Erase0();
			if (X[i].x == 1)
			{
				T[i].x = (rand() % (WIDTH_CONSOLE - CAR_LENGTH)) + 1;
				X[i].x = WIDTH_CONSOLE - CAR_LENGTH;
				STOP = TRUE;
				DEMSTOP = 50;
			}

		} while (cnt < SPEED);
	}
	for (int i = 1; i < MAX_CAR; i += 2)
	{
		cnt = 0;
		do
		{
			cnt++;
			if (X[i].x == T[i].x && DEMSTOP > 0)
			{
				STOP = FALSE;
				DEMSTOP--;
			}
			else X[i].x++;
			Erase0();
			if (X[i].x + CAR_LENGTH - 1 == WIDTH_CONSOLE)
			{
				T[i].x = (rand() % (WIDTH_CONSOLE - CAR_LENGTH)) + 1;
				X[i].x = 1;
				STOP = TRUE;
				DEMSTOP = 50;
			}
		} while (cnt < SPEED);
	}
}
void Erase0()
{
	for (int i = 0; i < MAX_CAR; i++)
	{
		if (X[i].x == 1 || X[i].x + CAR_LENGTH == WIDTH_CONSOLE)
		{
			for (int k = 0; k < CAR_HEIGH; k++)
			{
				GotoXY(X[i].x, X[i].y + k);
				cout << "                  ";
			}
		}
	}
}
void EraseCars()
{
	for (int i = 0; i < MAX_CAR; i += 2)
	{
		cnt = 0;
		do
		{
			for (int k = 0; k < CAR_HEIGH; k++)
			{
				GotoXY(X[i].x + CAR_LENGTH - 1 - cnt, X[i].y + k);
				cout << " ";
			}
			cnt++;
		} while (cnt < SPEED);
	}
	for (int i = 1; i < MAX_CAR; i += 2)
	{
		cnt = 0;
		do
		{
			for (int k = 0; k < 5; k++)
			{
				GotoXY(X[i].x + cnt, X[i].y + k);
				cout << " ";
			}
			cnt++;
		} while (cnt < SPEED);
	}
}
void DrawAmbulance()
{
	set_color(12);
	int temp = WIDTH_CONSOLE - 1 - 6;
	int tmp = WIDTH_CONSOLE;
	for (int i = 0; i < AMBULANCE_HEIGH; i++)
	{
		GotoXY(Z.x, Z.y + i);
		cout << AMBULANCE[i];
	}
	if (Z.x % 2 == 0)
	{
		GotoXY(Z.x + AMBULANCE_LENGTH, Z.y);
		set_color(64);
		cout << " ";
	}
	else
	{
		GotoXY(Z.x + AMBULANCE_LENGTH, Z.y);
		set_color(241);  //241=white 
		cout << " ";
	}
	set_color(7);
}
void MoveAmbulance()
{
	Z.y = Y.y;
	Z.x = 2;
	while (Z.x + AMBULANCE_LENGTH - 1 < WIDTH_CONSOLE)
	{
		DrawAmbulance();
		EraseAmbulance();
		Erase0Am();
		Drawtflight();
		Z.x++;
		Sleep(10);
	}
}
void EraseAmbulance()
{
	for (int k = 0; k < AMBULANCE_HEIGH; k++)
	{
		GotoXY(Z.x - 1, Z.y + k);
		cout << " ";
	}
}
void Erase0Am()
{
	if (Z.x + AMBULANCE_LENGTH == WIDTH_CONSOLE)
	{
		for (int k = 0; k < AMBULANCE_HEIGH; k++)
		{
			GotoXY(Z.x, Z.y + k);
			cout << "                  ";
		}
	}
}
void Drawtflight()
{
	int temp = WIDTH_CONSOLE + 1;
	set_color(15);
	for (int i = 0; i < 5; i++)
	{
		GotoXY(temp, Y.y + i);
		cout << TFLIGHT[i];
	}
	if (Z.x > 0 && Z.x < 60)
	{
		GotoXY(temp + 1, Y.y + 1);
		set_color(64);
		cout << "    ";
		GotoXY(temp + 1, Y.y + 2);
		set_color(7);
		cout << "    ";
		GotoXY(temp + 1, Y.y + 3);
		set_color(7);
		cout << "    ";
	}
	else
	{
		if (Z.x > 60 && Z.x < 120)
		{
			GotoXY(temp + 1, Y.y + 2);
			set_color(96);
			cout << "    ";
			GotoXY(temp + 1, Y.y + 1);
			set_color(7);
			GotoXY(temp + 1, Y.y + 3);
			set_color(7);
		}
		else if (Z.x > 120)
		{
			GotoXY(temp + 1, Y.y + 3);
			set_color(160);
			cout << "    ";
			GotoXY(temp + 1, Y.y + 1);
			set_color(7);
			cout << "    ";
			GotoXY(temp + 1, Y.y + 2);
			set_color(7);
			cout << "    ";
		}
	}
	set_color(7);
}

bool CheckCurrpos()
{
	if (SPEED == 1) return false;
	else if (Y.x == CURRPOS[0] || Y.x == CURRPOS[1]) return true;
	return false;
}
bool IsImpact(const POINT& p)
{
	if (p.y == 1 || p.y == HEIGH_CONSOLE - 5) return false;
	for (int i = 0; i < MAX_CAR; i++)
	{
		if (p.y == X[i].y)
		{
			if (X[i].x == p.x) return true;
			else if (X[i].x > p.x && p.x + PERSON_LENGTH >= X[i].x) return true;
			else if (X[i].x < p.x && X[i].x + CAR_LENGTH >= p.x) return true;
		}
	}
	return false;
}
void ProcessDead()
{
	STATE = 0;
	system("cls");
	DrawBoard(0, 0, WIDTH_CONSOLE, HEIGH_CONSOLE);
	DrawSticker(Y, PERSON);
	MoveAmbulance();
	system("cls");
	GotoXY(20, 20);
	cout << "Dead, press Space to return menu ";
}
void ProcessFinish(POINT& p)
{
	if (SPEED == MAX_SPEED)
	{
		if (SCORE != MAX_SPEED * 100)
		{
			SCORE += 100;
		}
		STATE = 0;
		SaveGame(player_name);
		system("cls");
		ShowRank();
		printf("You win, press Space to back to menu ");
	}
	else
	{
		CURRPOS[SPEED - 1] = p.x;
		SCORE += 100;
		SPEED++;
		int tmp = (WIDTH_CONSOLE - 2) / PERSON_LENGTH;
		p = { (rand() % (tmp)) * PERSON_LENGTH + 1,HEIGH_CONSOLE - 5 };
		MOVING = ARROW_RIGHT;
		DrawScore(player_name);
	}
}

void DrawSticker(const POINT& p, string s[])
{
	int x = p.x, y = p.y;
	set_color(13);
	for (int i = 0; i < PERSON_HEIGH; i++)
	{
		GotoXY(x, y);
		cout << s[i];
		y++;
	}
	set_color(7);
}
void MoveRight()
{
	if (Y.x < WIDTH_CONSOLE - 1 && Y.x + PERSON_LENGTH < WIDTH_CONSOLE - 1)
	{
		DrawSticker(Y, PERSON_SPACE);
		Y.x = Y.x + PERSON_LENGTH;
		DrawSticker(Y, PERSON);
	}
}
void MoveLeft()
{
	if (Y.x > 0 && Y.x - PERSON_LENGTH > 0)
	{
		DrawSticker(Y, PERSON_SPACE);
		Y.x = Y.x - PERSON_LENGTH;
		DrawSticker(Y, PERSON);
	}
}
void MoveDown()
{
	if (Y.y < HEIGH_CONSOLE - 1 && Y.y + PERSON_HEIGH + 1 < HEIGH_CONSOLE - 1)
	{
		DrawSticker(Y, PERSON_SPACE);
		Y.y = Y.y + PERSON_HEIGH + 1;
		DrawSticker(Y, PERSON);
	}
}
void MoveUp()
{
	int y = Y.y - PERSON_HEIGH - 1;
	if (Y.y > 0 && Y.y - PERSON_HEIGH - 1 > 0)
	{
		DrawSticker(Y, PERSON_SPACE);
		Y.y = Y.y - PERSON_HEIGH - 1;
		DrawSticker(Y, PERSON);
	}
}

void Loadinggame()
{
	system("cls");
	GotoXY(X_CENTER - 5, Y_CENTER-2);
	set_color(10);
	cout << "GAME LOADING..";
	GotoXY(X_CENTER - 5, Y_CENTER);
	int tmp = 1;
	for (tmp; tmp <= WIDTH_CONSOLE; tmp++)
	{
		Sleep(10);
		set_color(160);
		cout << " ";
		GotoXY(1, Y_CENTER);
		cout << 100 - (WIDTH_CONSOLE - tmp) * 100 / WIDTH_CONSOLE << "%";
		GotoXY(tmp, Y_CENTER);
	}
	set_color(7);
}
void DrawScore(char* player)
{
	GotoXY(WIDTH_CONSOLE + 1, 1);
	cout << "=======================================";
	GotoXY(WIDTH_CONSOLE + 1, 2);
	cout << "Player name :" << player;
	GotoXY(WIDTH_CONSOLE + 1, 3);
	cout << "=======================================";
	GotoXY(WIDTH_CONSOLE + 1, 4);
	cout << "Player's Score " << SCORE;
}
void HelpInfo()
{
	GotoXY(X_CENTER-5, Y_CENTER);
	cout << "Press arow up,left,down,right to move up,left,down,right" << endl;
	GotoXY(X_CENTER-5, Y_CENTER+1);
	cout << "Press L to save game when playing " << endl;
	GotoXY(X_CENTER-5, Y_CENTER+2);
	cout << "Press Space to pause " << endl;
	GotoXY(X_CENTER-5, Y_CENTER+3);
	cout << "Press ESC to exit game ";
}
void SaveScoreList(char* playername)
{
	FILE* fp = fopen("s_list.txt", "a");
	fprintf(fp, "%s\n", playername);
	fprintf(fp, "%d", SCORE);
	fclose(fp);
	return;
}
void ReadScoreList()
{
	int count = 0;
	fstream new_file;
	new_file.open("s_list.txt", ios::in);
	if (new_file.is_open())
	{

		char name[20];
		int data;
		do
		{
			new_file >> name >> data;
			count++;
		} while (!new_file.eof());
	}
	new_file.close();
	pl* listplayer = new pl[count];
	new_file.open("s_list.txt", ios::in);
	if (new_file.is_open())
	{
		int tmp = 0;
		do
		{
			new_file >> listplayer[tmp].name >> listplayer[tmp].score;
			tmp++;
		} while (!new_file.eof());
	}
	new_file.close();
	for (int i = 1; i < count; i++)
	{
		for (int j = 0; j < i; j++)
		{
			if (listplayer[i].score > listplayer[j].score)
			{
				pl temp = listplayer[i];
				listplayer[i] = listplayer[j];
				listplayer[j] = temp;
			}
		}
	}
	cout << setw(130) << "========================TOP SCORE========================" << endl;
	cout << setw(92) << "NAME" << setw(20) << "SCORE" << endl;
	for (int i = 0; i < count; i++)
	{
		cout << setw(92) << listplayer[i].name << setw(20) << listplayer[i].score << endl;
	}
}
void ShowRank()
{
	ReadScoreList();
	return;
}
void ReGame(char* player)
{
	system("cls");
	DrawBoard(0, 0, WIDTH_CONSOLE, HEIGH_CONSOLE);
	strcat(player, ".txt");
	FILE* fp = fopen(player, "r");
	fscanf_s(fp, "%d", &SPEED);
	fscanf_s(fp, "%d", &SCORE);
	for (int i = 0; i < MAX_CAR; i++)
	{
		fscanf(fp, "%d", &X[i].x);
		fscanf(fp, "%d", &T[i].x);
	}
	if (SPEED > 1)
	{
		fscanf(fp, "%d ", &CURRPOS[0]);
		GotoXY(CURRPOS[0], 1);
		Y.x = CURRPOS[0];
		Y.y = 1;
		DrawSticker(Y, PERSON);
	}
	if (SPEED > 2)
	{
		fscanf(fp, "%d ", &CURRPOS[0]);
		GotoXY(CURRPOS[1], 1);
		Y.x = CURRPOS[1];
		Y.y = 1;
		DrawSticker(Y, PERSON);
	}
	fscanf(fp, "%d %d", &Y.x, &Y.y);
	fscanf(fp, "%d %d", &Z.x, &Z.y);
	fclose(fp);

	//delete '.txt' out of player name
	player[strlen(player) - 1] = player[strlen(player)];
	player[strlen(player) - 1] = player[strlen(player)];
	player[strlen(player) - 1] = player[strlen(player)];
	player[strlen(player) - 1] = player[strlen(player)];


	DrawCars();
	DrawSticker(Y, PERSON);
}
void SaveGame(char* player)
{
	SaveScoreList(player_name);
	strcat(player, ".txt");
	FILE* fp = fopen(player, "w");
	fprintf(fp, "%d\n", SPEED);
	fprintf(fp, "%d\n", SCORE);
	for (int i = 0; i < MAX_CAR; i++)
	{
		fprintf(fp, "%d ", X[i].x);
		fprintf(fp, "\n");
		fprintf(fp, "%d ", T[i].x);
		if (i != 5)
		{
			fprintf(fp, "\n");
		}
	}
	if (SPEED > 1)
	{
		fprintf(fp, "%d", CURRPOS[0]);
		fprintf(fp, "\n");
	}
	if (SPEED > 2)
	{
		fprintf(fp, " %d", CURRPOS[1]);
		fprintf(fp, "\n");
	}
	fprintf(fp, "\n%d %d", Y.x, Y.y);
	fprintf(fp, "\n%d %d", Z.x, Z.y);

	fprintf(fp, "\n");
	fclose(fp);
	//delete '.txt' out of player name
	player[strlen(player) - 1] = player[strlen(player)];
	player[strlen(player) - 1] = player[strlen(player)];
	player[strlen(player) - 1] = player[strlen(player)];
	player[strlen(player) - 1] = player[strlen(player)];
	return;
}
void main()
{

	T = new POINT[MAX_CAR];
	ShowWindow(GetConsoleWindow(), SW_MAXIMIZE); FixConsoleWindow();
	Nocursortype();
	int temp;
	srand(time_t(NULL));
	thread t1(SubThread);
	int option;
	char key;
	do
	{
		option = SelectMenu(mainMenu());
		system("cls");
		key = _getch();
		if (key == ENTER) {
			switch (option) {
			case 1:
				ResetData();
				system("cls");
				GotoXY(X_CENTER - 5, Y_CENTER - 4);
				cout << "Type player name: ";
				gets_s(player_name);
				Loadinggame();
				StartGame();
				while (1)
				{
					int temp = toupper(_getch());
					if (STATE == 1)
					{
						if (temp == 27)
						{
							ExitGame(t1.native_handle());
							return;
						}
						else if (temp == ' ')
						{
							PauseGame(t1.native_handle());
						}
						else if (temp == 'L')
						{

							SaveGame(player_name);
							STATE = 0;
							ResetData();
							system("cls");
							break;
						}
						else
						{
							ResumeThread((HANDLE)t1.native_handle());
							if (temp == ARROW_RIGHT || temp == ARROW_LEFT || temp == ARROW_UP || temp == ARROW_DOWN)
							{
								MOVING = temp;
							}
						}
					}
					else
					{
						if (temp == ' ') break;
					}
				}
				break;
			case 2:
				ResetData();
				PauseGame(t1.native_handle());
				GotoXY(0, HEIGH_CONSOLE + 2);
				printf("Type name: ");
				gets_s(player_name);
				ReGame(player_name);
				STATE = true;
				DrawScore(player_name);
				while (1)
				{
					int temp = toupper(_getch());
					if (STATE == 1)
					{
						if (temp == 27)
						{
							ExitGame(t1.native_handle());
							return;
						}
						else if (temp == ' ')
						{
							PauseGame(t1.native_handle());
						}
						else if (temp == 'L')
						{
							PauseGame(t1.native_handle());
							SaveGame(player_name);
							STATE = 0;
							system("cls");
							break;
						}
						else
						{
							ResumeThread((HANDLE)t1.native_handle());
							if (temp == ARROW_RIGHT || temp == ARROW_LEFT || temp == ARROW_UP || temp == ARROW_DOWN)
							{
								MOVING = temp;
							}
						}
					}
					else
					{
						if (temp == ' ') break;
					}
				}
				break;
			case 3:
				ShowRank();
				cout << endl;
				system("pause");
				break;
			case 4:
				HelpInfo();
				cout << endl;
				system("pause");
				break;
			case 5:
				ExitGame(t1.native_handle());
				break;
			}
		}
		key = NULL;
		option = NULL;
		cursor = 1;
	} while (1);
}
