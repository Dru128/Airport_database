#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <stdio.h>
#include <time.h>

#define ANSI_RESET_ALL     "\x1b[0m"
#define COLOR_RED          "\x1b[31m"
#define COLOR_GREEN        "\x1b[32m"

#define WIDTH 60 // стенка два символа
#define HEIGHT 29

#define EMPTY_TAG '1'
#define WALL_TAG '2'
#define PLAYER_TAG '3'
#define START_TAG '4'
#define FINISH_TAG '5'

/*
 длина консоли 120, ширина 30
 Лабиринт с монстрами
 Ведется стаистика прохождения времени и сохраняется в rate.txt
 Уровень хранится в level_1.txt
*/
typedef struct Point {
	int x, y;
};

int comparePoint(Point p1, Point p2) 
{
	return p1.x == p2.x && p1.y == p2.y;
}
// выгрузка карты из базы данных
int loadMapFromFile(char* map)
{
	FILE* levelDB;
	levelDB = fopen("level_1.txt", "r");
	if (levelDB == NULL) return 0;
	char symbol;

	for (int y = 0; y < HEIGHT; y++)
		for (int x = 0; x <= WIDTH; x++)
		{
			symbol = getc(levelDB);
			//putchar(symbol);
			if (symbol == EOF) {}
			else if (symbol == 0) {}
			else if (symbol == '\n') { break; }
			else
			{
				*(map + y * WIDTH + x) = symbol;
			}
		}

	fclose(levelDB);
}

// отрисовка карты
void drawMap(char* map, Point player, int countEnemies, Point* enemies)
{
	system("CLS");
	int isDrawCharacter;

	for (int y = 0; y < HEIGHT; y++)
	{
		for (int x = 0; x < WIDTH; x++)
		{
			isDrawCharacter = 0;
			if (player.x == x && player.y == y)
			{ 
				putchar(1); putchar(' '); isDrawCharacter = 1; 
			}

			if (isDrawCharacter == 0)
			for (int i = 0; i < countEnemies; i++)
				if ((*(enemies + i)).x == x && (*(enemies + i)).y == y)
				{
					putchar('}'); putchar('{'); isDrawCharacter = 1;
				}

			if (isDrawCharacter == 0)
			switch (*(map + y * WIDTH + x))
			{
				case EMPTY_TAG: {
					putchar(' '); putchar(' ');
					break;
				}
				case WALL_TAG: {
					putchar(219); putchar(219);
					break;
				}
				case START_TAG: {
					putchar(' '); putchar(' ');
					break;
				}
				case FINISH_TAG: {
					putchar(177); putchar(177);
					break;
				}
				default: {
					putchar('*'); putchar('*');
					break;
				}
			}
		}
		putchar('\n');
	}
}

// проверка на столкновение с монстром
int playerCollisionEnemy(Point player, int countEnemies, Point* enemies)
{
	for (int i = 0; i < countEnemies; i++)
		if (comparePoint(player, enemies[i]))
			return 1;
	return 0;
}

// ход монстров
void enemyMove(char* map, Point player, int countEnemies, Point* enemies)
{ // ИИ противника
	int _x, _y;
	int returnFlag;
	for (int i = 0; i < countEnemies; i++)
	{
		returnFlag = 0;
		_x = enemies[i].x;
		_y = enemies[i].y;
		//if (enemies[i].x == player.x || enemies[i].y == player.y)
		while (returnFlag == 0) {
			if (_y == player.y)
			{
				if (_x < player.x) // игрок справа
				{
					// игрок в прямой видимости - движемся в его сторону
					if (_x + 1 == player.x) {
						enemies[i].x++; returnFlag = 1;
					}
					// игрок не в прямой видимости
					if (*(map + _y * WIDTH + _x + 1) == WALL_TAG)
						returnFlag = 1;


					// проверяем следующию клетку
					if (_x == WIDTH - 1) returnFlag = 1;
					else _x++;
				}
				else // игрок слева
				{
					// игрок в прямой видимости - движемся в его сторону
					if (_x - 1 == player.x) {
						enemies[i].x--; returnFlag = 1;
					}
					// игрок не в прямой видимости
					if (*(map + _y * WIDTH + _x - 1) == WALL_TAG)
						returnFlag = 1;

					// проверяем следующию клетку
					if (_x == 0) returnFlag = 1;
					else _x--;
				}
			}
			else if (_x == player.x)
			{
				if (_y < player.y) // игрок снизу
				{
					// игрок в прямой видимости - движемся в его сторону
					if (_y + 1 == player.y) {
						enemies[i].y++; returnFlag = 1;
					}
					// игрок не в прямой видимости
					if (*(map + (_y + 1) * WIDTH + _x) == WALL_TAG)
						returnFlag = 1;

					// проверяем следующию клетку
					if (_y == HEIGHT + 1) returnFlag = 1;
					else _y++;
				}
				else // игрок сверху
				{
					// игрок в прямой видимости - движемся в его сторону
					if (_y - 1 == player.y) {
						enemies[i].y--; returnFlag = 1;
					}
					// игрок не в прямой видимости
					if (*(map + (_y - 1) * WIDTH + _x) == WALL_TAG)
						returnFlag = 1;

					// проверяем следующию клетку
					if (_y == 0) returnFlag = 1;
					else _y--;
				}
			}
			else { //рандом ход
				int countVectors = 0,
					vectors[4] = {0, 0, 0, 0};
				if (enemies[i].y != 0 && *(map + (enemies[i].y - 1) * WIDTH + enemies[i].x) != WALL_TAG)
				{
					vectors[0] = 1; countVectors++;
				}
				if (enemies[i].x != 0 && *(map + enemies[i].y * WIDTH + enemies[i].x - 1) != WALL_TAG) 
				{
					vectors[1] = 1; countVectors++;
				}
				if (enemies[i].y != HEIGHT - 1 && *(map + (enemies[i].y + 1) * WIDTH + enemies[i].x) != WALL_TAG)
				{
					vectors[2] = 1; countVectors++;
				}
				if (enemies[i].x != WIDTH - 1 && *(map + enemies[i].y * WIDTH + enemies[i].x + 1) != WALL_TAG) 
				{
					vectors[3] = 1; countVectors++;
				}

				if (countVectors != 0)
				{
					int j = 0,randomVector = rand() % countVectors;
					for (int k = 0; k < 4 && returnFlag == 0; k++)
					{	
						if (vectors[k] != 0)
						{

							if (randomVector == j)
							{
								switch (k)
								{
									case 0: {
										enemies[i].y--; break;
									}
									case 1: {
										enemies[i].x--; break;
									}
									case 2: {
										enemies[i].y++; break;
									}
									case 3: {
										enemies[i].x++; break;
									}
									default: break;
									}
								returnFlag = 1;
							}
							else j++;
						}
					}
						

				} 
				returnFlag = 1;
			
			}
		}
	}
}

// расстановка монстров на карте
void initEnemies(int count, Point* enemies)
{
	for (int i = 0; i < count; i++)
	{
		(*(enemies + i)).x = rand() % (WIDTH - 2) + 1;
		(*(enemies + i)).y = rand() % (HEIGHT - 5) + 1;
	}
}

// определдение позиции игрока на карте
void initPlayer(char* map, Point* player)
{
	for (int y = 0; y < HEIGHT; y++)
		for (int x = 0; x <= WIDTH; x++)
			if (*(map + y * WIDTH + x) == START_TAG)
			{
				(*player).x = x;
				(*player).y = y;
			}
}
/* endGame
* 0 игра продоложается
* 1 игрок проиграл 
* 2 выиграл
*/
// игровой ход - ожидание действия игрока
int gameMove()
{

	char* map = (char*) calloc(HEIGHT * WIDTH, sizeof(Point));
	int isMovePlayer, countEnemies = 10, endGame = 0;
	loadMapFromFile(map);

	Point player = {2, 2};
	Point* enemies = (Point*)calloc(HEIGHT * WIDTH, sizeof(Point));

	initEnemies(countEnemies, enemies);
	initPlayer(map, &player);

	drawMap(map, player, countEnemies, enemies);
	char code;

	while (1)
	{
		isMovePlayer = 0;
		code = _getwch();
		switch (code)
		{
			case 'w': {
				if (player.y != 0 && *(map + (player.y - 1) * WIDTH + player.x) != WALL_TAG)
				{ player.y--; isMovePlayer = 1; }
				break;
			}
			case 'a': {
				if (player.x != 0 && *(map + player.y * WIDTH + player.x - 1) != WALL_TAG)
					{ player.x--; isMovePlayer = 1; }
				break;
			}
			case 's': {
				if (player.y != HEIGHT - 1 && *(map + (player.y + 1) * WIDTH + player.x) != WALL_TAG)
					{ player.y++; isMovePlayer = 1; }
				break;
			}
			case 'd': {
				if (player.x != WIDTH - 1&& *(map + player.y * WIDTH + player.x + 1) != WALL_TAG)
					{ player.x++; isMovePlayer = 1; }
				break;
			}
			default: break;
		}
		if (isMovePlayer)
		{
			if (playerCollisionEnemy(player, countEnemies, enemies)) return 1;

			// Игрок сделал ход - теперь нужно чтобы сходил противник (бот) 
			enemyMove(map, player, countEnemies, enemies);

			if (playerCollisionEnemy(player, countEnemies, enemies)) return 1;

			if (*(map + player.y * WIDTH + player.x) == FINISH_TAG) return 2;


			drawMap(map, player, countEnemies, enemies);
		}
	}

}

void saveRate(char* nick, int time)
{
	FILE* rateDB;
	rateDB = fopen("D:/Andrei/projects/consolePrj/DruLaboratory/rate.txt", "r+");
	if (rateDB == NULL) return;

	char *line = (char*)calloc(22, sizeof(char));
	char _nick[10];
	int _time, isFind = 0;
	long offset = 0;

	while (fgets(line, 22, rateDB) != NULL) 
	{
		line = strtok(line, " ");
		if (line == NULL) break;
		strcpy(_nick, line);

		line = strtok(NULL, " ");
		if (line == NULL || atoi(line) == NULL) break;
		_time = atoi(line);

		if (strcmp(_nick, nick) == 0)
		{
			if (time >= _time)
				isFind = -1;
			else {
				offset = ftell(rateDB) - 21;
				isFind = 1;
			}
			break;
		}
	}
	if (isFind != -1)
	{
		if (isFind == 1 && fseek(rateDB, offset, SEEK_SET) == 0)
			fprintf(rateDB, "%10.10s %.10d", nick, time);
		else if (isFind == 0 && fseek(rateDB, 0, SEEK_END) == 0)
			fprintf(rateDB, "%10.10s %.10d", nick, time);
	}

	fclose(rateDB);
}

void printRateTable()
{
	FILE* rateDB;
	rateDB = fopen("D:/Andrei/projects/consolePrj/DruLaboratory/rate.txt", "r");
	if (rateDB == NULL) return;

	char * nick, * line = (char*)calloc(22, sizeof(char));

	printf("\n\tTable of rate\n");
	for (int i = 0; i < 27; i++) putchar('~');
	printf("\n| %-10.10s | %10.10s |\n", "Nick", "best time");

	while (fgets(line, 22, rateDB) != NULL)
	{
		nick = strtok(line, " ");
		line = strtok(NULL, " ");
		for (int i = 0; i < 27; i++) putchar('~');
		printf("\n| %-10.10s | %10.d |\n", nick, atoi(line));
	}
	for (int i = 0; i < 27; i++) putchar('~');
	putchar('\n');
}

void menu()
{
	time_t timer = time(NULL);
	int lastGameStatus = 0;
	char nick[20];

	while (1)
	{
		switch (lastGameStatus)
		{
			case 0: { // игр еще не было
				break;
			}
			case 1: { // проиграл
				printf(COLOR_RED "\nYou lose!\n" ANSI_RESET_ALL);
				break;
			}
			case 2: { // выиграл
				int curTime = int(time(NULL) - timer);
				printf(COLOR_GREEN "\nYou win!\n" ANSI_RESET_ALL);
				printf("\nComplietion time = %ld sec\n", curTime);
				saveRate(nick, curTime);
				break;
			}
			default:break;
		}
		printRateTable();

		printf("\nEnter your Nick to start game:");
		while (scanf("%s", nick) == 0);

		timer = time(NULL);
		lastGameStatus = gameMove();
	}
}


int main() { menu(); }