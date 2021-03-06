﻿#include"pch.h"
#include<SFML/Graphics.hpp>
#include<SFML/Window.hpp>
#include<SFML/OpenGL.hpp>
#include<SFML/Audio.hpp>
#include<SFML/Config.hpp>
#include<SFML/System.hpp>
#include<iostream>
#include<algorithm>
#include<vector>
#include<queue>
#include<functional>
#include<ctime>
#include<time.h>
using namespace sf;
using namespace std;


void mainmenu();



struct leftmovement {
	int i, j;
};
struct rightmovement {
	int i, j;
};
struct upmovement {
	int i, j;
};
struct downmovement {
	int i, j;
};
struct BFS_Movement {
	int i, j;
}Smart_Skeleton_Movement, Red_Robot_Movement;
struct status {
	int lives = 3;
	int range = 3;

}player_status;
//////////////////////////////////////////////////////////
//Bomb struct 
struct bomb {
	bool bomb_placed = 0;
	bool bomb_exploded = 0;
	vector <Sprite>range_right;
	vector <Sprite>range_left;
	vector <Sprite>range_up;
	vector <Sprite>range_down;
	Sprite sprt;
	Time bomb_time, start_explosion, explosion_time;
	Clock bomb_clock, start_clock, explosion_clock;
	int x_pos, y_pos, x_index, y_index;
	int exp_animation_x = 1, exp_animation_y = 0, bomb_animation_y = 0, bomb_animation_x = 0;
	bool flag_range_up = 1, flag_range_down = 1, flag_range_right = 1, flag_range_left = 1;
};
int pos_x = 1, pos_y = 0, pos = 0, a, b, i, j;
bool explosion = false, ispressed = false, flag = 0, flag1 = 0;
/////////////////////////////////////////////////////////
//General stuff
const int length = 12, width = 17;
float Smart_Monsters_Speed = 0.3;
int Random_Monsters_Speed = 1;

// Menu stuff
#define MAX_NUMBER_OF_ITEMS 4
int selectedItemIndex = 0;
Font font; Text menu[MAX_NUMBER_OF_ITEMS];
void setmenustatus(float width, float height) {
	if (!font.loadFromFile("Chocolate Bar Demo.otf"))
	{
		// handle error
		cout << "SHIIIIT" << endl;
	}

	for (int i = 0; i < MAX_NUMBER_OF_ITEMS; i++)
	{
		menu[i].setFont(font);
		if (i == 0) {
			menu[i].setFillColor(Color::Red);
			menu[i].setString("Single player");
			menu[i].setPosition(sf::Vector2f(width / 2.5, height / (MAX_NUMBER_OF_ITEMS + 1) * 2.35));
		}
		else if (i == 1) {
			menu[i].setFillColor(Color::White);
			menu[i].setString("Multiplayer");
			menu[i].setPosition(sf::Vector2f(width / 2.5, height / (MAX_NUMBER_OF_ITEMS + 1) * 3.1));

		}
		else if (i == 2)
		{
			menu[i].setFillColor(Color::White);
			menu[i].setString("About us");
			menu[i].setPosition(sf::Vector2f(width / 2.4, height / (MAX_NUMBER_OF_ITEMS + 1) * 3.75));
		}
		else {
			menu[3].setFillColor(Color::White);
			menu[3].setString("Exit");
			menu[3].setPosition(sf::Vector2f(width / 2.25, height / (MAX_NUMBER_OF_ITEMS + 1) * 4.5));
		}

	}
}

bool visted[100][100];
// Levels_Menu stuff
#define MAX_NUMBER_OF_levels 4
int selectedlevelIndex = 0;
Font levelsfont; Text levels[MAX_NUMBER_OF_levels];

void setlevelsstatus(float width, float height) {
	if (!levelsfont.loadFromFile("Chocolate Bar Demo.otf"))
	{
		// handle error
		cout << "font error" << endl;
	}

	for (int i = 0; i < MAX_NUMBER_OF_levels; i++)
	{
		levels[i].setFont(levelsfont);
		if (i == 0) {
			levels[i].setFillColor(Color::Red);
			levels[i].setString("Beginner");
			levels[i].setPosition(sf::Vector2f(width / 2.4, height / (MAX_NUMBER_OF_ITEMS + 1) * 2.35));
		}
		else if (i == 1) {
			levels[i].setFillColor(Color::White);
			levels[i].setString("Advanced");
			levels[i].setPosition(sf::Vector2f(width / 2.4, height / (MAX_NUMBER_OF_ITEMS + 1) * 3.1));

		}
		else if (i == 2) {

			levels[i].setFillColor(Color::White);
			levels[i].setString("Legendary");
			levels[i].setPosition(sf::Vector2f(width / 2.4, height / (MAX_NUMBER_OF_ITEMS + 1) * 3.75));

		}
		else if (i==3)
        {
            levels[i].setFillColor(Color::White);
			levels[i].setString("Back");
			levels[i].setPosition(sf::Vector2f(width / 2.4, height / (MAX_NUMBER_OF_ITEMS + 1) * 4.45));

        }
	}
}

void drawmenu(RenderWindow &window)
{
	for (int i = 0; i < MAX_NUMBER_OF_ITEMS; i++)
	{
		window.draw(menu[i]);

	}
}
void drawlev(RenderWindow &window)
{
	for (int i = 0; i < MAX_NUMBER_OF_levels; i++)
	{
		window.draw(levels[i]);

	}
}
void MoveUp()
{
	if (selectedItemIndex - 1 >= 0)
	{
		menu[selectedItemIndex].setFillColor(Color::White);
		selectedItemIndex--;
		menu[selectedItemIndex].setFillColor(Color::Red);
	}
}
void MoveUplev()
{
	if (selectedlevelIndex - 1 >= 0)
	{
		levels[selectedlevelIndex].setFillColor(Color::White);
		selectedlevelIndex--;
		levels[selectedlevelIndex].setFillColor(Color::Red);
	}
}
void MoveDown()
{
	if (selectedItemIndex + 1 < MAX_NUMBER_OF_ITEMS)
	{
		menu[selectedItemIndex].setFillColor(Color::White);
		selectedItemIndex++;
		menu[selectedItemIndex].setFillColor(Color::Red);
	}
}
void MoveDownlev()
{
	if (selectedlevelIndex + 1 < MAX_NUMBER_OF_levels)
	{
		levels[selectedlevelIndex].setFillColor(Color::White);
		selectedlevelIndex++;
		levels[selectedlevelIndex].setFillColor(Color::Red);
	}
}
int GetPressedItem() {
	return selectedItemIndex;
}
int GetPressedlevel() {
	return selectedlevelIndex;
}

int GetXindx(float a)
{
	return (a - 183) / 53 + 2;
}
int GetYindx(float a)
{
	return (a - 74) / 48 + 2;
}
int GetXpostion(int a)
{
	return ((a - 2) * 53) + 198;
}
int GetYpostion(int a)
{
	return ((a - 2) * 48) + 89;
}
void destroy_breakable_walls(int Xindx, int Yindx, Sprite Grid[][13], Texture &ground, Texture &ground2)
{

	if (Xindx % 2 == Yindx % 2) { Grid[Xindx][Yindx].setTexture(ground); }
	else { Grid[Xindx][Yindx].setTexture(ground2); }

	Grid[Xindx][Yindx].setPosition((Xindx + 2) * 53, (Yindx + 0.8) * 48);
	Grid[Xindx][Yindx].setScale(0.1009, 0.101);


}
void destroy_breakable_walls(int Xindx, int Yindx, Sprite Grid[][10], Texture &ground, Texture &ground2)
{

	if (Xindx % 2 == Yindx % 2) { Grid[Xindx][Yindx].setTexture(ground); }
	else { Grid[Xindx][Yindx].setTexture(ground2); }

	Grid[Xindx][Yindx].setPosition((Xindx + 2) * 53, (Yindx + 0.8) * 48);
	Grid[Xindx][Yindx].setScale(0.1009, 0.101);


}

bool vis[20][20];
int dx[] = { 0,  0, 1, -1 };
int dy[] = { 1, -1, 0,  0 };
bool valid(int i, int j, Sprite arr[][length + 1], Texture &g1, Texture &g2)
{
	return i >= 2 and j >= 2 and i < width + 1 and j < length + 1 and !vis[i][j] and (arr[i][j].getTexture() == &g1 || arr[i][j].getTexture() == &g2);
}
void BFS(int srcX, int srcY, int distX, int distY, Sprite arr[][length + 1], Texture &g1, Texture &g2)
{
	memset(vis, 0, sizeof vis);
	queue <pair<int, int> > Q;
	vis[srcX][srcY] = 1;
	Q.push({ srcX, srcY });
	while (!Q.empty())
	{
		int nodeX = Q.front().first;
		int nodeY = Q.front().second;
		//cout << nodeY << endl;
		if ((nodeX == distX && nodeY + 1 == distY)) {
			Smart_Skeleton_Movement.i = nodeX;
			Smart_Skeleton_Movement.j = nodeY;
			Red_Robot_Movement.i = nodeX;
			Red_Robot_Movement.j = nodeY;
			break;
		}
		else if (nodeX == distX && nodeY - 1 == distY) {
			Smart_Skeleton_Movement.i = nodeX;
			Smart_Skeleton_Movement.j = nodeY;
			Red_Robot_Movement.i = nodeX;
			Red_Robot_Movement.j = nodeY;
			break;
		}
		else if (nodeX + 1 == distX && nodeY == distY) {
			Smart_Skeleton_Movement.i = nodeX;
			Smart_Skeleton_Movement.j = nodeY;
			Red_Robot_Movement.i = nodeX;
			Red_Robot_Movement.j = nodeY;
			break;
		}
		else if (nodeX - 1 == distX && nodeY == distY) {
			Smart_Skeleton_Movement.i = nodeX;
			Smart_Skeleton_Movement.j = nodeY;
			Red_Robot_Movement.i = nodeX;
			Red_Robot_Movement.j = nodeY;
			break;
		}

		Q.pop();
		for (int k = 0; k < 4; k++)
		{
			int newNodeX = nodeX + dx[k];
			int newNodeY = nodeY + dy[k];
			if (valid(newNodeX, newNodeY, arr, g1, g2))
			{
				vis[newNodeX][newNodeY] = 1;
				Q.push({ newNodeX, newNodeY });
			}
		}
	}
}

void Generate_Charcter(RectangleShape &charcter, Texture &charcter_texture, Vector2u &charcter_size, string filename, int X_Postion, int Y_Postion) {
	X_Postion = GetXpostion(X_Postion); Y_Postion = GetYpostion(Y_Postion);
	charcter.setPosition(X_Postion, Y_Postion);
	charcter.setScale(0.8, 0.8);
	charcter_texture.loadFromFile(filename);
	charcter.setTexture(&charcter_texture);
	charcter_size = charcter_texture.getSize();
	charcter_size.x /= 9;
	charcter_size.y /= 4;
	charcter.setTextureRect(IntRect(charcter_size.x * 0, charcter_size.y * 2, charcter_size.x, charcter_size.y));
}
void Movement_Upward(RectangleShape &character, Time &Monster_time, Clock &Monster_clock, float &Monster_position, Vector2u &Monster_text_size, float Monster_speed)
{
	character.move(0.0, -Monster_speed);
	Monster_time = Monster_clock.getElapsedTime();
	if (Monster_time.asSeconds() >= 0.1) {
		Monster_clock.restart();
		Monster_position++;
		if (Monster_position > 8)
			Monster_position -= 8;
	}
	character.setTextureRect(IntRect(Monster_text_size.x*Monster_position, Monster_text_size.y * 0, Monster_text_size.x, Monster_text_size.y));
}
void Movement_Downward(RectangleShape &character, Time &Monster_time, Clock &Monster_clock, float &Monster_position, Vector2u &Monster_text_size, float Monster_speed)
{
	character.move(0.0, Monster_speed);
	Monster_time = Monster_clock.getElapsedTime();
	if (Monster_time.asSeconds() >= 0.1) {
		Monster_clock.restart();
		Monster_position++;
		if (Monster_position > 8)
			Monster_position -= 8;
	}
	character.setTextureRect(IntRect(Monster_text_size.x*Monster_position, Monster_text_size.y * 2, Monster_text_size.x, Monster_text_size.y));
}
void Movement_Right(RectangleShape &character, Time &Monster_time, Clock &Monster_clock, float &Monster_position, Vector2u &Monster_text_size, float Monster_speed)
{
	character.move(Monster_speed, 0.0);
	Monster_time = Monster_clock.getElapsedTime();
	if (Monster_time.asSeconds() >= 0.1) {
		Monster_clock.restart();
		Monster_position++;
		if (Monster_position > 8)
			Monster_position -= 8;
	}
	character.setTextureRect(IntRect(Monster_text_size.x*Monster_position, Monster_text_size.y * 3, Monster_text_size.x, Monster_text_size.y));
}
void Movement_Left(RectangleShape &character, Time &Monster_time, Clock &Monster_clock, float &Monster_position, Vector2u &Monster_text_size, float Monster_speed)
{
	character.move(-Monster_speed, 0.0);
	Monster_time = Monster_clock.getElapsedTime();
	if (Monster_time.asSeconds() >= 0.1) {
		Monster_clock.restart();
		Monster_position++;
		if (Monster_position > 8)
			Monster_position -= 8;
	}
	character.setTextureRect(IntRect(Monster_text_size.x*Monster_position, Monster_text_size.y * 1, Monster_text_size.x, Monster_text_size.y));
}
bool Die, playsong = 1;;
void Beginner_Mode() 
{
	int big_pos_x = 1, big_pos_y = 0; //بيتعرف فوق
	Texture big_exp_text;
	big_exp_text.loadFromFile("exp.png");
	Sprite big_exp;
	big_exp.setTexture(big_exp_text);
	Vector2u text_size_big_exp = big_exp_text.getSize();
	text_size_big_exp.x /= 3;
	text_size_big_exp.y /= 3;
	big_exp.setScale(3, 3);
	Clock start_clock, timer_clock, clock_shutdown;
	Time start_timer, timer, timer_shutdown;
	int display_timer, minutes = 1;
	Text txt;
	Texture death_texture;
	death_texture.loadFromFile("death.png");
	Font dis_time;
	if (!dis_time.loadFromFile("Chocolate Bar Demo.otf"))
	{
		cout << "error font";
	}

	Time elaps = seconds(200);
	int elapsed = elaps.asSeconds();
	//###############################################################################
					// Load Songs
	Music ingame, lost;
	if (!ingame.openFromFile("ingame.wav"))             // ingame
		cout << "error to load ingame " << endl;
	if (!lost.openFromFile("For the Damaged Coda.wav"))     // End_Game
		cout << "error to load The damgae of coda";
	ingame.play();
	ingame.setLoop(1);

	if (Joystick::isConnected(0))
	{
		cout << "1 is connect" << endl;
	}	
	if (Joystick::isConnected(1))
	{
		cout << "2 is connect" << endl;
	}
	//###############################################################################
	// Making of Player
	RectangleShape player(Vector2f(100, 120));
	Texture player_texture;
	Vector2u player_size;
	Clock player_clock;
	Time player_time;
	float player_position = 0;
	const int player_speed = 2;
	Generate_Charcter(player, player_texture, player_size, "mainplayer.png", 2, 2);
	//###############################################################################

	//###############################################################################
	// Making of Yellow_Robot
	RectangleShape Yellow_Robot(Vector2f(100, 120));
	Texture Yellow_Robot_texture;
	Vector2u Yellow_Robot_size;
	Clock Yellow_Robot_clock;
	Time Yellow_Robot_time;
	float Yellow_Robot_speed = 1;
	float Yellow_Robot_position = 0;
	int Yellow_Robot_Counter = 0;
	int Yellow_Robot_Rand_motion = rand() % 4 + 1;
	Generate_Charcter(Yellow_Robot, Yellow_Robot_texture, Yellow_Robot_size, "yellow_robot.png", 15, 2);
	//###############################################################################

	//###############################################################################
	// Making of Skeleton
	RectangleShape Skeleton(Vector2f(100, 120));
	Texture Skeleton_texture;
	Vector2u Skeleton_size;
	Clock Skeleton_clock;
	Time Skeleton_time;
	int Skeleton_Rand_motion = rand() % 4 + 1;
	int Skeleton_counter = 0;
	float Skeleton_position = 0;
	Generate_Charcter(Skeleton, Skeleton_texture, Skeleton_size, "monster (5).png", 7, 8);
	//###############################################################################


	//###############################################################################
	// Making of White_Robot
	RectangleShape White_Robot(Vector2f(100, 120));
	Texture White_Robot_texture;
	Vector2u White_Robot_size;
	Clock White_Robot_clock;
	Time White_Robot_time;
	int White_Robot_Rand_motion = rand() % 4 + 1;
	int White_Robot_counter = 0;
	float White_Robot_position = 0;
	Generate_Charcter(White_Robot, White_Robot_texture, White_Robot_size, "white_robot.png", 14, 8);
	//###############################################################################

	//###############################################################################
	// Movement stuff
	leftmovement left; rightmovement right; upmovement up; downmovement down;
	int Player_xindx = 0, Player_yindx = 0, White_Robot_xindx = 0, White_Robot_yindx = 0, Skeleton_xindx = 0, Skeleton_yindx = 0, Robot_xindx = 0, Robot_yindx = 0;
	//###############################################################################
						// Making of MAP //
	//###############################################################################
						// Making of Grid //
	Texture wall, ground, wallpaper, ground2, breakable, hurt, choice;
	if (!breakable.loadFromFile("breakable.png"))
	{
		cout << "error loading breakable wall pic";
	}
	if (!wallpaper.loadFromFile("wallpaper.jpg"))
	{
		cout << "error loading wallpaper pic";
	}
	if (!wall.loadFromFile("3d wall.png"))
	{
		cout << "error loading wall pic";
	}

	if (!ground.loadFromFile("ground 1.png"))
	{
		cout << "error loading ground pic";
	}
	if (!ground2.loadFromFile("grass.png"))
	{
		cout << "error loading grass pic";
	}

	if (!hurt.loadFromFile("hurt.png"))
	{
		cout << "error loading  hurt pic";
	}
	hurt.setSmooth(1);
	//###############################################################################

				// Distribution of wall , brekable & ground on the Grid //

	Sprite Grid[width + 1][length + 1], Hurt, Wallpaper;

	for (int i = 1; i <= width; i++)                // Distribution of wall and ground on the Grid //
	{
		for (int j = 1; j <= length; j++)
		{
			if (i % 2 != 0 && j % 2 != 0)
			{

				Grid[i][j].setTexture(wall);
				Grid[i][j].setPosition((i + 1.97)*53.15, (j + 0.255) * 48);
				Grid[i][j].setScale(Vector2f(0.1009, 0.165));

			}

			else if (i == 1 || i == width)
			{
				Grid[i][j].setTexture(wall);
				Grid[i][j].setPosition((i + 1.97)*53.15, (j + 0.255) * 48);
				Grid[i][j].setScale(Vector2f(0.1009, 0.165));
			}
			else if (j == 1 || j == length)
			{
				Grid[i][j].setTexture(wall);
				Grid[i][j].setPosition((i + 1.97)*53.15, (j + 0.255) * 48);
				Grid[i][j].setScale(Vector2f(0.1009, 0.165));
			}
			else
			{
				if (i % 2 == j % 2) { Grid[i][j].setTexture(ground); }
				else { Grid[i][j].setTexture(ground2); }
				Grid[i][j].setPosition((i + 2) * 53, (j + 0.8) * 48);
				Grid[i][j].setScale(0.1, 0.101);
			}
		}
	}


	for (int i = 1; i <= 30; i++)                   // Distribution of Brekable on the Grid //
	{
		int x = rand() % 15 + 2, y = rand() % 9 + 2;
		if (x % 2 != 0 && y % 2 != 0)
		{
			x++;
		}
		Grid[x][y].setTexture(breakable);
		Grid[x][y].setPosition((x + 1.97)*53.15, (y + 0.255) * 48);
		Grid[x][y].setScale(Vector2f(0.1009, 0.165));
	}



	//###############################################################################


	//###############################################################################
							// Player Abilities and Features //
	Texture  life, bomb_frame, range, tomb, Rip, healskill, speedskill, rangeskill, bombskill;
	Hurt.setTexture(hurt);
	Wallpaper.setTexture(wallpaper);
	if (!life.loadFromFile("flife.png"))
	{
		cout << "error loading life pic";
	}
	if (!range.loadFromFile("frange.png"))
	{
		cout << "error loading range pic";
	}
	if (!bomb_frame.loadFromFile("fbomb.png"))
	{
		cout << "error loading bomb pic";
	}
	if (!tomb.loadFromFile("RIP.png"))
	{
		cout << "error loading tomb pic";
	}
	if (!healskill.loadFromFile("Heal .png"))
	{
		cout << "error to loading Heal skill" << endl;
	}

	if (!rangeskill.loadFromFile("Range skill.png"))
	{
		cout << "error to loading Range skill " << endl;
	}
	if (!bombskill.loadFromFile("bomb skill .png"))
	{
		cout << "error to loading Range skill " << endl;
	}



	//###############################################################################
	// Player Abilities
	Sprite SuperPowers[14];

	for (int i = 0; i < 5; i++)
	{

		SuperPowers[i].setTexture(bombskill);           // Rand_Choice 1 => Bombing
		SuperPowers[i].setScale(0.5, 0.3);

	}
	for (int i = 5; i < 8; i++)
	{
		SuperPowers[i].setTexture(healskill);       // Rand_Choice 2 => Health
		SuperPowers[i].setScale(0.5, 0.3);
	}

	for (int i = 8; i < 13; i++)
	{
		SuperPowers[i].setTexture(rangeskill);              // Rand_Choice 3 => Range
		SuperPowers[i].setScale(0.5, 0.3);
	}
	Texture timebomb;
	timebomb.loadFromFile("time-bomb.png");
	Sprite timebombS;
	SuperPowers[13].setTexture(timebomb);               // Rand_Choice 3 => Range
	SuperPowers[13].setScale(0.05, 0.05);



	SuperPowers[0].setPosition(GetXpostion(6) + 20, GetYpostion(5));
	SuperPowers[1].setPosition(GetXpostion(12) + 20, GetYpostion(11));
	SuperPowers[2].setPosition(GetXpostion(8) + 20, GetYpostion(5));
	SuperPowers[3].setPosition(GetXpostion(14) + 20, GetYpostion(10));
	SuperPowers[4].setPosition(GetXpostion(4) + 20, GetYpostion(6));
	SuperPowers[5].setPosition(GetXpostion(8) + 20, GetYpostion(8));
	SuperPowers[6].setPosition(GetXpostion(6) + 20, GetYpostion(10));
	SuperPowers[7].setPosition(GetXpostion(14) + 20, GetYpostion(6));
	SuperPowers[8].setPosition(GetXpostion(12) + 20, GetYpostion(8));
	SuperPowers[9].setPosition(GetXpostion(8) + 20, GetYpostion(10));
	SuperPowers[10].setPosition(GetXpostion(6) + 20, GetYpostion(6));
	SuperPowers[11].setPosition(GetXpostion(16) + 20, GetYpostion(6));
	SuperPowers[12].setPosition(GetXpostion(16) + 20, GetYpostion(8));
	SuperPowers[13].setPosition(GetXpostion(16) + 20, GetYpostion(11));



	//###############################################################################
	Texture Complete;
	Complete.loadFromFile("level-completed.png");
	Sprite lifeS, bombS, rangeS, Tomb, RIP_Photo, Level_complete;
	Level_complete.setTexture(Complete);
	lifeS.setTexture(life);
	lifeS.setPosition(0, 250);
	lifeS.setScale(0.4, 0.4);

	rangeS.setTexture(range);
	rangeS.setPosition(0, 180);
	rangeS.setScale(0.4, 0.4);

	bombS.setTexture(bomb_frame);
	bombS.setPosition(0, 330);
	bombS.setScale(0.4, 0.4);

	Tomb.setTexture(tomb);
	Tomb.setScale(0.1, 0.1);

	RIP_Photo.setTexture(death_texture);
	Clock Clock_Damgae, SuperPowers_clock;
	Time SuperPowers_time;

	//###############################################################################


	//###############################################################################
				//Player Bomb stuff //
	Sprite bombing, exp;
	bombing.setScale(0.2f, 0.2f);
	exp.setScale(0.25, 0.25);
	Texture playertext, player2text, bombtext, exptext;
	playertext.loadFromFile("mainplayer.png");
	player2text.loadFromFile("blue.png");
	bombtext.loadFromFile("BOOM.png");
	exptext.loadFromFile("exp.png");
	bombing.setTexture(bombtext);
	exp.setTexture(exptext);
	Vector2u text_size_bomb = bombtext.getSize();
	Vector2u text_size_exp = exptext.getSize();
	text_size_bomb.y /= 4;
	text_size_exp.x /= 3;
	text_size_exp.y /= 3;
	Time times2, times, t, ti, bomb_timer;
	Clock clocks2, c, cs, clocks, clock_timer;

	bomb bomb_template;
	bomb_template.sprt.setTexture(bombtext);
	bomb_template.sprt.setScale(0.15f, 0.15f);
	vector<bomb>bombs;
	int lives = 3;
	int boombs = 1;
	Clock counter;
	int number_of_bombs = 0, range_of_bomb = 1, bomb_placement_timer = 0, superpower_of_bomb = 2, counter_placed_bombs = 0;
	//bombs.push_back(bomb_template);
	//###############################################################################
	int death_time_dur = 0;
	bool death_flag = 0;
	bool Yellow_Robot_death = 0, White_Robot_death = 0, Skeleton_death = 0;
	//###############################################################################
							// Game Window //
	int temp_range_of_bomb = 1;
	bool isbomb = 0;
	RenderWindow Level_1(VideoMode(1280, 768), "Fcis in Danger ");
	while (Level_1.isOpen())
	{
		// dead
		/////////////////////////////////////
		timer = timer_clock.getElapsedTime();
		display_timer = timer.asSeconds();
		string s1, s2;
		s1 = to_string(59 - (display_timer));
		s2 = to_string(minutes);
		s2 += ":";
		txt.setFont(dis_time);
		txt.setString(s2 + s1);
		txt.setPosition(512, 22);
		if (display_timer > 59 && minutes > 0)
		{
			minutes--;
			timer_clock.restart();
		}
		if (minutes == 0 && display_timer == 59) {
			Die = 1;
			ispressed = 1;
		}
		if (ispressed) {
			start_timer = start_clock.getElapsedTime();
			if (start_timer.asSeconds() >= 0.2) {
				start_clock.restart();
				big_exp.setPosition((Level_1.getPosition().x / 2) - 25, (Level_1.getPosition().y / 2) - 210);
				big_exp.setTextureRect(IntRect(text_size_big_exp.x*big_pos_x, text_size_big_exp.y*big_pos_y, text_size_big_exp.x, text_size_big_exp.y));

				big_pos_x++;
				if (big_pos_x == 3) {
					big_pos_x = 0, big_pos_y++;
					if (big_pos_y == 2 && big_pos_x == 2) {
						ispressed = 0;
						timer_shutdown = clock_shutdown.getElapsedTime();
						death_time_dur = timer_shutdown.asSeconds();
						big_exp.setColor(Color::Transparent);
					}
				}
			}
		}


		/////////////////////////////////////

		string ss = to_string(temp_range_of_bomb);

		Text noofranges;
		noofranges.setFont(font);
		noofranges.setString(ss);
		noofranges.setCharacterSize(45);
		noofranges.setPosition(90, 195);

		string sss = to_string(boombs);

		Text noofbombs;
		noofbombs.setFont(font);
		noofbombs.setString(sss);
		noofbombs.setCharacterSize(45);
		noofbombs.setPosition(90, 335);

		string ssl = to_string(lives);

		Text nooflives;
		nooflives.setFont(font);
		nooflives.setString(ssl);
		nooflives.setCharacterSize(45);
		nooflives.setPosition(90, 270);

		// Robot 1 movement
		Robot_xindx = (float)((float)(Yellow_Robot.getPosition().x - 183) / 53 + 2);
		Robot_yindx = (float)((float)(Yellow_Robot.getPosition().y - 74) / 48 + 2);
		right.i = GetXindx(Yellow_Robot.getPosition().x + 20); right.j = Robot_yindx;
		left.i = GetXindx(Yellow_Robot.getPosition().x - 20); left.j = Robot_yindx;
		up.i = Robot_xindx; up.j = GetYindx(Yellow_Robot.getPosition().y - 7);
		down.i = Robot_xindx; down.j = GetYindx(Yellow_Robot.getPosition().y + 7);
		//######################################################################################################
									// Robot Movement //
		if (Yellow_Robot_Counter >= 250)
		{
			Yellow_Robot_Rand_motion = rand() % 4 + 1;
			Yellow_Robot_Counter = 0;
		}

		if (Yellow_Robot_Rand_motion == 1)                                          // Moving UP => Rand_motion == 1
		{
			Yellow_Robot_Counter++;
			if (Grid[GetXindx(Yellow_Robot.getPosition().x + 25)][up.j].getTexture() == &ground || Grid[up.i][up.j].getTexture() == &ground2)
			{

				Movement_Upward(Yellow_Robot, Yellow_Robot_time, Yellow_Robot_clock, Yellow_Robot_position, Yellow_Robot_size, Yellow_Robot_speed);

			}
			else
			{
				Yellow_Robot_Counter = 250;
			}

		}


		else if (Yellow_Robot_Rand_motion == 2)                                 // Moving Down => Rand_motion == 2
		{
			Yellow_Robot_Counter++;
			if (Grid[GetXindx(Yellow_Robot.getPosition().x + 25)][down.j].getTexture() == &ground || Grid[down.i][down.j].getTexture() == &ground2)
			{
				Movement_Downward(Yellow_Robot, Yellow_Robot_time, Yellow_Robot_clock, Yellow_Robot_position, Yellow_Robot_size, Yellow_Robot_speed);
			}

			else
			{
				Yellow_Robot_Counter = 250;
			}
		}


		else if (Yellow_Robot_Rand_motion == 3)                                 // Moving Right => Rand_motion == 3
		{
			Yellow_Robot_Counter++;
			if (Grid[right.i][right.j].getTexture() == &ground || Grid[right.i][right.j].getTexture() == &ground2)
			{
				Movement_Right(Yellow_Robot, Yellow_Robot_time, Yellow_Robot_clock, Yellow_Robot_position, Yellow_Robot_size, Yellow_Robot_speed);
			}
			else
			{
				Yellow_Robot_Counter = 250;
			}
		}


		else if (Yellow_Robot_Rand_motion == 4)                                 // Moving Left => Rand_motion == 4
		{
			Yellow_Robot_Counter++;
			if (Grid[left.i][left.j].getTexture() == &ground || Grid[left.i][left.j].getTexture() == &ground2)
			{
				Movement_Left(Yellow_Robot, Yellow_Robot_time, Yellow_Robot_clock, Yellow_Robot_position, Yellow_Robot_size, Yellow_Robot_speed);
			}
			else
			{
				Yellow_Robot_Counter = 250;
			}

		}

		// Skeleton movement
		Skeleton_xindx = (float)((float)(Skeleton.getPosition().x - 183) / 53 + 2);
		Skeleton_yindx = (float)((float)(Skeleton.getPosition().y - 74) / 48 + 2);
		right.i = GetXindx(Skeleton.getPosition().x + 20); right.j = Skeleton_yindx;
		left.i = GetXindx(Skeleton.getPosition().x - 20); left.j = Skeleton_yindx;
		up.i = Skeleton_xindx; up.j = GetYindx(Skeleton.getPosition().y - 7);
		down.i = Skeleton_xindx; down.j = GetYindx(Skeleton.getPosition().y + 7);

		// Skeleton Movement
		if (Skeleton_counter >= 250)
		{
			Skeleton_Rand_motion = rand() % 4 + 1;
			Skeleton_counter = 0;
		}

		if (Skeleton_Rand_motion == 1)                                          // Moving UP => Rand_motion == 1
		{
			Skeleton_counter++;
			if (Grid[GetXindx(Skeleton.getPosition().x + 25)][up.j].getTexture() == &ground || Grid[up.i][up.j].getTexture() == &ground2)
			{
				Movement_Upward(Skeleton, Skeleton_time, Skeleton_clock, Skeleton_position, Skeleton_size, Random_Monsters_Speed);
			}
			else
			{
				Skeleton_counter = 250;
			}

		}


		else if (Skeleton_Rand_motion == 2)                                 // Moving Down => Rand_motion == 2
		{
			Skeleton_counter++;
			if (Grid[GetXindx(Skeleton.getPosition().x + 25)][down.j].getTexture() == &ground || Grid[down.i][down.j].getTexture() == &ground2)
			{
				Movement_Downward(Skeleton, Skeleton_time, Skeleton_clock, Skeleton_position, Skeleton_size, Random_Monsters_Speed);
			}

			else
			{
				Skeleton_counter = 250;
			}
		}


		else if (Skeleton_Rand_motion == 3)                                 // Moving Right => Rand_motion == 3
		{
			Skeleton_counter++;
			if (Grid[right.i][right.j].getTexture() == &ground || Grid[right.i][right.j].getTexture() == &ground2)
			{
				Movement_Right(Skeleton, Skeleton_time, Skeleton_clock, Skeleton_position, Skeleton_size, Random_Monsters_Speed);


			}
			else
			{
				Skeleton_counter = 250;
			}
		}


		else if (Skeleton_Rand_motion == 4)                                 // Moving Left => Rand_motion == 4
		{
			Skeleton_counter++;
			if (Grid[left.i][left.j].getTexture() == &ground || Grid[left.i][left.j].getTexture() == &ground2)
			{

				Movement_Left(Skeleton, Skeleton_time, Skeleton_clock, Skeleton_position, Skeleton_size, Random_Monsters_Speed);

			}
			else
			{
				Skeleton_counter = 250;
			}

		}

		//######################################################################################################
		// White_Robot movement
		White_Robot_xindx = (float)((float)(White_Robot.getPosition().x - 183) / 53 + 2);
		White_Robot_yindx = (float)((float)(White_Robot.getPosition().y - 74) / 48 + 2);
		right.i = GetXindx(White_Robot.getPosition().x + 20); right.j = White_Robot_yindx;
		left.i = GetXindx(White_Robot.getPosition().x - 20); left.j = White_Robot_yindx;
		up.i = White_Robot_xindx; up.j = GetYindx(White_Robot.getPosition().y - 7);
		down.i = White_Robot_xindx; down.j = GetYindx(White_Robot.getPosition().y + 7);
		//######################################################################################################

		//######################################################################################################
		// White_Robot Movement
		if (White_Robot_counter >= 250)
		{
			White_Robot_Rand_motion = rand() % 4 + 1;
			White_Robot_counter = 0;
		}

		if (White_Robot_Rand_motion == 1)                                           // Moving UP => Rand_motion == 1
		{
			White_Robot_counter++;
			if (Grid[GetXindx(White_Robot.getPosition().x + 25)][up.j].getTexture() == &ground || Grid[up.i][up.j].getTexture() == &ground2)
			{
				Movement_Upward(White_Robot, White_Robot_time, White_Robot_clock, White_Robot_position, White_Robot_size, Random_Monsters_Speed);
			}
			else
			{
				White_Robot_counter = 250;
			}

		}


		else if (White_Robot_Rand_motion == 2)                                  // Moving Down => Rand_motion == 2
		{
			White_Robot_counter++;
			if (Grid[GetXindx(White_Robot.getPosition().x + 25)][down.j].getTexture() == &ground || Grid[down.i][down.j].getTexture() == &ground2)
			{
				Movement_Downward(White_Robot, White_Robot_time, White_Robot_clock, White_Robot_position, White_Robot_size, Random_Monsters_Speed);
			}

			else
			{
				White_Robot_counter = 250;
			}
		}


		else if (White_Robot_Rand_motion == 3)                                  // Moving Right => Rand_motion == 3
		{
			White_Robot_counter++;
			if (Grid[right.i][right.j].getTexture() == &ground || Grid[right.i][right.j].getTexture() == &ground2)
			{
				Movement_Right(White_Robot, White_Robot_time, White_Robot_clock, White_Robot_position, White_Robot_size, Random_Monsters_Speed);

			}
			else
			{
				White_Robot_counter = 250;
			}
		}


		else if (White_Robot_Rand_motion == 4)                                  // Moving Left => Rand_motion == 4
		{
			White_Robot_counter++;
			if (Grid[left.i][left.j].getTexture() == &ground || Grid[left.i][left.j].getTexture() == &ground2)
			{
				Movement_Left(White_Robot, White_Robot_time, White_Robot_clock, White_Robot_position, White_Robot_size, Random_Monsters_Speed);
			}
			else
			{
				White_Robot_counter = 250;
			}

		}

		//######################################################################################################

		// Player Movement
		Player_xindx = (float)((float)(player.getPosition().x - 183) / 53 + 2);
		Player_yindx = (float)((float)(player.getPosition().y - 74) / 48 + 2);
		right.i = GetXindx(player.getPosition().x + 20); right.j = Player_yindx;
		left.i = GetXindx(player.getPosition().x - 20); left.j = Player_yindx;
		up.i = Player_xindx; up.j = GetYindx(player.getPosition().y - 7);
		down.i = Player_xindx; down.j = GetYindx(player.getPosition().y + 7);

		if (Keyboard::isKeyPressed(Keyboard::Key::Right) && (Grid[right.i][right.j].getTexture() == &ground || Grid[right.i][right.j].getTexture() == &ground2))      // Moving Right
		{
			bool check_bomb = 0;
			for (int io = 0; io < bombs.size(); io++)
			{
				if (bombs[io].x_index == GetXindx(player.getPosition().x + 1) && bombs[io].x_index != GetXindx(player.getPosition().x) && bombs[io].y_index == right.j)
				{
					check_bomb = 1; break;
				}
			}
			if (!check_bomb)
				Movement_Right(player, player_time, player_clock, player_position, player_size, player_speed);
		}
		else if (Keyboard::isKeyPressed(Keyboard::Key::Left) && (Grid[left.i][left.j].getTexture() == &ground || Grid[left.i][left.j].getTexture() == &ground2))         // Moving Left
		{
			bool check_bomb = 0;
			for (int io = 0; io < bombs.size(); io++)
			{
				if (bombs[io].x_index == GetXindx(player.getPosition().x - 1) && bombs[io].x_index != GetXindx(player.getPosition().x) && bombs[io].y_index == left.j)
				{
					check_bomb = 1; break;
				}
			}
			if (!check_bomb)
				Movement_Left(player, player_time, player_clock, player_position, player_size, player_speed);
		}
		else if (Keyboard::isKeyPressed(Keyboard::Key::Up) && (Grid[GetXindx(player.getPosition().x + 25)][up.j].getTexture() == &ground || Grid[up.i][up.j].getTexture() == &ground2))           // Moving Up
		{
			bool check_bomb = 0;
			for (int io = 0; io < bombs.size(); io++)
			{
				if (bombs[io].y_index == GetYindx(player.getPosition().y - 1) && bombs[io].y_index != GetYindx(player.getPosition().y) && bombs[io].x_index == up.i)
				{
					check_bomb = 1; break;
				}
			}
			if (!check_bomb)

				Movement_Upward(player, player_time, player_clock, player_position, player_size, player_speed);
		}
		else if (Keyboard::isKeyPressed(Keyboard::Key::Down) && (Grid[GetXindx(player.getPosition().x + 25)][down.j].getTexture() == &ground || Grid[down.i][down.j].getTexture() == &ground2))         // Moving Down
		{
			bool check_bomb = 0;
			for (int io = 0; io < bombs.size(); io++)
			{
				if (bombs[io].y_index == GetYindx(player.getPosition().y + 1) && bombs[io].y_index != GetYindx(player.getPosition().y) && bombs[io].x_index == up.i)
				{
					check_bomb = 1; break;
				}
			}
			if (!check_bomb)

				Movement_Downward(player, player_time, player_clock, player_position, player_size, player_speed);
		}

		//######################################################################################################

		Event event;
		while (Level_1.pollEvent(event))
		{
			if (event.type == sf::Event::Closed || Keyboard::isKeyPressed(Keyboard::Escape))
				Level_1.close();

		}

		//#####################################################################################################
		// Bombing stuff
		//#####################################################################################################
		// Bombing stuff
			// bombing

		if (bomb_placement_timer < 80)
		{
			bomb_placement_timer++;
		}
		if (Keyboard::isKeyPressed(Keyboard::Key::Space) && bomb_placement_timer == 80 && counter_placed_bombs < boombs) {
			bombs.push_back(bomb_template);
			bombs[bombs.size() - 1].bomb_clock.restart();
			bombs[bombs.size() - 1].explosion_clock.restart();
			bombs[bombs.size() - 1].start_clock.restart();
			bombs[bombs.size() - 1].bomb_placed = 1;
			bombs[bombs.size() - 1].x_pos = GetXpostion(Player_xindx) + 12;
			bombs[bombs.size() - 1].y_pos = GetYpostion(Player_yindx) + 12;
			bombs[bombs.size() - 1].sprt.setPosition(bombs[bombs.size() - 1].x_pos, bombs[bombs.size() - 1].y_pos);
			bombs[bombs.size() - 1].sprt.setTextureRect(IntRect(0, text_size_bomb.y*bombs[bombs.size() - 1].bomb_animation_y, text_size_bomb.x, text_size_bomb.y));
			bombs[bombs.size() - 1].x_index = GetXindx(bombs[bombs.size() - 1].x_pos);
			bombs[bombs.size() - 1].y_index = GetYindx(bombs[bombs.size() - 1].y_pos);
			bomb_placement_timer = 0;
			counter_placed_bombs++;
		}

		for (int io = 0; io < bombs.size(); io++)
		{
			if (bombs[io].bomb_placed == 1) {
				bombs[io].bomb_time = bombs[io].bomb_clock.getElapsedTime();
				bombs[io].start_explosion = bombs[io].start_clock.getElapsedTime();
				if (bombs[io].bomb_time.asSeconds() >= 0.2) {
					bombs[io].bomb_clock.restart();
					bombs[io].sprt.setPosition(bombs[io].x_pos, bombs[io].y_pos + 25);
					bombs[io].sprt.setTextureRect(IntRect(0, text_size_bomb.y*bombs[io].bomb_animation_y, text_size_bomb.x, text_size_bomb.y));
					bombs[io].bomb_animation_y += 2;
					if (bombs[io].bomb_animation_y > 2)
						bombs[io].bomb_animation_y = 0;
				}
				if (bombs[io].start_explosion.asSeconds() >= 3) {
					bombs[io].bomb_exploded = 1;
					bombs[io].bomb_placed = 0;
					//                bombs[number_of_bombs].sprt.setColor(Color::Transparent);
					for (int k = 0; k <= range_of_bomb; k++)
					{
						bombs[io].range_right.push_back(exp);
						bombs[io].range_left.push_back(exp);
						bombs[io].range_up.push_back(exp);
						bombs[io].range_down.push_back(exp);
						//                     v[z].push_back(exp);
					}
				}
			}
			//cout<<player.getPosition().x<<' '<<player.getPosition().y<<endl;
			if (bombs[io].bomb_exploded) {
				bombs[io].explosion_time = bombs[io].explosion_clock.getElapsedTime();
				if (bombs[io].explosion_time.asSeconds() >= 0.08) {
					bombs[io].explosion_clock.restart();
					for (int n = 0, k = 0; k < bombs[io].range_right.size(), n <= range_of_bomb * 50; k++, n += 50)
						bombs[io].range_right[k].setPosition(bombs[io].x_pos + n, bombs[io].y_pos);

					for (int n = 0, k = 0; k < bombs[io].range_right.size(), n <= range_of_bomb * 50; k++, n += 50)
						bombs[io].range_right[k].setTextureRect(IntRect(text_size_exp.x*bombs[io].exp_animation_x, text_size_exp.y*bombs[io].exp_animation_y, text_size_exp.x, text_size_exp.y));

					for (int n = 0, k = 0; k < bombs[io].range_left.size(), n <= range_of_bomb * 50; k++, n += 50)
						bombs[io].range_left[k].setPosition(bombs[io].x_pos - n, bombs[io].y_pos);

					for (int n = 0, k = 0; k < bombs[io].range_left.size(), n <= range_of_bomb * 50; k++, n += 50)
						bombs[io].range_left[k].setTextureRect(IntRect(text_size_exp.x*bombs[io].exp_animation_x, text_size_exp.y*bombs[io].exp_animation_y, text_size_exp.x, text_size_exp.y));

					for (int n = 0, k = 0; k < bombs[io].range_up.size(), n <= range_of_bomb * 50; k++, n += 50)
						bombs[io].range_up[k].setPosition(bombs[io].x_pos, bombs[io].y_pos - n);

					for (int n = 0, k = 0; k < bombs[io].range_up.size(), n <= range_of_bomb * 50; k++, n += 50)
						bombs[io].range_up[k].setTextureRect(IntRect(text_size_exp.x*bombs[io].exp_animation_x, text_size_exp.y*bombs[io].exp_animation_y, text_size_exp.x, text_size_exp.y));

					for (int n = 0, k = 0; k < bombs[io].range_down.size(), n <= range_of_bomb * 50; k++, n += 50)
						bombs[io].range_down[k].setPosition(bombs[io].x_pos, bombs[io].y_pos + n);

					for (int n = 0, k = 0; k < bombs[io].range_down.size(), n <= range_of_bomb * 50; k++, n += 50)
						bombs[io].range_down[k].setTextureRect(IntRect(text_size_exp.x*bombs[io].exp_animation_x, text_size_exp.y*bombs[io].exp_animation_y, text_size_exp.x, text_size_exp.y));

					bombs[io].exp_animation_x++;
					if (bombs[io].exp_animation_x == 3) {
						bombs[io].exp_animation_x = 0, bombs[io].exp_animation_y++;
						if (bombs[io].exp_animation_y == 3) {
							bombs[io].bomb_exploded = 0;
							bombs.erase(bombs.begin());
							counter_placed_bombs--;
						}
					}
				}
			}
		}
		//}
		// Drawing of the Wallpaper
		Level_1.clear();
		Level_1.draw(Wallpaper);
		//}
		// Drawing of the Wallpaper
		Level_1.clear();
		Level_1.draw(Wallpaper);
		//}
		//#####################################################################################################
		bool death_flag = 0;

		

		//Drawing of The Map
		for (int i = 2; i < width; i++)
		{
			for (int j = 2; j < length; j++)
			{
				if (Grid[i][j].getTexture() == &ground || Grid[i][j].getTexture() == &ground2)
				{
					Level_1.draw(Grid[i][j]);
				}
			}

		}

		for (int i = 0; i < 14; i++)
		{
			Level_1.draw(SuperPowers[i]);
		}

		for (int j = 1; j <= length; j++)
		{
			for (int i = 1; i <= width; i++)
			{
				if (Grid[i][j].getTexture() == &ground || Grid[i][j].getTexture() == &ground2)
				{
					continue;
				}

				Level_1.draw(Grid[i][j]);

				if (GetYindx(player.getPosition().y) >= j)
				{
					Level_1.draw(player);
				}

				if (GetYindx(Yellow_Robot.getPosition().y) >= j&&!Yellow_Robot_death)
				{
					Level_1.draw(Yellow_Robot);
				}
				if (GetYindx(Skeleton.getPosition().y) >= j&&!Skeleton_death)
				{
					Level_1.draw(Skeleton);
				}
				if (GetYindx(White_Robot.getPosition().y) >= j&&!White_Robot_death)
				{
					Level_1.draw(White_Robot);
				}
				for (int io = 0; io < bombs.size(); io++)
				{
					if (bombs[io].y_index >= j && bombs[io].bomb_placed)
					{
						Level_1.draw(bombs[io].sprt);
					}
				}

			}
		}
	
		// Drawing Bombs
		for (int io = 0; io < bombs.size(); io++)
		{
			//if (bombs[io].bomb_placed)
				//window.draw(bombs[io].sprt);
			if (bombs[io].bomb_exploded)
			{

				//Right
				for (int g = 0; g < bombs[io].range_right.size(); g++)
				{

					Level_1.draw(bombs[io].range_right[g]);

					if ((Grid[bombs[io].x_index + g][bombs[io].y_index].getTexture() == &breakable) && bombs[io].flag_range_right)
					{
						destroy_breakable_walls(bombs[io].x_index + g, bombs[io].y_index, Grid, ground, ground2);
						bombs[io].flag_range_right = 0;
						break;
					}
					else if (Grid[bombs[io].x_index + g][bombs[io].y_index].getTexture() == &wall)
					{

						bombs[io].flag_range_right = 0;
						break;
					}

					if (bombs[io].x_index + g == Player_xindx && bombs[io].y_index == Player_yindx && bombs[io].flag_range_right)
					{
						if (Clock_Damgae.getElapsedTime().asSeconds() >= 1) {
							lives--;
							ssl = to_string(lives);
							Clock_Damgae.restart();
						}
						Level_1.draw(Hurt);
					}

					if (bombs[io].x_index + g == Robot_xindx && bombs[io].y_index == Robot_yindx && bombs[io].flag_range_right)
					{
						Yellow_Robot_death = 1;
						Yellow_Robot.setFillColor(Color::Transparent);
					}
					if (bombs[io].x_index + g == White_Robot_xindx && bombs[io].y_index == White_Robot_yindx && bombs[io].flag_range_right)
					{
						White_Robot_death = 1;
						White_Robot.setFillColor(Color::Transparent);
					}
					if (bombs[io].x_index + g == Skeleton_xindx && bombs[io].y_index == Skeleton_yindx && bombs[io].flag_range_right)
					{
						Skeleton_death = 1;
						Skeleton.setFillColor(Color::Transparent);
					}
					


				}


				//UP
				for (int g = 0; g < bombs[io].range_up.size(); g++)
				{

					Level_1.draw(bombs[io].range_up[g]);

					if ((Grid[bombs[io].x_index][bombs[io].y_index - g].getTexture() == &breakable) && bombs[io].flag_range_up)
					{


						destroy_breakable_walls(bombs[io].x_index, bombs[io].y_index - g, Grid, ground, ground2);

						bombs[io].flag_range_up = 0;
						break;

					}
					else if (Grid[bombs[io].x_index][bombs[io].y_index - g].getTexture() == &wall)
					{

						bombs[io].flag_range_up = 0;
						break;
					}

					if (bombs[io].x_index == Player_xindx && bombs[io].y_index - g == Player_yindx && bombs[io].flag_range_up)
					{
						if (Clock_Damgae.getElapsedTime().asSeconds() >= 1) {
							lives--;
							ssl = to_string(lives);
							Clock_Damgae.restart();
						}
						Level_1.draw(Hurt);
					}

					if (bombs[io].x_index == Robot_xindx && bombs[io].y_index - g == Robot_yindx && bombs[io].flag_range_up)
					{
						Yellow_Robot_death = 1;
						Yellow_Robot.setFillColor(Color::Transparent);
					}
					if (bombs[io].x_index == White_Robot_xindx && bombs[io].y_index - g == White_Robot_yindx && bombs[io].flag_range_up)
					{
						White_Robot_death = 1;
						White_Robot.setFillColor(Color::Transparent);
					}
					if (bombs[io].x_index == Skeleton_xindx && bombs[io].y_index - g == Skeleton_yindx && bombs[io].flag_range_up)
					{
						Skeleton_death = 1;
						Skeleton.setFillColor(Color::Transparent);
					}
					

				}

				//Left
				for (int g = 0; g < bombs[io].range_left.size(); g++)
				{

					Level_1.draw(bombs[io].range_left[g]);

					if ((Grid[bombs[io].x_index - g][bombs[io].y_index].getTexture() == &breakable) && bombs[io].flag_range_left)
					{
						destroy_breakable_walls(bombs[io].x_index - g, bombs[io].y_index, Grid, ground, ground2);

						bombs[io].flag_range_left = 0;
						break;
					}
					else if (Grid[bombs[io].x_index - g][bombs[io].y_index].getTexture() == &wall)
					{
						bombs[io].flag_range_left = 0;
						break;
					}
					if (bombs[io].x_index - g == Player_xindx && bombs[io].y_index == Player_yindx && bombs[io].flag_range_left)
					{
						if (Clock_Damgae.getElapsedTime().asSeconds() >= 1) {
							lives--;
							ssl = to_string(lives);
							Clock_Damgae.restart();
						}
						Level_1.draw(Hurt);
					}

					if (bombs[io].x_index - g == Robot_xindx - g && bombs[io].y_index == Robot_yindx && bombs[io].flag_range_left)
					{
						Yellow_Robot_death = 1;
						Yellow_Robot.setFillColor(Color::Transparent);
					}
					if (bombs[io].x_index - g == White_Robot_xindx && bombs[io].y_index == White_Robot_yindx && bombs[io].flag_range_left)
					{
						White_Robot_death = 1;
						White_Robot.setFillColor(Color::Transparent);
					}
					if (bombs[io].x_index - g == Skeleton_xindx && bombs[io].y_index == Skeleton_yindx && bombs[io].flag_range_left)
					{
						Skeleton_death = 1;
						Skeleton.setFillColor(Color::Transparent);
					}
					

				}

				//Down
				for (int g = 0; g < bombs[io].range_down.size(); g++)
				{

					Level_1.draw(bombs[io].range_down[g]);
					if ((Grid[bombs[io].x_index][bombs[io].y_index + g].getTexture() == &breakable) && bombs[io].flag_range_down)
					{
						destroy_breakable_walls(bombs[io].x_index, bombs[io].y_index + g, Grid, ground, ground2);
						bombs[io].flag_range_down = 0;
						break;
					}
					else if (Grid[bombs[io].x_index][bombs[io].y_index + g].getTexture() == &wall)
					{

						bombs[io].flag_range_down = 0;
						break;
					}
					if (bombs[io].x_index == Player_xindx && bombs[io].y_index + g == Player_yindx && bombs[io].flag_range_down)
					{
						if (Clock_Damgae.getElapsedTime().asSeconds() >= 1) {
							lives--;
							ssl = to_string(lives);
							Clock_Damgae.restart();
						}
						Level_1.draw(Hurt);
					}

					if (bombs[io].x_index == Robot_xindx && bombs[io].y_index + g == Robot_yindx && bombs[io].flag_range_down)
					{
						Yellow_Robot_death = 1;
						Yellow_Robot.setFillColor(Color::Transparent);
					}
					if (bombs[io].x_index == White_Robot_xindx && bombs[io].y_index + g == White_Robot_yindx && bombs[io].flag_range_down)
					{
						White_Robot_death = 1;
						White_Robot.setFillColor(Color::Transparent);
					}
					if (bombs[io].x_index == Skeleton_xindx && bombs[io].y_index + g == Skeleton_yindx && bombs[io].flag_range_down)
					{
						Skeleton_death = 1;
						Skeleton.setFillColor(Color::Transparent);
					}
					
				}

			}
		}
	

		//############################################################################################//############################################################################################

		for (int i = 0; i < 14; i++) {
			int sumP = 0; int sumR = 0;
			sumP = GetXindx(player.getPosition().x) + GetYindx(player.getPosition().y);
			sumR = GetXindx(SuperPowers[i].getPosition().x) + GetYindx(SuperPowers[i].getPosition().y);

			if (sumR == sumP + 1 && (GetXindx(SuperPowers[i].getPosition().x) == GetXindx(player.getPosition().x))
				&& !visted[GetXindx(SuperPowers[i].getPosition().x)][GetYindx(SuperPowers[i].getPosition().y)])
			{

				visted[GetXindx(SuperPowers[i].getPosition().x)][GetYindx(SuperPowers[i].getPosition().y)] = 1;

				if (SuperPowers[i].getTexture() == &rangeskill) {
					

						temp_range_of_bomb++;
						SuperPowers[i].setColor(Color::Transparent);
						ss = to_string(temp_range_of_bomb);
					
				}
				else if (SuperPowers[i].getTexture() == &bombskill) {
						boombs++;
						SuperPowers[i].setColor(Color::Transparent);
						sss = to_string(boombs);
						
					
				}
				else if (SuperPowers[i].getTexture() == &healskill) {
						lives++;
						SuperPowers[i].setColor(Color::Transparent);
						ssl = to_string(lives);
				}
			}

		}



		bool isbombing = 0;
		for (int io = 0; io < bombs.size(); io++)
		{
			if (bombs[io].bomb_exploded) { isbombing = 1; break; }
		}
		if (!isbombing)
		{
			range_of_bomb = temp_range_of_bomb;
		}
		//############################################################################################


		

		

		

		if ((GetXindx(player.getPosition().x) == GetXindx(White_Robot.getPosition().x) && GetYindx(player.getPosition().y) == GetYindx(White_Robot.getPosition().y)) && !White_Robot_death) {
			if (Clock_Damgae.getElapsedTime().asSeconds() >= 1) {
				lives--;
				ssl = to_string(lives);
				Clock_Damgae.restart();
			}
			Level_1.draw(Hurt);

		}

		if (GetXindx(player.getPosition().x) == GetXindx(Yellow_Robot.getPosition().x) && GetYindx(player.getPosition().y) == GetYindx(Yellow_Robot.getPosition().y) && !Yellow_Robot_death)
		{
			if (Clock_Damgae.getElapsedTime().asSeconds() >= 1) {
				lives--;
				ssl = to_string(lives);
				Clock_Damgae.restart();
			}
			Level_1.draw(Hurt);
		}

		if (GetXindx(player.getPosition().x) == GetXindx(Skeleton.getPosition().x) && GetYindx(player.getPosition().y) == GetYindx(Skeleton.getPosition().y)&&!Skeleton_death)
		{
			if (Clock_Damgae.getElapsedTime().asSeconds() >= 1) {
				lives--;
				ssl = to_string(lives);
				Clock_Damgae.restart();
			}
			Level_1.draw(Hurt);
		}

		Level_1.draw(nooflives);
		Level_1.draw(noofbombs);
		Level_1.draw(noofranges);
		
		Level_1.draw(lifeS);
		Level_1.draw(rangeS);
		Level_1.draw(bombS);
		Level_1.draw(txt); // بيترسم تحت
		if (Player_xindx == 16 && Player_yindx == 10) {
			//ingame.stop();
			Level_1.clear(Color::Black), Level_1.draw(Level_complete);
			--elapsed;
			if (elapsed <= 1) {
				exit(0);
			}
		}
		
		if (lives < 1) {
			ingame.stop();
			player.setFillColor(Color::Transparent);
			Tomb.setPosition(player.getPosition().x, player.getPosition().y);
			Level_1.draw(Tomb);
			--elapsed;
			if (elapsed <= 50+500) {
				if (playsong) {
					lost.play();
					playsong = 0;
				}
				if (elapsed <= -500) {
					Level_1.clear(Color::Black), Level_1.draw(RIP_Photo);
					exit(0);
				}
			}
		}

		if (Die) {
			ingame.stop();
			Level_1.clear(Color::Black), Level_1.draw(big_exp), Level_1.draw(RIP_Photo);
			--elapsed;
			if (elapsed <= 1) {
				exit(0);
			}
		}
		
		
		Level_1.display();


	}

}
void Advanced_Level()
{
	////////////////////////////////////////////////////////
	int big_pos_x = 1, big_pos_y = 0; //بيتعرف فوق
	Texture big_exp_text;
	big_exp_text.loadFromFile("exp.png");
	Sprite big_exp,RIP_photo,Tomb;
	big_exp.setTexture(big_exp_text);
	Vector2u text_size_big_exp = big_exp_text.getSize();
	text_size_big_exp.x /= 3;
	text_size_big_exp.y /= 3;
	big_exp.setScale(3, 3);
	Clock start_clock, timer_clock, clock_shutdown;
	Time start_timer, timer, timer_shutdown;
	int display_timer, minutes = 1;
	Text txt;
	Texture death_texture,tomb;
	tomb.loadFromFile("RIP.png");
	Tomb.setTexture(tomb);
	death_texture.loadFromFile("death.png");
	Font dis_time;
	if (!dis_time.loadFromFile("Chocolate Bar Demo.otf"))
	{
		cout << "error font";
	}
	RIP_photo.setTexture(death_texture);
	Tomb.setScale(0.1, 0.1);
	Texture Complete;
	Complete.loadFromFile("level-completed.png");
	Sprite Level_complete;
	Level_complete.setTexture(Complete);
	////////////////////////////////////////////////////////
	Music ingame, lost; 
	ingame.openFromFile("ingame.wav");
	lost.openFromFile("For the Damaged Coda.wav");
	ingame.play();
	ingame.setLoop(1);

	//======================================================================//
						// Making of Player // 
	RectangleShape player(Vector2f(100, 120));
	Texture player_texture;
	Vector2u player_size;
	Clock player_clock;
	Time player_time;
	float player_position = 0;
	const int player_speed = 2;
	Generate_Charcter(player, player_texture, player_size, "mainplayer.png", 2, 2);
	//======================================================================//
	Sprite timebombS, SuperPowers[19];
	Texture timebomb;
	Texture healskill, bombskill, rangeskill;
	timebomb.loadFromFile("time-bomb.png");
	healskill.loadFromFile("Heal .png");
	bombskill.loadFromFile("bomb skill .png");
	rangeskill.loadFromFile("Range skill.png");
	for (int i = 0; i < 5; i++)
	{

		SuperPowers[i].setTexture(healskill);			// Rand_Choice 1 => Bombing
		SuperPowers[i].setScale(0.5, 0.3);

	}
	for (int i = 5; i < 13; i++)
	{
		SuperPowers[i].setTexture(bombskill);		// Rand_Choice 2 => Health
		SuperPowers[i].setScale(0.5, 0.3);
	}

	for (int i = 13; i < 16; i++)
	{
		SuperPowers[i].setTexture(rangeskill);				// Rand_Choice 3 => Range
		SuperPowers[i].setScale(0.5, 0.3);
	}
	SuperPowers[18].setTexture(timebomb);				// Rand_Choice 3 => Range
	SuperPowers[18].setScale(0.05, 0.05);
	SuperPowers[0].setPosition(GetXpostion(16) + 20, GetYpostion(4));
	SuperPowers[1].setPosition(GetXpostion(14) + 20, GetYpostion(6));
	//SuperPowers[2].setPosition(GetXpostion(8) + 20, GetYpostion(8));
	SuperPowers[3].setPosition(GetXpostion(8) + 20, GetYpostion(10));
	SuperPowers[4].setPosition(GetXpostion(8) + 20, GetYpostion(5));
	SuperPowers[5].setPosition(GetXpostion(14) + 20, GetYpostion(10));
	SuperPowers[6].setPosition(GetXpostion(16) + 20, GetYpostion(6));
	SuperPowers[7].setPosition(GetXpostion(4) + 20, GetYpostion(6));
	SuperPowers[8].setPosition(GetXpostion(4) + 20, GetYpostion(7));
	SuperPowers[9].setPosition(GetXpostion(16) + 20, GetYpostion(8));
	SuperPowers[10].setPosition(GetXpostion(6) + 20, GetYpostion(10));
	SuperPowers[11].setPosition(GetXpostion(2) + 20, GetYpostion(8));
	SuperPowers[12].setPosition(GetXpostion(6) + 20, GetYpostion(6));
	SuperPowers[13].setPosition(GetXpostion(6) + 20, GetYpostion(5));
	//SuperPowers[14].setPosition(GetXpostion(16) + 20, GetYpostion(4));
	SuperPowers[15].setPosition(GetXpostion(12) + 20, GetYpostion(11));
	SuperPowers[16].setPosition(GetXpostion(16) + 20, GetYpostion(11));
	SuperPowers[17].setPosition(GetXpostion(12) + 20, GetYpostion(10));
	SuperPowers[18].setPosition(GetXpostion(16) + 15, GetYpostion(11));
	
	//======================================================================//
						// Player abilities //

	// Making of Grid
	Texture wall, ground, wallpaper, ground2, breakable, life, bomb_frame, range, hurt, choice;
	if (!breakable.loadFromFile("breakable.png"))
	{
		cout << "error loading breakable wall pic";
	}
	if (!wallpaper.loadFromFile("wallpaper.jpg"))
	{
		cout << "error loading wallpaper pic";
	}
	if (!wall.loadFromFile("3d wall.png"))
	{
		cout << "error loading wall pic";
	}

	if (!ground.loadFromFile("ground 1.png"))
	{
		cout << "error loading ground pic";
	}
	if (!ground2.loadFromFile("grass.png"))
	{
		cout << "error loading grass pic";
	}

	if (!life.loadFromFile("flife.png"))
	{
		cout << "error loading life pic";
	}
	if (!range.loadFromFile("frange.png"))
	{

		cout << "error loading range pic";
	}
	if (!bomb_frame.loadFromFile("fbomb.png"))
	{
		cout << "error loading bomb pic";
	}

	if (!hurt.loadFromFile("hurt.png"))
	{
		cout << "error loading  hurt pic";
	}
	


	life.setSmooth(1);
	bomb_frame.setSmooth(1);
	hurt.setSmooth(1);
	range.setSmooth(1);
	//======================================================================//
	// player stuff 
	
	Clock Clock_Damgae;
	Sprite lifeS, bombS, rangeS, Hurt;
	lifeS.setTexture(life);
	lifeS.setPosition(0, 250);
	lifeS.setScale(0.4f, 0.4f);

	rangeS.setTexture(range);
	rangeS.setPosition(0, 180);
	rangeS.setScale(0.4f, 0.4f);

	bombS.setTexture(bomb_frame);
	bombS.setPosition(0, 320);
	bombS.setScale(0.4f, 0.4f);

	Hurt.setTexture(hurt);

	//======================================================================//

	//======================================================================//
					 // Making of Yellow Robot // 

	RectangleShape Yellow_Robot(Vector2f(100, 120));
	Texture Yellow_Robot_text;
	Vector2u Yellow_Robot_size;
	Clock Yellow_Robot_clock;
	Time Yellow_Robot_time;
	float Yellow_Robot_speed = 1;
	float Yellow_Robot_position = 0;
	int Yellow_Robot_Counter = 0;
	int Yellow_Robot_Rand_motion = rand() % 4 + 1;
	Generate_Charcter(Yellow_Robot, Yellow_Robot_text, Yellow_Robot_size, "yellow_robot.png", 10, 7);
	//======================================================================//


	//======================================================================//
					  // Making of White Robot // 

	RectangleShape  White_Robot(Vector2f(100, 120));
	Texture  White_Robot_text;
	Vector2u  White_Robot_size;
	Clock  White_Robot_clock;
	Time  White_Robot_time;
	float White_Robot_speed = 1;
	float White_Robot_position = 0;
	int White_Robot_Counter = 0;
	int White_Robot_Rand_motion = rand() % 4 + 1;
	Generate_Charcter(White_Robot, White_Robot_text, White_Robot_size, "white_robot.png", 14, 3);
	//======================================================================//


	//======================================================================//
				// Making of  Blue Skeleton  //

	RectangleShape Blue_Skeleton(Vector2f(100, 120));
	Texture Blue_Skeleton_text;
	Vector2u Blue_Skeleton_size;
	Clock Blue_Skeleton_clock;
	Time Blue_Skeleton_time;
	float Blue_Skeleton_speed = 0.5;
	float Blue_Skeleton_position = 0;
	Generate_Charcter(Blue_Skeleton, Blue_Skeleton_text, Blue_Skeleton_size, "smart_skeleton.png", 3, 8);
	//======================================================================//



	//======================================================================//
					// Making of White Skeleton //  

	RectangleShape White_Skeleton(Vector2f(100, 120));
	Texture White_Skeleton_text;
	Vector2u White_Skeleton_text_size;
	Clock White_Skeleton_clock;
	Time White_Skeleton_time;
	float White_Skeleton_speed = 1;
	float  White_Skeleton_position = 0;
	int White_Skeleton_Counter = 0;
	int White_Skeleton_Rand_motion = rand() % 4 + 1;
	Generate_Charcter(White_Skeleton, White_Skeleton_text, White_Skeleton_text_size, "monster (5).png", 10, 3);
	//======================================================================//


	//###############################################################################
	// Movement stuff
	leftmovement left; rightmovement right; upmovement up; downmovement down;
	int Player_xindx = 0, Player_yindx = 0, White_Robot_xindx = 0, White_Robot_yindx = 0, Skeleton_xindx = 0, Skeleton_yindx = 0, Robot_xindx = 0, Robot_yindx = 0;
	//###############################################################################




					   // Making of Grid //
	//======================================================================//
					   // MAP DEFINATION //

	Sprite Wallpaper;
	Wallpaper.setTexture(wallpaper);
	//======================================================================//

	//======================================================================//
		  // Distribution of wall ,breakable and ground on the Grid // 


	Sprite Grid[width + 1][length + 1];

	for (int i = 1; i <= width; i++)      //  Distribution of wall and ground
	{
		for (int j = 1; j <= length; j++)
		{
			if (i % 2 != 0 && j % 2 != 0)
			{

				Grid[i][j].setTexture(wall);
				Grid[i][j].setPosition((i + 1.97)*53.15, (j + 0.255) * 48);
				Grid[i][j].setScale(Vector2f(0.1009, 0.165));

			}

			else if (i == 1 || i == width)
			{
				Grid[i][j].setTexture(wall);
				Grid[i][j].setPosition((i + 1.97)*53.15, (j + 0.255) * 48);
				Grid[i][j].setScale(Vector2f(0.1009, 0.165));
			}
			else if (j == 1 || j == length)
			{
				Grid[i][j].setTexture(wall);
				Grid[i][j].setPosition((i + 1.97)*53.15, (j + 0.255) * 48);
				Grid[i][j].setScale(Vector2f(0.1009, 0.165));
			}
			else
			{
				if (i % 2 == j % 2) { Grid[i][j].setTexture(ground); }
				else { Grid[i][j].setTexture(ground2); }
				Grid[i][j].setPosition((i + 2) * 53, (j + 0.8) * 48);
				Grid[i][j].setScale(0.1, 0.101);
			}
		}
	}

	for (int i = 1; i <= 40; i++)		//	Distribution of breakable
	{
		int x = rand() % 15 + 2, y = rand() % 9 + 2;
		if (x % 2 != 0 && y % 2 != 0)
		{
			x++;
		}
		Grid[x][y].setTexture(breakable);
		Grid[x][y].setPosition((x + 1.97)*53.15, (y + 0.255) * 48);
		Grid[x][y].setScale(Vector2f(0.1009, 0.165));
	}



	//======================================================================//

	//###############################################################################
				//Player Bomb stuff //
	Sprite bombing, exp;
	bombing.setScale(0.2f, 0.2f);
	exp.setScale(0.25, 0.25);
	Texture playertext, player2text, bombtext, exptext;
	playertext.loadFromFile("mainplayer.png");
	player2text.loadFromFile("blue.png");
	bombtext.loadFromFile("BOOM.png");
	exptext.loadFromFile("exp.png");
	bombing.setTexture(bombtext);
	exp.setTexture(exptext);
	Vector2u text_size_bomb = bombtext.getSize();
	Vector2u text_size_exp = exptext.getSize();
	text_size_bomb.y /= 4;
	text_size_exp.x /= 3;
	text_size_exp.y /= 3;
	Time times2, times, t, ti, bomb_timer;
	Clock clocks2, c, cs, clocks, clock_timer;

	bomb bomb_template;
	bomb_template.sprt.setTexture(bombtext);
	bomb_template.sprt.setScale(0.15f, 0.15f);
	vector<bomb>bombs;
	int number_of_bombs = 0, range_of_bomb = 3, bomb_placement_timer = 0, superpower_of_bomb = 2, counter_placed_bombs=0;
	//###############################################################################
	
	bool Yellow_Robot_death = 0, White_Robot_death = 0, White_Skeleton_death =0, Blue_Skeleton_death =0;
	int lives = 3;
	int boombs = 1;
	Clock counter;
	Time elaps = seconds(200);
	int elapsed = elaps.asSeconds();

	int temp_range_of_bomb = 3;
	//=======================================================================================//

	RenderWindow window(VideoMode(1280, 768), "Advanced Level");

	while (window.isOpen())
	{
		//=======================================================================================//
		timer = timer_clock.getElapsedTime();
		display_timer = timer.asSeconds();
		string s1, s2;
		//int  minu=display_timer/60;
		s1 = to_string(59 - (display_timer));
		s2 = to_string(minutes);
		s2 += ":";
		//s2=to_string(1-minu);
		//s2+=" : ";
		//s2+=s1;
		txt.setFont(dis_time);
		txt.setString(s2 + s1);
		txt.setPosition(512, 22);
		if (display_timer > 59 && minutes > 0)
		{
			minutes--;
			timer_clock.restart();
		}
		if (minutes == 0 && display_timer == 59) {
			Die = 1;
			ispressed = 1;
		}
		if (ispressed) {
			start_timer = start_clock.getElapsedTime();
			if (start_timer.asSeconds() >= 0.2) {
				start_clock.restart();
				big_exp.setPosition((window.getPosition().x / 2) - 25, (window.getPosition().y / 2) - 210);
				big_exp.setTextureRect(IntRect(text_size_big_exp.x*big_pos_x, text_size_big_exp.y*big_pos_y, text_size_big_exp.x, text_size_big_exp.y));

				big_pos_x++;
				if (big_pos_x == 3) {
					big_pos_x = 0, big_pos_y++;
					if (big_pos_y == 2 && big_pos_x == 2) {
						ispressed = 0;
						big_exp.setColor(Color::Transparent);
					}
				}
			}
		}
		//=======================================================================================//
		// player status
		string ss = to_string(temp_range_of_bomb);

		Text noofranges;
		noofranges.setFont(font);
		noofranges.setString(ss);
		noofranges.setCharacterSize(45);
		noofranges.setPosition(90, 195);

		string sss = to_string(boombs);

		Text noofbombs;
		noofbombs.setFont(font);
		noofbombs.setString(sss);
		noofbombs.setCharacterSize(50);
		noofbombs.setPosition(90, 320);

		string ssl = to_string(lives);

		Text nooflives;
		nooflives.setFont(font);
		nooflives.setString(ssl);
		nooflives.setCharacterSize(50);
		nooflives.setPosition(90, 255);

		//=======================================================================================//
		// indxex of all monsters
		Robot_xindx = GetXindx(Yellow_Robot.getPosition().x), Robot_yindx = GetYindx(Yellow_Robot.getPosition().y);
		White_Robot_xindx = GetXindx(White_Robot.getPosition().x), White_Robot_yindx = GetYindx(White_Robot.getPosition().y);
		Skeleton_xindx = GetXindx(White_Skeleton.getPosition().x), Skeleton_yindx = GetYindx(White_Skeleton.getPosition().y);

		//=======================================================================================//
							// Random Movment of Yellow Robot // 
		if (Yellow_Robot_Counter >= 5000)
		{
			Yellow_Robot_Rand_motion = rand() % 4 + 1;
			Yellow_Robot_Counter = 0;
		}

		if (Yellow_Robot_Rand_motion == 1)											// Moving UP => Rand_motion == 1
		{
			Yellow_Robot_Counter++;
			if (Grid[GetXindx(Yellow_Robot.getPosition().x + 25)][GetYindx(Yellow_Robot.getPosition().y - 7)].getTexture() == &ground || Grid[GetXindx(Yellow_Robot.getPosition().x)][GetYindx(Yellow_Robot.getPosition().y - 7)].getTexture() == &ground2)
			{
				Movement_Upward(Yellow_Robot, Yellow_Robot_time, Yellow_Robot_clock, Yellow_Robot_position, Yellow_Robot_size, Yellow_Robot_speed);
			}
			else
			{
				Yellow_Robot_Counter = 5000;
			}

		}
		else if (Yellow_Robot_Rand_motion == 2)									// Moving Down => Rand_motion == 2
		{
			Yellow_Robot_Counter++;
			if (Grid[GetXindx(Yellow_Robot.getPosition().x + 25)][GetYindx(Yellow_Robot.getPosition().y + 7)].getTexture() == &ground || Grid[GetXindx(Yellow_Robot.getPosition().x)][GetYindx(Yellow_Robot.getPosition().y + 7)].getTexture() == &ground2)
			{
				Movement_Downward(Yellow_Robot, Yellow_Robot_time, Yellow_Robot_clock, Yellow_Robot_position, Yellow_Robot_size, Yellow_Robot_speed);
			}

			else
			{
				Yellow_Robot_Counter = 5000;
			}
		}


		else if (Yellow_Robot_Rand_motion == 3)									// Moving Right => Rand_motion == 3
		{
			Yellow_Robot_Counter++;
			if (Grid[GetXindx(Yellow_Robot.getPosition().x + 12)][GetYindx(Yellow_Robot.getPosition().y)].getTexture() == &ground || Grid[GetXindx(Yellow_Robot.getPosition().x + 12)][GetYindx(Yellow_Robot.getPosition().y)].getTexture() == &ground2)
			{
				Movement_Right(Yellow_Robot, Yellow_Robot_time, Yellow_Robot_clock, Yellow_Robot_position, Yellow_Robot_size, Yellow_Robot_speed);
			}
			else
			{
				Yellow_Robot_Counter = 5000;
			}
		}


		else if (Yellow_Robot_Rand_motion == 4)									// Moving Left => Rand_motion == 4
		{
			Yellow_Robot_Counter++;
			if (Grid[GetXindx(Yellow_Robot.getPosition().x - 12)][GetYindx(Yellow_Robot.getPosition().y)].getTexture() == &ground || Grid[GetXindx(Yellow_Robot.getPosition().x - 12)][GetYindx(Yellow_Robot.getPosition().y)].getTexture() == &ground2)
			{
				Movement_Left(Yellow_Robot, Yellow_Robot_time, Yellow_Robot_clock, Yellow_Robot_position, Yellow_Robot_size, Yellow_Robot_speed);
			}
			else
			{
				Yellow_Robot_Counter = 5000;
			}

		}
		//=======================================================================================//

		//=======================================================================================//
								// Random Movment of White Robot // 
		if (White_Robot_Counter >= 5000)
		{
			White_Robot_Rand_motion = rand() % 4 + 1;
			White_Robot_Counter = 0;
		}

		if (White_Robot_Rand_motion == 1)											// Moving UP => Rand_motion == 1
		{
			White_Robot_Counter++;
			if (Grid[GetXindx(White_Robot.getPosition().x + 25)][GetYindx(White_Robot.getPosition().y - 7)].getTexture() == &ground || Grid[GetXindx(White_Robot.getPosition().x)][GetYindx(White_Robot.getPosition().y - 7)].getTexture() == &ground2)
			{
				Movement_Upward(White_Robot, White_Robot_time, White_Robot_clock, White_Robot_position, White_Robot_size, White_Robot_speed);
			}
			else
			{
				White_Robot_Counter = 5000;
			}

		}
		else if (White_Robot_Rand_motion == 2)									// Moving Down => Rand_motion == 2
		{
			White_Robot_Counter++;
			if (Grid[GetXindx(White_Robot.getPosition().x + 25)][GetYindx(White_Robot.getPosition().y + 7)].getTexture() == &ground || Grid[GetXindx(White_Robot.getPosition().x)][GetYindx(White_Robot.getPosition().y + 7)].getTexture() == &ground2)
			{
				Movement_Downward(White_Robot, White_Robot_time, White_Robot_clock, White_Robot_position, White_Robot_size, White_Robot_speed);
			}

			else
			{
				White_Robot_Counter = 5000;
			}
		}


		else if (White_Robot_Rand_motion == 3)									// Moving Right => Rand_motion == 3
		{
			White_Robot_Counter++;
			if (Grid[GetXindx(White_Robot.getPosition().x + 12)][GetYindx(White_Robot.getPosition().y)].getTexture() == &ground || Grid[GetXindx(White_Robot.getPosition().x + 12)][GetYindx(White_Robot.getPosition().y)].getTexture() == &ground2)
			{
				Movement_Right(White_Robot, White_Robot_time, White_Robot_clock, White_Robot_position, White_Robot_size, White_Robot_speed);
			}
			else
			{
				White_Robot_Counter = 5000;
			}
		}


		else if (White_Robot_Rand_motion == 4)									// Moving Left => Rand_motion == 4
		{
			White_Robot_Counter++;
			if (Grid[GetXindx(White_Robot.getPosition().x - 12)][GetYindx(White_Robot.getPosition().y)].getTexture() == &ground || Grid[GetXindx(White_Robot.getPosition().x - 12)][GetYindx(White_Robot.getPosition().y)].getTexture() == &ground2)
			{
				Movement_Left(White_Robot, White_Robot_time, White_Robot_clock, White_Robot_position, White_Robot_size, White_Robot_speed);
			}
			else
			{
				White_Robot_Counter = 5000;
			}

		}
		//=======================================================================================//


		//=======================================================================================//
							// Random Movment of White Skeleton //  
		if (White_Skeleton_Counter >= 5000)
		{
			White_Skeleton_Rand_motion = rand() % 4 + 1;
			White_Skeleton_Counter = 0;
		}

		if (White_Skeleton_Rand_motion == 1)											// Moving UP => Rand_motion == 1
		{
			White_Skeleton_Counter++;
			if (Grid[GetXindx(White_Skeleton.getPosition().x + 25)][GetYindx(White_Skeleton.getPosition().y - 7)].getTexture() == &ground || Grid[GetXindx(White_Skeleton.getPosition().x)][GetYindx(White_Skeleton.getPosition().y - 7)].getTexture() == &ground2)
			{
				Movement_Upward(White_Skeleton, White_Skeleton_time, White_Skeleton_clock, White_Skeleton_position, White_Skeleton_text_size, White_Skeleton_speed);
			}
			else
			{
				White_Skeleton_Counter = 5000;
			}

		}
		else if (White_Skeleton_Rand_motion == 2)									// Moving Down => Rand_motion == 2
		{
			White_Skeleton_Counter++;
			if (Grid[GetXindx(White_Skeleton.getPosition().x + 25)][GetYindx(White_Skeleton.getPosition().y + 7)].getTexture() == &ground || Grid[GetXindx(White_Skeleton.getPosition().x)][GetYindx(White_Skeleton.getPosition().y + 7)].getTexture() == &ground2)
			{
				Movement_Downward(White_Skeleton, White_Skeleton_time, White_Skeleton_clock, White_Skeleton_position, White_Skeleton_text_size, White_Skeleton_speed);
			}

			else
			{
				White_Skeleton_Counter = 5000;
			}
		}


		else if (White_Skeleton_Rand_motion == 3)									// Moving Right => Rand_motion == 3
		{
			White_Skeleton_Counter++;
			if (Grid[GetXindx(White_Skeleton.getPosition().x + 12)][GetYindx(White_Skeleton.getPosition().y)].getTexture() == &ground || Grid[GetXindx(White_Skeleton.getPosition().x + 12)][GetYindx(White_Skeleton.getPosition().y)].getTexture() == &ground2)
			{
				Movement_Right(White_Skeleton, White_Skeleton_time, White_Skeleton_clock, White_Skeleton_position, White_Skeleton_text_size, White_Skeleton_speed);
			}
			else
			{
				White_Skeleton_Counter = 5000;
			}
		}


		else if (White_Skeleton_Rand_motion == 4)									// Moving Left => Rand_motion == 4
		{
			White_Skeleton_Counter++;
			if (Grid[GetXindx(White_Skeleton.getPosition().x - 12)][GetYindx(White_Skeleton.getPosition().y)].getTexture() == &ground || Grid[GetXindx(White_Skeleton.getPosition().x - 12)][GetYindx(White_Skeleton.getPosition().y)].getTexture() == &ground2)
			{
				Movement_Left(White_Skeleton, White_Skeleton_time, White_Skeleton_clock, White_Skeleton_position, White_Skeleton_text_size, White_Skeleton_speed);
			}
			else
			{
				White_Skeleton_Counter = 5000;
			}

		}
		//=======================================================================================//


		//=======================================================================================//
							// BFS Movement of Blue Skeleton 

		BFS(GetXindx(player.getPosition().x), GetYindx(player.getPosition().y), GetXindx(Blue_Skeleton.getPosition().x), GetYindx(Blue_Skeleton.getPosition().y), Grid, ground, ground2);
		if (Smart_Skeleton_Movement.i == GetXindx(Blue_Skeleton.getPosition().x) && Smart_Skeleton_Movement.j - GetYindx(Blue_Skeleton.getPosition().y) == 1)
		{
			Movement_Downward(Blue_Skeleton, Blue_Skeleton_time, Blue_Skeleton_clock, Blue_Skeleton_position, Blue_Skeleton_size, Blue_Skeleton_speed);	// Moving Downward
		}
		else if (Smart_Skeleton_Movement.i == GetXindx(Blue_Skeleton.getPosition().x) && Smart_Skeleton_Movement.j - GetYindx(Blue_Skeleton.getPosition().y) == -1)
		{
			Movement_Upward(Blue_Skeleton, Blue_Skeleton_time, Blue_Skeleton_clock, Blue_Skeleton_position, Blue_Skeleton_size, Blue_Skeleton_speed);		// Moving Upward
		}
		else if (Smart_Skeleton_Movement.i - GetXindx(Blue_Skeleton.getPosition().x) == 1 && Smart_Skeleton_Movement.j == GetYindx(Blue_Skeleton.getPosition().y))
		{
			Movement_Right(Blue_Skeleton, Blue_Skeleton_time, Blue_Skeleton_clock, Blue_Skeleton_position, Blue_Skeleton_size, Blue_Skeleton_speed);		// Moving Right
		}
		else if (Smart_Skeleton_Movement.i - GetXindx(Blue_Skeleton.getPosition().x) == -1 && Smart_Skeleton_Movement.j == GetYindx(Blue_Skeleton.getPosition().y))
		{
			Movement_Left(Blue_Skeleton, Blue_Skeleton_time, Blue_Skeleton_clock, Blue_Skeleton_position, Blue_Skeleton_size, Blue_Skeleton_speed);		// Moving Left
		}

		//=======================================================================================//

			//######################################################################################################

		// Player Movement
		Player_xindx = (float)((float)(player.getPosition().x - 183) / 53 + 2);
		Player_yindx = (float)((float)(player.getPosition().y - 74) / 48 + 2);
		right.i = GetXindx(player.getPosition().x + 20); right.j = Player_yindx;
		left.i = GetXindx(player.getPosition().x - 20); left.j = Player_yindx;
		up.i = Player_xindx; up.j = GetYindx(player.getPosition().y - 7);
		down.i = Player_xindx; down.j = GetYindx(player.getPosition().y + 7);

		if (Keyboard::isKeyPressed(Keyboard::Key::Right) && (Grid[right.i][right.j].getTexture() == &ground || Grid[right.i][right.j].getTexture() == &ground2))      // Moving Right
		{
			bool check_bomb = 0;
			for (int io = 0; io < bombs.size(); io++)
			{
				if (bombs[io].x_index == GetXindx(player.getPosition().x + 1) && bombs[io].x_index != GetXindx(player.getPosition().x) && bombs[io].y_index == right.j)
				{
					check_bomb = 1; break;
				}
			}
			if (!check_bomb)
				Movement_Right(player, player_time, player_clock, player_position, player_size, player_speed);
		}
		else if (Keyboard::isKeyPressed(Keyboard::Key::Left) && (Grid[left.i][left.j].getTexture() == &ground || Grid[left.i][left.j].getTexture() == &ground2))         // Moving Left
		{
			bool check_bomb = 0;
			for (int io = 0; io < bombs.size(); io++)
			{
				if (bombs[io].x_index == GetXindx(player.getPosition().x - 1) && bombs[io].x_index != GetXindx(player.getPosition().x) && bombs[io].y_index == left.j)
				{
					check_bomb = 1; break;
				}
			}
			if (!check_bomb)
				Movement_Left(player, player_time, player_clock, player_position, player_size, player_speed);
		}
		else if (Keyboard::isKeyPressed(Keyboard::Key::Up) && (Grid[GetXindx(player.getPosition().x + 25)][up.j].getTexture() == &ground || Grid[up.i][up.j].getTexture() == &ground2))           // Moving Up
		{
			bool check_bomb = 0;
			for (int io = 0; io < bombs.size(); io++)
			{
				if (bombs[io].y_index == GetYindx(player.getPosition().y - 1) && bombs[io].y_index != GetYindx(player.getPosition().y) && bombs[io].x_index == up.i)
				{
					check_bomb = 1; break;
				}
			}
			if (!check_bomb)


				Movement_Upward(player, player_time, player_clock, player_position, player_size, player_speed);
		}
		else if (Keyboard::isKeyPressed(Keyboard::Key::Down) && (Grid[GetXindx(player.getPosition().x + 25)][down.j].getTexture() == &ground || Grid[down.i][down.j].getTexture() == &ground2))         // Moving Down
		{
			bool check_bomb = 0;
			for (int io = 0; io < bombs.size(); io++)
			{
				if (bombs[io].y_index == GetYindx(player.getPosition().y + 1) && bombs[io].y_index != GetYindx(player.getPosition().y) && bombs[io].x_index == up.i)
				{
					check_bomb = 1; break;
				}
			}
			if (!check_bomb)

				Movement_Downward(player, player_time, player_clock, player_position, player_size, player_speed);
		}

		//######################################################################################################
		//=======================================================================================//


		//=======================================================================================//
		Event event;
		while (window.pollEvent(event))
		{
			if (event.type == sf::Event::Closed || Keyboard::isKeyPressed(Keyboard::Escape))
				window.close();
		}

		//#####################################################################################################
		// Bombing stuff 
		//#####################################################################################################
		// Bombing stuff
			// bombing

		if (bomb_placement_timer < 80)
		{
			bomb_placement_timer++;
		}
		if (Keyboard::isKeyPressed(Keyboard::Key::Space) && bomb_placement_timer == 80&& counter_placed_bombs < boombs) {
			bombs.push_back(bomb_template);
			bombs[bombs.size() - 1].bomb_clock.restart();
			bombs[bombs.size() - 1].explosion_clock.restart();
			bombs[bombs.size() - 1].start_clock.restart();
			bombs[bombs.size() - 1].bomb_placed = 1;
			bombs[bombs.size() - 1].x_pos = GetXpostion(Player_xindx) + 12;
			bombs[bombs.size() - 1].y_pos = GetYpostion(Player_yindx) + 12;
			bombs[bombs.size() - 1].x_index = GetXindx(bombs[bombs.size() - 1].x_pos);
			bombs[bombs.size() - 1].y_index = GetYindx(bombs[bombs.size() - 1].y_pos);
			bombs[bombs.size() - 1].sprt.setPosition(bombs[bombs.size() - 1].x_pos, bombs[bombs.size() - 1].y_pos);
			bombs[bombs.size() - 1].sprt.setTextureRect(IntRect(0, text_size_bomb.y*bombs[bombs.size() - 1].bomb_animation_y, text_size_bomb.x, text_size_bomb.y));
			bomb_placement_timer = 0;
			counter_placed_bombs++;
		}

		for (int io = 0; io < bombs.size(); io++)
		{
			if (bombs[io].bomb_placed == 1) {
				bombs[io].bomb_time = bombs[io].bomb_clock.getElapsedTime();
				bombs[io].start_explosion = bombs[io].start_clock.getElapsedTime();
				if (bombs[io].bomb_time.asSeconds() >= 0.2) {
					bombs[io].bomb_clock.restart();
					bombs[io].sprt.setPosition(bombs[io].x_pos, bombs[io].y_pos + 25);
					bombs[io].sprt.setTextureRect(IntRect(0, text_size_bomb.y*bombs[io].bomb_animation_y, text_size_bomb.x, text_size_bomb.y));
					bombs[io].bomb_animation_y += 2;
					if (bombs[io].bomb_animation_y > 2)
						bombs[io].bomb_animation_y = 0;
				}
				if (bombs[io].start_explosion.asSeconds() >= 3) {
					bombs[io].bomb_exploded = 1;
					bombs[io].bomb_placed = 0;
					//                bombs[number_of_bombs].sprt.setColor(Color::Transparent);
					for (int k = 0; k <= range_of_bomb; k++)
					{
						bombs[io].range_right.push_back(exp);
						bombs[io].range_left.push_back(exp);
						bombs[io].range_up.push_back(exp);
						bombs[io].range_down.push_back(exp);
						//                     v[z].push_back(exp);
					}
				}
			}
			//cout<<player.getPosition().x<<' '<<player.getPosition().y<<endl;
			if (bombs[io].bomb_exploded) {
				bombs[io].explosion_time = bombs[io].explosion_clock.getElapsedTime();
				if (bombs[io].explosion_time.asSeconds() >= 0.08) {
					bombs[io].explosion_clock.restart();
					for (int n = 0, k = 0; k < bombs[io].range_right.size(), n <= range_of_bomb * 50; k++, n += 50)
						bombs[io].range_right[k].setPosition(bombs[io].x_pos + n, bombs[io].y_pos);

					for (int n = 0, k = 0; k < bombs[io].range_right.size(), n <= range_of_bomb * 50; k++, n += 50)
						bombs[io].range_right[k].setTextureRect(IntRect(text_size_exp.x*bombs[io].exp_animation_x, text_size_exp.y*bombs[io].exp_animation_y, text_size_exp.x, text_size_exp.y));

					for (int n = 0, k = 0; k < bombs[io].range_left.size(), n <= range_of_bomb * 50; k++, n += 50)
						bombs[io].range_left[k].setPosition(bombs[io].x_pos - n, bombs[io].y_pos);

					for (int n = 0, k = 0; k < bombs[io].range_left.size(), n <= range_of_bomb * 50; k++, n += 50)
						bombs[io].range_left[k].setTextureRect(IntRect(text_size_exp.x*bombs[io].exp_animation_x, text_size_exp.y*bombs[io].exp_animation_y, text_size_exp.x, text_size_exp.y));

					for (int n = 0, k = 0; k < bombs[io].range_up.size(), n <= range_of_bomb * 50; k++, n += 50)
						bombs[io].range_up[k].setPosition(bombs[io].x_pos, bombs[io].y_pos - n);

					for (int n = 0, k = 0; k < bombs[io].range_up.size(), n <= range_of_bomb * 50; k++, n += 50)
						bombs[io].range_up[k].setTextureRect(IntRect(text_size_exp.x*bombs[io].exp_animation_x, text_size_exp.y*bombs[io].exp_animation_y, text_size_exp.x, text_size_exp.y));

					for (int n = 0, k = 0; k < bombs[io].range_down.size(), n <= range_of_bomb * 50; k++, n += 50)
						bombs[io].range_down[k].setPosition(bombs[io].x_pos, bombs[io].y_pos + n);

					for (int n = 0, k = 0; k < bombs[io].range_down.size(), n <= range_of_bomb * 50; k++, n += 50)
						bombs[io].range_down[k].setTextureRect(IntRect(text_size_exp.x*bombs[io].exp_animation_x, text_size_exp.y*bombs[io].exp_animation_y, text_size_exp.x, text_size_exp.y));

					bombs[io].exp_animation_x++;
					if (bombs[io].exp_animation_x == 3) {
						bombs[io].exp_animation_x = 0, bombs[io].exp_animation_y++;
						if (bombs[io].exp_animation_y == 3) {
							bombs[io].bomb_exploded = 0;
							bombs.erase(bombs.begin());
							counter_placed_bombs--;

						}
					}
				}
			}
		}
		//}
		// Drawing of the Wallpaper
		window.clear();
		window.draw(Wallpaper);
		//=======================================================================================//
		
		// Drawing of The Map // 
//=======================================================================================//
						// Drawing of The Ground // 

		for (int i = 2; i < width; i++)
		{
			for (int j = 2; j < length; j++)
			{
				if (Grid[i][j].getTexture() == &ground || Grid[i][j].getTexture() == &ground2)
				{
					window.draw(Grid[i][j]);
				}
			}

		}

		//=======================================================================================//

		for (int i = 0; i < 19; i++)
		{
			if (i == 14||i==2)
				continue;
			
			window.draw(SuperPowers[i]);
		}
		//=======================================================================================//
									// Drawing of Wall and Breakable // 
		for (int j = 1; j <= length; j++)
		{
			for (int i = 1; i <= width; i++)
			{
				if (Grid[i][j].getTexture() == &ground || Grid[i][j].getTexture() == &ground2)
				{
					continue;
				}

				window.draw(Grid[i][j]);

				if (GetYindx(player.getPosition().y) >= j)				// Drawing of Player // 
				{
					window.draw(player);
				}
				if (GetYindx(White_Robot.getPosition().y) >= j&&!White_Robot_death)			// Drawing of White_Robot // 
				{
					window.draw(White_Robot);
				}
				if (GetYindx(Yellow_Robot.getPosition().y) >= j&&!Yellow_Robot_death)		// Drawing of Yellow_Robot // 
				{
					window.draw(Yellow_Robot);
				}
				if (GetYindx(White_Skeleton.getPosition().y) >= j&&!White_Skeleton_death)		// Drawing of White_Skeleton // 
				{
					window.draw(White_Skeleton);
				}
				if (GetYindx(Blue_Skeleton.getPosition().y) >= j&&!Blue_Skeleton_death)		// Drawing of Blue_Skeleton // 
				{
					window.draw(Blue_Skeleton);
				}
				for (int io = 0; io <bombs.size() ; io++)
				{
					if (bombs[io].y_index>=j && bombs[io].bomb_placed)
					{
						window.draw(bombs[io].sprt);
					}
				}

			}
		}

		//############################################################################################
		// Drawing Bombs
		for (int io = 0; io < bombs.size(); io++)
		{
			//if (bombs[io].bomb_placed)
				//window.draw(bombs[io].sprt);
			if (bombs[io].bomb_exploded)
			{

				//Right
				for (int g = 0; g <= range_of_bomb; g++)
				{

					window.draw(bombs[io].range_right[g]);

					if ((Grid[bombs[io].x_index + g][bombs[io].y_index].getTexture() == &breakable) && bombs[io].flag_range_right)
					{
						destroy_breakable_walls(bombs[io].x_index + g, bombs[io].y_index, Grid, ground, ground2);
						bombs[io].flag_range_right = 0;
						break;
					}
					else if (Grid[bombs[io].x_index + g][bombs[io].y_index].getTexture() == &wall)
					{

						bombs[io].flag_range_right = 0;
						break;
					}
				
					if (bombs[io].x_index+g == Player_xindx && bombs[io].y_index  == Player_yindx && bombs[io].flag_range_right)
					{
						if (Clock_Damgae.getElapsedTime().asSeconds() >= 1) {
							lives--;
							ssl = to_string(lives);
							Clock_Damgae.restart();
						}
						window.draw(Hurt);
					}

					if (bombs[io].x_index+g == Robot_xindx && bombs[io].y_index  == Robot_yindx && bombs[io].flag_range_right)
					{
						Yellow_Robot_death = 1;
						Yellow_Robot.setFillColor(Color::Transparent);
					}
					if (bombs[io].x_index+g == White_Robot_xindx && bombs[io].y_index == White_Robot_yindx && bombs[io].flag_range_right)
					{
						White_Robot_death = 1;
						White_Robot.setFillColor(Color::Transparent);
					}
					if (bombs[io].x_index+g == Skeleton_xindx && bombs[io].y_index  == Skeleton_yindx && bombs[io].flag_range_right)
					{
						White_Skeleton_death = 1;
						White_Skeleton.setFillColor(Color::Transparent);
					}
					if (bombs[io].x_index +g == GetXindx(Blue_Skeleton.getPosition().x) && bombs[io].y_index  == GetYindx(Blue_Skeleton.getPosition().y) && bombs[io].flag_range_right)
					{
						Blue_Skeleton_death = 1;
						Blue_Skeleton.setFillColor(Color::Transparent);
					}

				
				}

				
				//UP
				for (int g = 0; g <= range_of_bomb; g++)
				{

					window.draw(bombs[io].range_up[g]);

					if ((Grid[bombs[io].x_index][bombs[io].y_index - g].getTexture() == &breakable) && bombs[io].flag_range_up)
					{


						destroy_breakable_walls(bombs[io].x_index, bombs[io].y_index - g, Grid, ground, ground2);

						bombs[io].flag_range_up = 0;
						break;

					}
					else if (Grid[bombs[io].x_index][bombs[io].y_index - g].getTexture() == &wall)
					{

						bombs[io].flag_range_up = 0;
						break;
					}

					if (bombs[io].x_index == Player_xindx && bombs[io].y_index - g == Player_yindx && bombs[io].flag_range_up)
					{
						if (Clock_Damgae.getElapsedTime().asSeconds() >= 1) {
							lives--;
							ssl = to_string(lives);
							Clock_Damgae.restart();
						}
						window.draw(Hurt);
					}

					if (bombs[io].x_index == Robot_xindx && bombs[io].y_index - g == Robot_yindx && bombs[io].flag_range_up)
					{
						Yellow_Robot_death = 1;
						Yellow_Robot.setFillColor(Color::Transparent);
					}
					if (bombs[io].x_index == White_Robot_xindx && bombs[io].y_index - g == White_Robot_yindx && bombs[io].flag_range_up)
					{
						White_Robot_death = 1;
						White_Robot.setFillColor(Color::Transparent);
					}
					if (bombs[io].x_index == Skeleton_xindx && bombs[io].y_index - g == Skeleton_yindx && bombs[io].flag_range_up)
					{
						White_Skeleton_death = 1;
						White_Skeleton.setFillColor(Color::Transparent);
					}
					if (bombs[io].x_index == GetXindx(Blue_Skeleton.getPosition().x) && bombs[io].y_index - g == GetYindx(Blue_Skeleton.getPosition().y) && bombs[io].flag_range_up)
					{
						Blue_Skeleton_death = 1;
						Blue_Skeleton.setFillColor(Color::Transparent);
					}

				}

				//Left
				for (int g = 0; g <= range_of_bomb; g++)
				{

					window.draw(bombs[io].range_left[g]);

					if ((Grid[bombs[io].x_index - g][bombs[io].y_index].getTexture() == &breakable)&& bombs[io].flag_range_left)
					{
						destroy_breakable_walls(bombs[io].x_index - g, bombs[io].y_index, Grid, ground, ground2);

						bombs[io].flag_range_left = 0;
						break;
					}
					else if (Grid[bombs[io].x_index - g][bombs[io].y_index].getTexture() == &wall)
					{
						bombs[io].flag_range_left = 0;
						break;
					}
					if (bombs[io].x_index - g == Player_xindx && bombs[io].y_index == Player_yindx && bombs[io].flag_range_left)
					{
						if (Clock_Damgae.getElapsedTime().asSeconds() >= 1) {
							lives--;
							//SuperPowers[i].setColor(Color::Transparent);
							ssl = to_string(lives);
							//counter.restart();
							Clock_Damgae.restart();
						}
						window.draw(Hurt);
					}

					if (bombs[io].x_index - g == Robot_xindx - g && bombs[io].y_index == Robot_yindx && bombs[io].flag_range_left)
					{
						Yellow_Robot_death = 1;
						Yellow_Robot.setFillColor(Color::Transparent);
					}
					if (bombs[io].x_index - g == White_Robot_xindx && bombs[io].y_index == White_Robot_yindx && bombs[io].flag_range_left)
					{
						White_Robot_death = 1;
						White_Robot.setFillColor(Color::Transparent);
					}
					if (bombs[io].x_index - g == Skeleton_xindx && bombs[io].y_index == Skeleton_yindx && bombs[io].flag_range_left)
					{
						White_Skeleton_death = 1;
						White_Skeleton.setFillColor(Color::Transparent);
					}
					if (bombs[io].x_index - g == GetXindx(Blue_Skeleton.getPosition().x) && bombs[io].y_index == GetYindx(Blue_Skeleton.getPosition().y) && bombs[io].flag_range_left)
					{
						Blue_Skeleton_death = 1;
						Blue_Skeleton.setFillColor(Color::Transparent);
					}

				}

				//Down
				for (int g = 0; g <= range_of_bomb; g++)
				{

					window.draw(bombs[io].range_down[g]);
					if ((Grid[bombs[io].x_index][bombs[io].y_index + g].getTexture() == &breakable) && bombs[io].flag_range_down)
					{
						destroy_breakable_walls(bombs[io].x_index, bombs[io].y_index + g, Grid, ground, ground2);
						bombs[io].flag_range_down = 0;
						break;
					}
					else if (Grid[bombs[io].x_index][bombs[io].y_index + g].getTexture() == &wall)
					{

						bombs[io].flag_range_down = 0;
						break;
					}
					if (bombs[io].x_index == Player_xindx && bombs[io].y_index + g == Player_yindx && bombs[io].flag_range_down)
					{
						if (Clock_Damgae.getElapsedTime().asSeconds() >= 1) {
							lives--;
							ssl = to_string(lives);
							Clock_Damgae.restart();
						}
						window.draw(Hurt);
					}

					if (bombs[io].x_index == Robot_xindx && bombs[io].y_index + g == Robot_yindx && bombs[io].flag_range_down)
					{
						Yellow_Robot_death = 1;
						Yellow_Robot.setFillColor(Color::Transparent);
					}
					if (bombs[io].x_index == White_Robot_xindx && bombs[io].y_index + g == White_Robot_yindx && bombs[io].flag_range_down)
					{
						White_Robot_death = 1;
						White_Robot.setFillColor(Color::Transparent);
					}
					if (bombs[io].x_index == Skeleton_xindx && bombs[io].y_index + g == Skeleton_yindx && bombs[io].flag_range_down)
					{
						White_Skeleton_death = 1;
						White_Skeleton.setFillColor(Color::Transparent);
					}
					if (bombs[io].x_index == GetXindx(Blue_Skeleton.getPosition().x) && bombs[io].y_index + g == GetYindx(Blue_Skeleton.getPosition().y) && bombs[io].flag_range_down)
					{
						Blue_Skeleton_death = 1;
						Blue_Skeleton.setFillColor(Color::Transparent);
					}

				}

			}
		}
	
		//############################################################################################//############################################################################################

		for (int i = 0; i < 19; i++) {
			int sumP = 0; int sumR = 0;
			sumP = GetXindx(player.getPosition().x) + GetYindx(player.getPosition().y);
			sumR = GetXindx(SuperPowers[i].getPosition().x) + GetYindx(SuperPowers[i].getPosition().y);

			if (sumR == sumP + 1 && (GetXindx(SuperPowers[i].getPosition().x) == GetXindx(player.getPosition().x))
				&& !visted[GetXindx(SuperPowers[i].getPosition().x)][GetYindx(SuperPowers[i].getPosition().y)])
			{

				visted[GetXindx(SuperPowers[i].getPosition().x)][GetYindx(SuperPowers[i].getPosition().y)] = 1;

				if (SuperPowers[i].getTexture() == &rangeskill) {


					temp_range_of_bomb++;
					SuperPowers[i].setColor(Color::Transparent);
					ss = to_string(temp_range_of_bomb);

				}
				else if (SuperPowers[i].getTexture() == &bombskill) {
					boombs++;
					SuperPowers[i].setColor(Color::Transparent);
					sss = to_string(boombs);


				}
				else if (SuperPowers[i].getTexture() == &healskill) {
					lives++;
					SuperPowers[i].setColor(Color::Transparent);
					ssl = to_string(lives);
				}
			}

		}



		bool isbombing = 0;
		for (int io = 0; io < bombs.size(); io++)
		{
			if (bombs[io].bomb_exploded) { isbombing = 1; break; }
		}
		if (!isbombing)
		{
			range_of_bomb = temp_range_of_bomb;
		}
		//############################################################################################

		
		
		if ((GetXindx(player.getPosition().x) == GetXindx(White_Robot.getPosition().x) && GetYindx(player.getPosition().y) == GetYindx(White_Robot.getPosition().y)) && !White_Robot_death) {
			if (Clock_Damgae.getElapsedTime().asSeconds() >= 1) {
				lives--;
				ssl = to_string(lives);
				Clock_Damgae.restart();
			}
			window.draw(Hurt);

		}

		if (GetXindx(player.getPosition().x) == GetXindx(Yellow_Robot.getPosition().x) && GetYindx(player.getPosition().y) == GetYindx(Yellow_Robot.getPosition().y) && !Yellow_Robot_death)
		{
			if (Clock_Damgae.getElapsedTime().asSeconds() >= 1) {
				lives--;
				ssl = to_string(lives);
				Clock_Damgae.restart();
			}
			window.draw(Hurt);
		}

		if (GetXindx(player.getPosition().x) == GetXindx(White_Skeleton.getPosition().x) && GetYindx(player.getPosition().y) == GetYindx(White_Skeleton.getPosition().y) && !White_Skeleton_death)
		{
			if (Clock_Damgae.getElapsedTime().asSeconds() >= 1) {
				lives--;
				ssl = to_string(lives);
				Clock_Damgae.restart();
			}
			window.draw(Hurt);
		}

		if (GetXindx(player.getPosition().x) == GetXindx(Blue_Skeleton.getPosition().x) && GetYindx(player.getPosition().y) == GetYindx(Blue_Skeleton.getPosition().y) && !Blue_Skeleton_death)
		{
			if (Clock_Damgae.getElapsedTime().asSeconds() >= 1) {
				lives--;
				ssl = to_string(lives);
				Clock_Damgae.restart();
			}
			window.draw(Hurt);
		}

		cout << lives << endl;

		window.draw(nooflives);
		window.draw(noofbombs);
		window.draw(noofranges);
		window.draw(lifeS);
		window.draw(rangeS);
		window.draw(bombS);
		window.draw(txt); // بيترسم تحت
		if(lives < 1) {
			ingame.stop();
			player.setFillColor(Color::Transparent);
			Tomb.setPosition(player.getPosition().x, player.getPosition().y);
			window.draw(Tomb);
			--elapsed;
			if (elapsed <= 50 + 500) {
				if (playsong) {
					lost.play();
					playsong = 0;
				}
				if (elapsed <= -500) {
					window.clear(Color::Black), window.draw(RIP_photo);
					exit(0);
				}
			}
		}
		
		if (Die) {
			ingame.stop();
			window.clear(Color::Black), window.draw(big_exp), window.draw(RIP_photo);
			--elapsed;
			if (elapsed <= 1) {
				exit(0);
			}
		}
		
		if (Player_xindx == 16 && Player_yindx == 11) {
			ingame.stop();
			window.clear(Color::Black), window.draw(Level_complete);
			--elapsed;
			if (elapsed <= 1) {
				exit(0);
			}
		}
		window.display();
	}
}
void Legendary_Mode() {
	//////////////////////////////////////////////////////////////////////////////////
	int big_pos_x = 1, big_pos_y = 0; //بيتعرف فوق
	Texture big_exp_text;
	big_exp_text.loadFromFile("exp.png");
	Sprite big_exp;
	big_exp.setTexture(big_exp_text);
	Vector2u text_size_big_exp = big_exp_text.getSize();
	text_size_big_exp.x /= 3;
	text_size_big_exp.y /= 3;
	big_exp.setScale(3, 3);
	Clock start_clock;
	Time start_timer;
	int display_timer, minutes = 1;
	Text txt;
	Font dis_time;
	if (!dis_time.loadFromFile("Chocolate Bar Demo.otf"))
	{
		cout << "error font";
	}
	Time elaps = seconds(200);
	int elapsed = elaps.asSeconds();
	//////////////////////////////////////////////////////////////////////////////////
	Music ingame,lost; ingame.openFromFile("ingame.wav");
	ingame.play();
	lost.openFromFile("For the Damaged Coda.wav");
	ingame.setLoop(1);

	//###############################################################################
	// Making of Player
	RectangleShape player(Vector2f(100, 120));
	Texture player_texture;
	Vector2u player_size;
	Clock player_clock;
	Time player_time;
	float player_position = 0;
	const int player_speed = 2;
	Generate_Charcter(player, player_texture, player_size, "mainplayer.png", 2, 2);
	//###############################################################################

	//###############################################################################
	// Making of Red_Monster(Smart)
	RectangleShape Red_Robot(Vector2f(100, 120));
	Texture Red_Robot_texture;
	Vector2u Red_Robot_size;
	Clock Red_Robot_clock;
	Time Red_Robot_time;
	float Red_Robot_position = 0;
	float Red_Robot_Speed = 0.6;
	Generate_Charcter(Red_Robot, Red_Robot_texture, Red_Robot_size, "red_robot.png", 14, 3);
	//###############################################################################

	//###############################################################################
	// Making of Blue_Skeleton(smart)
	RectangleShape Blue_Skeleton(Vector2f(100, 120));
	Texture Blue_Skeleton_text;
	Vector2u Blue_Skeleton_text_size;
	Clock Blue_Skeleton_clock;
	Time Blue_Skeleton_time;
	float Blue_Skeleton_speed = 0.3;
	float Blue_Skeleton_position = 0;
	Generate_Charcter(Blue_Skeleton, Blue_Skeleton_text, Blue_Skeleton_text_size, "smart_skeleton.png", 10, 2);
	//###############################################################################

	//###############################################################################
	// Making of Skeleton
	RectangleShape Skeleton(Vector2f(100, 120));
	Texture Skeleton_texture;
	Vector2u Skeleton_size;
	Clock Skeleton_clock;
	Time Skeleton_time;
	int Skeleton_Rand_motion = rand() % 4 + 1;
	int Skeleton_counter = 0;
	float Skeleton_position = 0;
	Generate_Charcter(Skeleton, Skeleton_texture, Skeleton_size, "monster (5).png", 7, 8);
	//###############################################################################

	//###############################################################################
	// Making of White_Robot
	RectangleShape White_Robot(Vector2f(100, 120));
	Texture White_Robot_texture;
	Vector2u White_Robot_size;
	Clock White_Robot_clock;
	Time White_Robot_time;
	int White_Robot_Rand_motion = rand() % 4 + 1;
	int White_Robot_counter = 0;
	float White_Robot_position = 0;
	Generate_Charcter(White_Robot, White_Robot_texture, White_Robot_size, "white_robot.png", 14, 8);
	//###############################################################################

	//###############################################################################
	// Movement stuff
	leftmovement left; rightmovement right; upmovement up; downmovement down;
	int Player_xindx = 0, Player_yindx = 0, White_Robot_xindx = 0, White_Robot_yindx = 0, Skeleton_xindx = 0, Skeleton_yindx = 0, Blue_Skeleton_xindx = 0, Blue_Skeleton_yindx = 0, Red_Robot_xindx = 0, Red_Robot_yindx = 0;
	bool Red_Robot_death = 0, White_Robot_death = 0, Skeleton_death = 0,Blue_Skeleton_death=0;
	//###############################################################################

 // Making of Grid
	Texture wall, ground, wallpaper, ground2, breakable, life, bomb_frame, range, hurt, choice;
	if (!breakable.loadFromFile("breakable.png"))
	{
		cout << "error loading breakable wall pic";
	}
	if (!wallpaper.loadFromFile("wallpaper.jpg"))
	{
		cout << "error loading wallpaper pic";
	}
	if (!wall.loadFromFile("3d wall.png"))
	{
		cout << "error loading wall pic";
	}

	if (!ground.loadFromFile("ground 1.png"))
	{
		cout << "error loading ground pic";
	}
	if (!ground2.loadFromFile("grass.png"))
	{
		cout << "error loading grass pic";
	}

	if (!life.loadFromFile("flife.png"))
	{
		cout << "error loading life pic";
	}
	if (!range.loadFromFile("frange.png"))
	{

		cout << "error loading range pic";
	}
	if (!bomb_frame.loadFromFile("fbomb.png"))
	{
		cout << "error loading bomb pic";
	}

	if (!hurt.loadFromFile("hurt.png"))
	{
		cout << "error loading  hurt pic";
	}
	
	//###############################################################################
							// Player Abilities and Features // 
	Sprite lifeS, bombS, rangeS, Hurt, Wallpaper;
	Texture Rip,tomb, healskill, speedskill, rangeskill, bombskill;
	Hurt.setTexture(hurt);
	Wallpaper.setTexture(wallpaper);
	if (!life.loadFromFile("flife.png"))
	{
		cout << "error loading life pic";
	}
	if (!range.loadFromFile("frange.png"))
	{
		cout << "error loading range pic";
	}
	if (!bomb_frame.loadFromFile("fbomb.png"))
	{
		cout << "error loading bomb pic";
	}
	if (!Rip.loadFromFile("death.png"))
	{
		cout << "error loading RIP pic";
	}
	if (!tomb.loadFromFile("RIP.png"))
	{
		cout << "error loading tomb pic";
	}
	if (!healskill.loadFromFile("Heal .png"))
	{
		cout << "error to loading Heal skill" << endl;
	}

	if (!rangeskill.loadFromFile("Range skill.png"))
	{
		cout << "error to loading Range skill " << endl;
	}
	if (!bombskill.loadFromFile("bomb skill .png"))
	{
		cout << "error to loading Range skill " << endl;
	}



	//###############################################################################

	


	life.setSmooth(1);
	bomb_frame.setSmooth(1);
	hurt.setSmooth(1);
	range.setSmooth(1);


	//###############################################################################
	// Player abilities
	int lives = 3;
	int boombs = 1;
	Clock counter;

	//###############################################################################
	Texture Complete;
	Complete.loadFromFile("level-completed.png");
	Sprite Tomb, RIP_Photo, Level_complete;
	Level_complete.setTexture(Complete);
	Tomb.setTexture(tomb);
	Tomb.setScale(0.1, 0.1);
	RIP_Photo.setTexture(Rip);
	lifeS.setTexture(life);
	lifeS.setPosition(0, 250);
	lifeS.setScale(0.4, 0.4);

	rangeS.setTexture(range);
	rangeS.setPosition(0, 180);
	rangeS.setScale(0.4, 0.4);

	bombS.setTexture(bomb_frame);
	bombS.setPosition(0, 330);
	bombS.setScale(0.4, 0.4);

	Tomb.setTexture(tomb);
	Tomb.setScale(0.09, 0.09);

	RIP_Photo.setTexture(Rip);

	Hurt.setTexture(hurt);
	Wallpaper.setTexture(wallpaper);
	// Numbers
	Clock Clock_Damgae;
	//###############################################################################


	// Distribution of wall & ground on the Grid

	Sprite Grid[width + 1][length + 1];

	for (int i = 1; i <= width; i++)
	{
		for (int j = 1; j <= length; j++)
		{
			if (i % 2 != 0 && j % 2 != 0)
			{

				Grid[i][j].setTexture(wall);
				Grid[i][j].setPosition((i + 1.97)*53.15, (j + 0.255) * 48);
				Grid[i][j].setScale(Vector2f(0.1009, 0.165));

			}

			else if (i == 1 || i == width)
			{
				Grid[i][j].setTexture(wall);
				Grid[i][j].setPosition((i + 1.97)*53.15, (j + 0.255) * 48);
				Grid[i][j].setScale(Vector2f(0.1009, 0.165));
			}
			else if (j == 1 || j == length)
			{
				Grid[i][j].setTexture(wall);
				Grid[i][j].setPosition((i + 1.97)*53.15, (j + 0.255) * 48);
				Grid[i][j].setScale(Vector2f(0.1009, 0.165));
			}
			else
			{
				if (i % 2 == j % 2) { Grid[i][j].setTexture(ground); }
				else { Grid[i][j].setTexture(ground2); }
				Grid[i][j].setPosition((i + 2) * 53, (j + 0.8) * 48);
				Grid[i][j].setScale(0.1, 0.101);
			}
		}
	}



	for (int i = 1; i <= 30; i++)
	{
		int x = rand() % 15 + 2, y = rand() % 9 + 2;
		if (x % 2 != 0 && y % 2 != 0)
		{
			x++;
		}
		Grid[x][y].setTexture(breakable);
		Grid[x][y].setPosition((x + 1.97)*53.15, (y + 0.255) * 48);
		Grid[x][y].setScale(Vector2f(0.1009, 0.165));
	}

	

	//###############################################################################
			   //Player Bomb stuff //
	Sprite bombing, exp;
	bombing.setScale(0.2f, 0.2f);
	exp.setScale(0.25, 0.25);
	Texture playertext, player2text, bombtext, exptext;
	playertext.loadFromFile("mainplayer.png");
	player2text.loadFromFile("blue.png");
	bombtext.loadFromFile("BOOM.png");
	exptext.loadFromFile("exp.png");
	bombing.setTexture(bombtext);
	exp.setTexture(exptext);
	Vector2u text_size_bomb = bombtext.getSize();
	Vector2u text_size_exp = exptext.getSize();
	text_size_bomb.y /= 4;
	text_size_exp.x /= 3;
	text_size_exp.y /= 3;
	Time times2, times, t, ti, bomb_timer;
	Clock clocks2, c, cs, clocks, clock_timer;

	bomb bomb_template;
	bomb_template.sprt.setTexture(bombtext);
	bomb_template.sprt.setScale(0.15f, 0.15f);
	vector<bomb>bombs;
	int number_of_bombs = 0, range_of_bomb = 2, bomb_placement_timer = 0, superpower_of_bomb = 2, counter_placed_bombs=0;
	int temp_range_of_bomb = 2;
	//bombs.push_back(bomb_template);
	//###############################################################################
	// abilites

	Sprite SuperPowers[14];
	for (int i = 0; i < 5; i++)
	{

		SuperPowers[i].setTexture(bombskill);			// Rand_Choice 1 => Bombing
		SuperPowers[i].setScale(0.5, 0.3);

	}
	for (int i = 5; i < 8; i++)
	{
		SuperPowers[i].setTexture(healskill);		// Rand_Choice 2 => Health
		SuperPowers[i].setScale(0.5, 0.3);
	}

	for (int i = 8; i < 13; i++)
	{
		SuperPowers[i].setTexture(rangeskill);				// Rand_Choice 3 => Range
		SuperPowers[i].setScale(0.5, 0.3);
	}

	SuperPowers[0].setPosition(GetXpostion(4) + 20, GetYpostion(3));
	SuperPowers[1].setPosition(GetXpostion(15) + 20, GetYpostion(4));
	SuperPowers[2].setPosition(GetXpostion(4) + 20, GetYpostion(4));
	SuperPowers[3].setPosition(GetXpostion(14) + 20, GetYpostion(10));
	SuperPowers[4].setPosition(GetXpostion(4) + 20, GetYpostion(6));
	SuperPowers[5].setPosition(GetXpostion(14) + 20, GetYpostion(3));
	SuperPowers[6].setPosition(GetXpostion(4) + 20, GetYpostion(8));
	SuperPowers[7].setPosition(GetXpostion(12) + 20, GetYpostion(10));
	SuperPowers[8].setPosition(GetXpostion(6) + 20, GetYpostion(7));
	SuperPowers[9].setPosition(GetXpostion(12) + 20, GetYpostion(6));
	SuperPowers[10].setPosition(GetXpostion(8) + 20, GetYpostion(8));
	SuperPowers[11].setPosition(GetXpostion(10) + 20, GetYpostion(9));
	SuperPowers[12].setPosition(GetXpostion(8) + 20, GetYpostion(9));
	SuperPowers[13].setPosition(GetXpostion(16) + 20, GetYpostion(4));
	
	Texture timebomb;
	timebomb.loadFromFile("time-bomb.png");
	Sprite timebombS;
	SuperPowers[13].setTexture(timebomb);				// Rand_Choice 3 => Range
	SuperPowers[13].setScale(0.05, 0.05);

	Time timer; Clock timer_clock;
	//###############################################################################
	// Game Window
	RenderWindow Level_3(VideoMode(1280, 768), "Legendary Mode");
	while (Level_3.isOpen())
	{
		////////////////////////////////////////////////////////////////////////////////
		timer = timer_clock.getElapsedTime();
		display_timer = timer.asSeconds();
		string s1, s2;
		s1 = to_string(59 - (display_timer));
		s2 = to_string(minutes);
		s2 += ":";
		txt.setFont(dis_time);
		txt.setString(s2 + s1);
		txt.setPosition(512, 22);
		if (display_timer > 59 && minutes > 0)
		{
			minutes--;
			timer_clock.restart();
		}
		if (minutes == 0 && display_timer == 59) {
			Die = 1;
			ispressed = 1;
		}
		if (ispressed) {
			start_timer = start_clock.getElapsedTime();
			if (start_timer.asSeconds() >= 0.2) {
				start_clock.restart();
				big_exp.setPosition((Level_3.getPosition().x / 2) - 25, (Level_3.getPosition().y / 2) - 210);
				big_exp.setTextureRect(IntRect(text_size_big_exp.x*big_pos_x, text_size_big_exp.y*big_pos_y, text_size_big_exp.x, text_size_big_exp.y));

				big_pos_x++;
				if (big_pos_x == 3) {
					big_pos_x = 0, big_pos_y++;
					if (big_pos_y == 2 && big_pos_x == 2) {
						ispressed = 0;
						big_exp.setColor(Color::Transparent);
					}
				}
			}
		}
		/////////////////////////////////////////////////////////////////////////////////
		string ss = to_string(temp_range_of_bomb);

		Text noofranges;
		noofranges.setFont(font);
		noofranges.setString(ss);
		noofranges.setCharacterSize(45);
		noofranges.setPosition(90, 195);

		string sss = to_string(boombs);

		Text noofbombs;
		noofbombs.setFont(font);
		noofbombs.setString(sss);
		noofbombs.setCharacterSize(45);
		noofbombs.setPosition(90, 335);

		string ssl = to_string(lives);

		Text nooflives;
		nooflives.setFont(font);
		nooflives.setString(ssl);
		nooflives.setCharacterSize(45);
		nooflives.setPosition(90, 270);

		/////////////////////////////////////////////////////////////////////////////////

	//######################################################################################################

		// Player Movement
		Player_xindx = (float)((float)(player.getPosition().x - 183) / 53 + 2);
		Player_yindx = (float)((float)(player.getPosition().y - 74) / 48 + 2);
		right.i = GetXindx(player.getPosition().x + 20); right.j = Player_yindx;
		left.i = GetXindx(player.getPosition().x - 20); left.j = Player_yindx;
		up.i = Player_xindx; up.j = GetYindx(player.getPosition().y - 7);
		down.i = Player_xindx; down.j = GetYindx(player.getPosition().y + 7);

		if (Keyboard::isKeyPressed(Keyboard::Key::Right) && (Grid[right.i][right.j].getTexture() == &ground || Grid[right.i][right.j].getTexture() == &ground2))      // Moving Right
		{
			bool check_bomb = 0;
			for (int io = 0; io < bombs.size(); io++)
			{
				if (bombs[io].x_index == GetXindx(player.getPosition().x + 1) && bombs[io].x_index != GetXindx(player.getPosition().x) && bombs[io].y_index == right.j)
				{
					check_bomb = 1; break;
				}
			}
			if (!check_bomb)
				Movement_Right(player, player_time, player_clock, player_position, player_size, player_speed);
		}
		else if (Keyboard::isKeyPressed(Keyboard::Key::Left) && (Grid[left.i][left.j].getTexture() == &ground || Grid[left.i][left.j].getTexture() == &ground2))         // Moving Left
		{
			bool check_bomb = 0;
			for (int io = 0; io < bombs.size(); io++)
			{
				if (bombs[io].x_index == GetXindx(player.getPosition().x - 1) && bombs[io].x_index != GetXindx(player.getPosition().x) && bombs[io].y_index == left.j)
				{
					check_bomb = 1; break;
				}
			}
			if (!check_bomb)
				Movement_Left(player, player_time, player_clock, player_position, player_size, player_speed);
		}
		else if (Keyboard::isKeyPressed(Keyboard::Key::Up) && (Grid[GetXindx(player.getPosition().x + 25)][up.j].getTexture() == &ground || Grid[up.i][up.j].getTexture() == &ground2))           // Moving Up
		{
			bool check_bomb = 0;
			for (int io = 0; io < bombs.size(); io++)
			{
				if (bombs[io].y_index == GetYindx(player.getPosition().y - 1) && bombs[io].y_index != GetYindx(player.getPosition().y) && bombs[io].x_index == up.i)
				{
					check_bomb = 1; break;
				}
			}
			if (!check_bomb)
				Movement_Upward(player, player_time, player_clock, player_position, player_size, player_speed);
		
		}
		else if (Keyboard::isKeyPressed(Keyboard::Key::Down) && (Grid[GetXindx(player.getPosition().x+25)][down.j].getTexture() == &ground || Grid[down.i][down.j].getTexture() == &ground2))         // Moving Down
		{
			bool check_bomb = 0;
			for (int io = 0; io < bombs.size(); io++)
			{
				if (bombs[io].y_index == GetYindx(player.getPosition().y + 1) && bombs[io].y_index != GetYindx(player.getPosition().y) && bombs[io].x_index == up.i)
				{
					check_bomb = 1; break;
				}
			}
			if (!check_bomb)

				Movement_Downward(player, player_time, player_clock, player_position, player_size, player_speed);
		}

		//######################################################################################################
		//######################################################################################################



		//######################################################################################################
		// Smart_Skeleton movement
		Blue_Skeleton_xindx = (float)((float)(Blue_Skeleton.getPosition().x - 183) / 53 + 2);
		Blue_Skeleton_yindx = (float)((float)(Blue_Skeleton.getPosition().y - 74) / 48 + 2);
		right.i = GetXindx(Blue_Skeleton.getPosition().x + 20); right.j = Blue_Skeleton_yindx;
		left.i = GetXindx(Blue_Skeleton.getPosition().x - 20); left.j = Blue_Skeleton_yindx;
		up.i = Blue_Skeleton_xindx; up.j = GetYindx(Blue_Skeleton.getPosition().y - 7);
		down.i = Blue_Skeleton_xindx; down.j = GetYindx(Blue_Skeleton.getPosition().y + 7);

		// Call BFS
		BFS(Player_xindx, Player_yindx, Blue_Skeleton_xindx, Blue_Skeleton_yindx, Grid, ground, ground2);

		// Movement
		if (Smart_Skeleton_Movement.i == Blue_Skeleton_xindx && Smart_Skeleton_Movement.j - Blue_Skeleton_yindx == 1) {
			Movement_Downward(Blue_Skeleton, Blue_Skeleton_time, Blue_Skeleton_clock, Blue_Skeleton_position, Blue_Skeleton_text_size, Blue_Skeleton_speed);	// Moving Downward
		}
		else if (Smart_Skeleton_Movement.i == Blue_Skeleton_xindx && Smart_Skeleton_Movement.j - Blue_Skeleton_yindx == -1) {
			Movement_Upward(Blue_Skeleton, Blue_Skeleton_time, Blue_Skeleton_clock, Blue_Skeleton_position, Blue_Skeleton_text_size, Blue_Skeleton_speed);		// Moving Upward
		}
		else if (Smart_Skeleton_Movement.i - Blue_Skeleton_xindx == 1 && Smart_Skeleton_Movement.j == Blue_Skeleton_yindx) {
			Movement_Right(Blue_Skeleton, Blue_Skeleton_time, Blue_Skeleton_clock, Blue_Skeleton_position, Blue_Skeleton_text_size, Blue_Skeleton_speed);		// Moving Right
		}
		else if (Smart_Skeleton_Movement.i - Blue_Skeleton_xindx == -1 && Smart_Skeleton_Movement.j == Blue_Skeleton_yindx) {
			Movement_Left(Blue_Skeleton, Blue_Skeleton_time, Blue_Skeleton_clock, Blue_Skeleton_position, Blue_Skeleton_text_size, Blue_Skeleton_speed);		// Moving Left
		}

		//######################################################################################################


		//######################################################################################################
		// Red_Robot movement
		Red_Robot_xindx = (float)((float)(Red_Robot.getPosition().x - 183) / 53 + 2);
		Red_Robot_yindx = (float)((float)(Red_Robot.getPosition().y - 74) / 48 + 2);
		right.i = GetXindx(Red_Robot.getPosition().x + 20); right.j = Red_Robot_yindx;
		left.i = GetXindx(Red_Robot.getPosition().x - 20); left.j = Red_Robot_yindx;
		up.i = Red_Robot_xindx; up.j = GetYindx(Red_Robot.getPosition().y - 7);
		down.i = Red_Robot_xindx; down.j = GetYindx(Red_Robot.getPosition().y + 7);

		// Call BFS
		BFS(Player_xindx, Player_yindx, Red_Robot_xindx, Red_Robot_yindx, Grid, ground, ground2);

		// Movement
		if (Red_Robot_Movement.i == Red_Robot_xindx && Red_Robot_Movement.j - Red_Robot_yindx == 1) {
			Movement_Downward(Red_Robot, Red_Robot_time, Red_Robot_clock, Red_Robot_position, Red_Robot_size, Red_Robot_Speed);	// Moving Downward
		}
		else if (Red_Robot_Movement.i == Red_Robot_xindx && Red_Robot_Movement.j - Red_Robot_yindx == -1) {
			Movement_Upward(Red_Robot, Red_Robot_time, Red_Robot_clock, Red_Robot_position, Red_Robot_size, Red_Robot_Speed);	// Moving Upward
		}
		else if (Red_Robot_Movement.i - Red_Robot_xindx == 1 && Red_Robot_Movement.j == Red_Robot_yindx) {
			Movement_Right(Red_Robot, Red_Robot_time, Red_Robot_clock, Red_Robot_position, Red_Robot_size, Red_Robot_Speed);	// Movement Right
		}
		else if (Red_Robot_Movement.i - Red_Robot_xindx == -1 && Red_Robot_Movement.j == Red_Robot_yindx) {
			Movement_Left(Red_Robot, Red_Robot_time, Red_Robot_clock, Red_Robot_position, Red_Robot_size, Red_Robot_Speed);	// Moving left
		}

		//######################################################################################################




		// Skeleton movement
		Skeleton_xindx = (float)((float)(Skeleton.getPosition().x - 183) / 53 + 2);
		Skeleton_yindx = (float)((float)(Skeleton.getPosition().y - 74) / 48 + 2);
		right.i = GetXindx(Skeleton.getPosition().x + 20); right.j = Skeleton_yindx;
		left.i = GetXindx(Skeleton.getPosition().x - 20); left.j = Skeleton_yindx;
		up.i = Skeleton_xindx; up.j = GetYindx(Skeleton.getPosition().y - 7);
		down.i = Skeleton_xindx; down.j = GetYindx(Skeleton.getPosition().y + 7);
		//######################################################################################################

		//######################################################################################################
		// Skeleton Movement
		if (Skeleton_counter >= 250)
		{
			Skeleton_Rand_motion = rand() % 4 + 1;
			Skeleton_counter = 0;
		}

		if (Skeleton_Rand_motion == 1)											// Moving UP => Rand_motion == 1
		{
			Skeleton_counter++;
			if (Grid[GetXindx(Skeleton.getPosition().x + 25)][up.j].getTexture() == &ground || Grid[up.i][up.j].getTexture() == &ground2)
			{

				Movement_Upward(Skeleton, Skeleton_time, Skeleton_clock, Skeleton_position, Skeleton_size, Random_Monsters_Speed);

			}
			else
			{
				Skeleton_counter = 250;
			}

		}


		else if (Skeleton_Rand_motion == 2)									// Moving Down => Rand_motion == 2
		{
			Skeleton_counter++;
			if (Grid[GetXindx(Skeleton.getPosition().x + 25)][down.j].getTexture() == &ground || Grid[down.i][down.j].getTexture() == &ground2)
			{
				Movement_Downward(Skeleton, Skeleton_time, Skeleton_clock, Skeleton_position, Skeleton_size, Random_Monsters_Speed);

			}

			else
			{
				Skeleton_counter = 250;
			}
		}


		else if (Skeleton_Rand_motion == 3)									// Moving Right => Rand_motion == 3
		{
			Skeleton_counter++;
			if (Grid[right.i][right.j].getTexture() == &ground || Grid[right.i][right.j].getTexture() == &ground2)
			{
				Movement_Right(Skeleton, Skeleton_time, Skeleton_clock, Skeleton_position, Skeleton_size, Random_Monsters_Speed);

			}
			else
			{
				Skeleton_counter = 250;
			}
		}


		else if (Skeleton_Rand_motion == 4)									// Moving Left => Rand_motion == 4
		{
			Skeleton_counter++;
			if (Grid[left.i][left.j].getTexture() == &ground || Grid[left.i][left.j].getTexture() == &ground2)
			{
				Movement_Left(Skeleton, Skeleton_time, Skeleton_clock, Skeleton_position, Skeleton_size, Random_Monsters_Speed);

			}
			else
			{
				Skeleton_counter = 250;
			}

		}

		//######################################################################################################
		// White_Robot movement
		White_Robot_xindx = (float)((float)(White_Robot.getPosition().x - 183) / 53 + 2);
		White_Robot_yindx = (float)((float)(White_Robot.getPosition().y - 74) / 48 + 2);
		right.i = GetXindx(White_Robot.getPosition().x + 20); right.j = White_Robot_yindx;
		left.i = GetXindx(White_Robot.getPosition().x - 20); left.j = White_Robot_yindx;
		up.i = White_Robot_xindx; up.j = GetYindx(White_Robot.getPosition().y - 7);
		down.i = White_Robot_xindx; down.j = GetYindx(White_Robot.getPosition().y + 7);
		//######################################################################################################

		//######################################################################################################
		// White_Robot Movement
		if (White_Robot_counter >= 250)
		{
			White_Robot_Rand_motion = rand() % 4 + 1;
			White_Robot_counter = 0;
		}

		if (White_Robot_Rand_motion == 1)											// Moving UP => Rand_motion == 1
		{
			White_Robot_counter++;
			if (Grid[GetXindx(White_Robot.getPosition().x + 25)][up.j].getTexture() == &ground || Grid[up.i][up.j].getTexture() == &ground2)
			{

				Movement_Upward(White_Robot, White_Robot_time, White_Robot_clock, White_Robot_position, White_Robot_size, Random_Monsters_Speed);

			}
			else
			{
				White_Robot_counter = 250;
			}

		}


		else if (White_Robot_Rand_motion == 2)									// Moving Down => Rand_motion == 2
		{
			White_Robot_counter++;
			if (Grid[GetXindx(White_Robot.getPosition().x + 25)][down.j].getTexture() == &ground || Grid[down.i][down.j].getTexture() == &ground2)
			{
				Movement_Downward(White_Robot, White_Robot_time, White_Robot_clock, White_Robot_position, White_Robot_size, Random_Monsters_Speed);
			}

			else
			{
				White_Robot_counter = 250;
			}
		}


		else if (White_Robot_Rand_motion == 3)									// Moving Right => Rand_motion == 3
		{
			White_Robot_counter++;
			if (Grid[right.i][right.j].getTexture() == &ground || Grid[right.i][right.j].getTexture() == &ground2)
			{
				Movement_Right(White_Robot, White_Robot_time, White_Robot_clock, White_Robot_position, White_Robot_size, Random_Monsters_Speed);
			}
			else
			{
				White_Robot_counter = 250;
			}
		}


		else if (White_Robot_Rand_motion == 4)									// Moving Left => Rand_motion == 4
		{
			White_Robot_counter++;
			if (Grid[left.i][left.j].getTexture() == &ground || Grid[left.i][left.j].getTexture() == &ground2)
			{
				Movement_Left(White_Robot, White_Robot_time, White_Robot_clock, White_Robot_position, White_Robot_size, Random_Monsters_Speed);
			}
			else
			{
				White_Robot_counter = 250;
			}

		}

		//######################################################################################################



		Event event;
		while (Level_3.pollEvent(event))
		{
			if (event.type == sf::Event::Closed || Keyboard::isKeyPressed(Keyboard::Escape))
				Level_3.close();

		}

		//#####################################################################################################
		// Bombing stuff
			// bombing

		if (bomb_placement_timer < 80)
		{
			bomb_placement_timer++;
		}
		if (Keyboard::isKeyPressed(Keyboard::Key::Space) && bomb_placement_timer == 80 && counter_placed_bombs < boombs) {
			bombs.push_back(bomb_template);
			bombs[bombs.size() - 1].bomb_clock.restart();
			bombs[bombs.size() - 1].explosion_clock.restart();
			bombs[bombs.size() - 1].start_clock.restart();
			bombs[bombs.size() - 1].bomb_placed = 1;
			bombs[bombs.size() - 1].x_pos = GetXpostion(Player_xindx) + 12;
			bombs[bombs.size() - 1].y_pos = GetYpostion(Player_yindx) + 12;
			bombs[bombs.size() - 1].x_index = GetXindx(bombs[bombs.size() - 1].x_pos);
			bombs[bombs.size() - 1].y_index = GetYindx(bombs[bombs.size() - 1].y_pos);
			bombs[bombs.size() - 1].sprt.setPosition(bombs[bombs.size() - 1].x_pos, bombs[bombs.size() - 1].y_pos);
			bombs[bombs.size() - 1].sprt.setTextureRect(IntRect(0, text_size_bomb.y*bombs[bombs.size() - 1].bomb_animation_y, text_size_bomb.x, text_size_bomb.y));
			bomb_placement_timer = 0;
		}

		for (int io = 0; io < bombs.size(); io++)
		{
			if (bombs[io].bomb_placed == 1) {
				bombs[io].bomb_time = bombs[io].bomb_clock.getElapsedTime();
				bombs[io].start_explosion = bombs[io].start_clock.getElapsedTime();
				if (bombs[io].bomb_time.asSeconds() >= 0.2) {
					bombs[io].bomb_clock.restart();
					bombs[io].sprt.setPosition(bombs[io].x_pos, bombs[io].y_pos + 25);
					bombs[io].sprt.setTextureRect(IntRect(0, text_size_bomb.y*bombs[io].bomb_animation_y, text_size_bomb.x, text_size_bomb.y));
					bombs[io].bomb_animation_y += 2;
					if (bombs[io].bomb_animation_y > 2)
						bombs[io].bomb_animation_y = 0;
				}
				if (bombs[io].start_explosion.asSeconds() >= 3) {
					bombs[io].bomb_exploded = 1;
					bombs[io].bomb_placed = 0;
					//                bombs[number_of_bombs].sprt.setColor(Color::Transparent);
					for (int k = 0; k <= range_of_bomb; k++)
					{
						bombs[io].range_right.push_back(exp);
						bombs[io].range_left.push_back(exp);
						bombs[io].range_up.push_back(exp);
						bombs[io].range_down.push_back(exp);
						//                     v[z].push_back(exp);
					}
				}
			}
			//cout<<player.getPosition().x<<' '<<player.getPosition().y<<endl;
			if (bombs[io].bomb_exploded) {
				bombs[io].explosion_time = bombs[io].explosion_clock.getElapsedTime();
				if (bombs[io].explosion_time.asSeconds() >= 0.08) {
					bombs[io].explosion_clock.restart();
					for (int n = 0, k = 0; k < bombs[io].range_right.size(), n <= range_of_bomb * 50; k++, n += 50)
						bombs[io].range_right[k].setPosition(bombs[io].x_pos + n, bombs[io].y_pos);

					for (int n = 0, k = 0; k < bombs[io].range_right.size(), n <= range_of_bomb * 50; k++, n += 50)
						bombs[io].range_right[k].setTextureRect(IntRect(text_size_exp.x*bombs[io].exp_animation_x, text_size_exp.y*bombs[io].exp_animation_y, text_size_exp.x, text_size_exp.y));

					for (int n = 0, k = 0; k < bombs[io].range_left.size(), n <= range_of_bomb * 50; k++, n += 50)
						bombs[io].range_left[k].setPosition(bombs[io].x_pos - n, bombs[io].y_pos);

					for (int n = 0, k = 0; k < bombs[io].range_left.size(), n <= range_of_bomb * 50; k++, n += 50)
						bombs[io].range_left[k].setTextureRect(IntRect(text_size_exp.x*bombs[io].exp_animation_x, text_size_exp.y*bombs[io].exp_animation_y, text_size_exp.x, text_size_exp.y));

					for (int n = 0, k = 0; k < bombs[io].range_up.size(), n <= range_of_bomb * 50; k++, n += 50)
						bombs[io].range_up[k].setPosition(bombs[io].x_pos, bombs[io].y_pos - n);

					for (int n = 0, k = 0; k < bombs[io].range_up.size(), n <= range_of_bomb * 50; k++, n += 50)
						bombs[io].range_up[k].setTextureRect(IntRect(text_size_exp.x*bombs[io].exp_animation_x, text_size_exp.y*bombs[io].exp_animation_y, text_size_exp.x, text_size_exp.y));

					for (int n = 0, k = 0; k < bombs[io].range_down.size(), n <= range_of_bomb * 50; k++, n += 50)
						bombs[io].range_down[k].setPosition(bombs[io].x_pos, bombs[io].y_pos + n);

					for (int n = 0, k = 0; k < bombs[io].range_down.size(), n <= range_of_bomb * 50; k++, n += 50)
						bombs[io].range_down[k].setTextureRect(IntRect(text_size_exp.x*bombs[io].exp_animation_x, text_size_exp.y*bombs[io].exp_animation_y, text_size_exp.x, text_size_exp.y));

					bombs[io].exp_animation_x++;
					if (bombs[io].exp_animation_x == 3) {
						bombs[io].exp_animation_x = 0, bombs[io].exp_animation_y++;
						if (bombs[io].exp_animation_y == 3) {
							bombs[io].bomb_exploded = 0;
							bombs.erase(bombs.begin());

						}
					}
				}
			}
		}

		// Drawing of the Wallpaper
		Level_3.clear();
		Level_3.draw(Wallpaper);

		//Drawing of The Map
		for (int i = 2; i < width; i++)
		{
			for (int j = 2; j < length; j++)
			{
				if (Grid[i][j].getTexture() == &ground || Grid[i][j].getTexture() == &ground2)
				{
					Level_3.draw(Grid[i][j]);
				}
			}

		}

		for (int i = 0; i < 14; i++)
		{
			Level_3.draw(SuperPowers[i]);
		}
		for (int j = 1; j <= length; j++)
		{
			for (int i = 1; i <= width; i++)
			{
				if (Grid[i][j].getTexture() == &ground || Grid[i][j].getTexture() == &ground2)
				{
					continue;
				}

				Level_3.draw(Grid[i][j]);

				if (GetYindx(player.getPosition().y) >= j)
				{
					Level_3.draw(player);
				}

				if (GetYindx(Skeleton.getPosition().y) >= j&&!Skeleton_death)
				{
					Level_3.draw(Skeleton);
				}

				if (GetYindx(White_Robot.getPosition().y) >= j&&!White_Robot_death)
				{
					Level_3.draw(White_Robot);
				}

				if (GetYindx(Blue_Skeleton.getPosition().y) >= j&&!Blue_Skeleton_death)
				{
					Level_3.draw(Blue_Skeleton);
				}

				if (GetYindx(Red_Robot.getPosition().y) >= j&&!Red_Robot_death)
				{
					Level_3.draw(Red_Robot);
				}
				for (int io = 0; io < bombs.size(); io++)
				{
					if (bombs[io].y_index >= j && bombs[io].bomb_placed)
					{
						Level_3.draw(bombs[io].sprt);
					}
				}
			}
		}



		for (int io = 0; io < bombs.size(); io++)
		{
			//if (bombs[io].bomb_placed)
				//window.draw(bombs[io].sprt);
			if (bombs[io].bomb_exploded)
			{

				//Right
				for (int g = 0; g <= range_of_bomb; g++)
				{

					Level_3.draw(bombs[io].range_right[g]);

					if ((Grid[bombs[io].x_index + g][bombs[io].y_index].getTexture() == &breakable) && bombs[io].flag_range_right)
					{
						destroy_breakable_walls(bombs[io].x_index + g, bombs[io].y_index, Grid, ground, ground2);
						bombs[io].flag_range_right = 0;
						break;
					}
					else if (Grid[bombs[io].x_index + g][bombs[io].y_index].getTexture() == &wall)
					{

						bombs[io].flag_range_right = 0;
						break;
					}

					if (bombs[io].x_index + g == Player_xindx && bombs[io].y_index == Player_yindx && bombs[io].flag_range_right)
					{
						if (Clock_Damgae.getElapsedTime().asSeconds() >= 1) {
							lives--;
							ssl = to_string(lives);
							Clock_Damgae.restart();
						}
						Level_3.draw(Hurt);
					}

					if (bombs[io].x_index + g == Red_Robot_xindx && bombs[io].y_index == Red_Robot_yindx && bombs[io].flag_range_right)
					{
						Red_Robot_death = 1;
						Red_Robot.setFillColor(Color::Transparent);
					}
					if (bombs[io].x_index + g == White_Robot_xindx && bombs[io].y_index == White_Robot_yindx && bombs[io].flag_range_right)
					{
						White_Robot_death = 1;
						White_Robot.setFillColor(Color::Transparent);
					}
					if (bombs[io].x_index + g == Skeleton_xindx && bombs[io].y_index == Skeleton_yindx && bombs[io].flag_range_right)
					{
						Skeleton_death = 1;
						Skeleton.setFillColor(Color::Transparent);
					}
					if (bombs[io].x_index + g == GetXindx(Blue_Skeleton.getPosition().x) && bombs[io].y_index == GetYindx(Blue_Skeleton.getPosition().y) && bombs[io].flag_range_right)
					{
						Blue_Skeleton_death = 1;
						Blue_Skeleton.setFillColor(Color::Transparent);
					}


				}


				//UP
				for (int g = 0; g <= range_of_bomb; g++)
				{

					Level_3.draw(bombs[io].range_up[g]);

					if ((Grid[bombs[io].x_index][bombs[io].y_index - g].getTexture() == &breakable) && bombs[io].flag_range_up)
					{


						destroy_breakable_walls(bombs[io].x_index, bombs[io].y_index - g, Grid, ground, ground2);

						bombs[io].flag_range_up = 0;
						break;

					}
					else if (Grid[bombs[io].x_index][bombs[io].y_index - g].getTexture() == &wall)
					{

						bombs[io].flag_range_up = 0;
						break;
					}

					if (bombs[io].x_index == Player_xindx && bombs[io].y_index - g == Player_yindx && bombs[io].flag_range_up)
					{
						if (Clock_Damgae.getElapsedTime().asSeconds() >= 1) {
							lives--;
							ssl = to_string(lives);
							Clock_Damgae.restart();
						}
						Level_3.draw(Hurt);
					}

					if (bombs[io].x_index == Red_Robot_xindx && bombs[io].y_index - g == Red_Robot_yindx && bombs[io].flag_range_up)
					{
						Red_Robot_death = 1;
						Red_Robot.setFillColor(Color::Transparent);
					}
					if (bombs[io].x_index == White_Robot_xindx && bombs[io].y_index - g == White_Robot_yindx && bombs[io].flag_range_up)
					{
						White_Robot_death = 1;
						White_Robot.setFillColor(Color::Transparent);
					}
					if (bombs[io].x_index == Skeleton_xindx && bombs[io].y_index - g == Skeleton_yindx && bombs[io].flag_range_up)
					{
						Skeleton_death = 1;
						Skeleton.setFillColor(Color::Transparent);
					}
					if (bombs[io].x_index == GetXindx(Blue_Skeleton.getPosition().x) && bombs[io].y_index - g == GetYindx(Blue_Skeleton.getPosition().y) && bombs[io].flag_range_up)
					{
						Blue_Skeleton_death = 1;
						Blue_Skeleton.setFillColor(Color::Transparent);
					}

				}

				//Left
				for (int g = 0; g <= range_of_bomb; g++)
				{

					Level_3.draw(bombs[io].range_left[g]);

					if ((Grid[bombs[io].x_index - g][bombs[io].y_index].getTexture() == &breakable) && bombs[io].flag_range_left)
					{
						destroy_breakable_walls(bombs[io].x_index - g, bombs[io].y_index, Grid, ground, ground2);

						bombs[io].flag_range_left = 0;
						break;
					}
					else if (Grid[bombs[io].x_index - g][bombs[io].y_index].getTexture() == &wall)
					{
						bombs[io].flag_range_left = 0;
						break;
					}
					if (bombs[io].x_index - g == Player_xindx && bombs[io].y_index == Player_yindx && bombs[io].flag_range_left)
					{
						if (Clock_Damgae.getElapsedTime().asSeconds() >= 1) {
							lives--;
							//SuperPowers[i].setColor(Color::Transparent);
							ssl = to_string(lives);
							//counter.restart();
							Clock_Damgae.restart();
						}
						Level_3.draw(Hurt);
					}

					if (bombs[io].x_index - g == Red_Robot_xindx - g && bombs[io].y_index == Red_Robot_yindx && bombs[io].flag_range_left)
					{
						Red_Robot_death = 1;
						Red_Robot.setFillColor(Color::Transparent);
					}
					if (bombs[io].x_index - g == White_Robot_xindx && bombs[io].y_index == White_Robot_yindx && bombs[io].flag_range_left)
					{
						White_Robot_death = 1;
						White_Robot.setFillColor(Color::Transparent);
					}
					if (bombs[io].x_index - g == Skeleton_xindx && bombs[io].y_index == Skeleton_yindx && bombs[io].flag_range_left)
					{
						Skeleton_death = 1;
						Skeleton.setFillColor(Color::Transparent);
					}
					if (bombs[io].x_index - g == GetXindx(Blue_Skeleton.getPosition().x) && bombs[io].y_index == GetYindx(Blue_Skeleton.getPosition().y) && bombs[io].flag_range_left)
					{
						Blue_Skeleton_death = 1;
						Blue_Skeleton.setFillColor(Color::Transparent);
					}

				}

				//Down
				for (int g = 0; g <= range_of_bomb; g++)
				{

					Level_3.draw(bombs[io].range_down[g]);
					if ((Grid[bombs[io].x_index][bombs[io].y_index + g].getTexture() == &breakable) && bombs[io].flag_range_down)
					{
						destroy_breakable_walls(bombs[io].x_index, bombs[io].y_index + g, Grid, ground, ground2);
						bombs[io].flag_range_down = 0;
						break;
					}
					else if (Grid[bombs[io].x_index][bombs[io].y_index + g].getTexture() == &wall)
					{

						bombs[io].flag_range_down = 0;
						break;
					}
					if (bombs[io].x_index == Player_xindx && bombs[io].y_index + g == Player_yindx && bombs[io].flag_range_down)
					{
						if (Clock_Damgae.getElapsedTime().asSeconds() >= 1) {
							lives--;
							ssl = to_string(lives);
							Clock_Damgae.restart();
						}
						Level_3.draw(Hurt);
					}

					if (bombs[io].x_index == Red_Robot_xindx && bombs[io].y_index + g == Red_Robot_yindx && bombs[io].flag_range_down)
					{
						Red_Robot_death = 1;
						Red_Robot.setFillColor(Color::Transparent);
					}
					if (bombs[io].x_index == White_Robot_xindx && bombs[io].y_index + g == White_Robot_yindx && bombs[io].flag_range_down)
					{
						White_Robot_death = 1;
						White_Robot.setFillColor(Color::Transparent);
					}
					if (bombs[io].x_index == Skeleton_xindx && bombs[io].y_index + g == Skeleton_yindx && bombs[io].flag_range_down)
					{
						Skeleton_death = 1;
						Skeleton.setFillColor(Color::Transparent);
					}
					if (bombs[io].x_index == GetXindx(Blue_Skeleton.getPosition().x) && bombs[io].y_index + g == GetYindx(Blue_Skeleton.getPosition().y) && bombs[io].flag_range_down)
					{
						Blue_Skeleton_death = 1;
						Blue_Skeleton.setFillColor(Color::Transparent);
					}

				}

			}
		}

		//############################################################################################

		//############################################################################################//############################################################################################

		for (int i = 0; i < 13; i++) {
			int sumP = 0; int sumR = 0;
			sumP = GetXindx(player.getPosition().x) + GetYindx(player.getPosition().y);
			sumR = GetXindx(SuperPowers[i].getPosition().x) + GetYindx(SuperPowers[i].getPosition().y);

			if (sumR == sumP + 1 && (GetXindx(SuperPowers[i].getPosition().x) == GetXindx(player.getPosition().x))
				&& !visted[GetXindx(SuperPowers[i].getPosition().x)][GetYindx(SuperPowers[i].getPosition().y)])
			{

				visted[GetXindx(SuperPowers[i].getPosition().x)][GetYindx(SuperPowers[i].getPosition().y)] = 1;

				if (SuperPowers[i].getTexture() == &rangeskill) {


					temp_range_of_bomb++;
					SuperPowers[i].setColor(Color::Transparent);
					ss = to_string(temp_range_of_bomb);

				}
				else if (SuperPowers[i].getTexture() == &bombskill) {
					boombs++;
					SuperPowers[i].setColor(Color::Transparent);
					sss = to_string(boombs);


				}
				else if (SuperPowers[i].getTexture() == &healskill) {
					lives++;
					SuperPowers[i].setColor(Color::Transparent);
					ssl = to_string(lives);
				}
			}

		}



		bool isbombing = 0;
		for (int io = 0; io < bombs.size(); io++)
		{
			if (bombs[io].bomb_exploded) { isbombing = 1; break; }
		}
		if (!isbombing)
		{
			range_of_bomb = temp_range_of_bomb;
		}
		//############################################################################################
		// player damage
		if (GetXindx(player.getPosition().x) == GetXindx(White_Robot.getPosition().x) && GetYindx(player.getPosition().y) == GetYindx(White_Robot.getPosition().y)&&!White_Robot_death)
		{
			if (Clock_Damgae.getElapsedTime().asSeconds() >= 1) {
				lives--;
				Clock_Damgae.restart();
			}
			Level_3.draw(Hurt);

		}

		if (GetXindx(player.getPosition().x) == GetXindx(Skeleton.getPosition().x) && GetYindx(player.getPosition().y) == GetYindx(Skeleton.getPosition().y)&&!Skeleton_death)
		{
			if (Clock_Damgae.getElapsedTime().asSeconds() >= 1) {
				lives--;
				Clock_Damgae.restart();
			}
			Level_3.draw(Hurt);
		}

		if (GetXindx(player.getPosition().x) == GetXindx(Blue_Skeleton.getPosition().x) && GetYindx(player.getPosition().y) == GetYindx(Blue_Skeleton.getPosition().y)&&!Blue_Skeleton_death)
		{
			if (Clock_Damgae.getElapsedTime().asSeconds() >= 1) {
				lives--;
				Clock_Damgae.restart();
			}
			Level_3.draw(Hurt);
		}

		if (GetXindx(player.getPosition().x) == GetXindx(Red_Robot.getPosition().x) && GetYindx(player.getPosition().y) == GetYindx(Red_Robot.getPosition().y)&&!Red_Robot_death)
		{
			if (Clock_Damgae.getElapsedTime().asSeconds() >= 1) {
				lives--;
				Clock_Damgae.restart();
			}
			Level_3.draw(Hurt);
		}

		cout << player_status.lives << endl;
		Level_3.draw(nooflives);
		Level_3.draw(noofbombs);
		Level_3.draw(noofranges);
		Level_3.draw(lifeS);
		Level_3.draw(rangeS);
		Level_3.draw(bombS);
		Level_3.draw(txt); // بيترسم تحت
		if (lives < 1) {
			ingame.stop();
			player.setFillColor(Color::Transparent);
			Tomb.setPosition(player.getPosition().x, player.getPosition().y);
			Level_3.draw(Tomb);
			--elapsed;
			if (elapsed <= 50 + 500) {
				if (playsong) {
					lost.play();
					playsong = 0;
				}
				if (elapsed <= -500) {
					Level_3.clear(Color::Black), Level_3.draw(RIP_Photo);
					exit(0);
				}
			}
		}

		if (Die) {
			ingame.stop();
			Level_3.clear(Color::Black), Level_3.draw(big_exp), Level_3.draw(RIP_Photo);
			--elapsed;
			if (elapsed <= 1) {
				exit(0);
			}
		}

		if (Player_xindx == 16 && Player_yindx == 3) {
			ingame.stop();
			Level_3.clear(Color::Black), Level_3.draw(Level_complete);
			--elapsed;
			if (elapsed <= 1) {
				exit(0);
			}
		}
		Level_3.display();



	}

}

void Multiplayer()
{

	Font dis_time;
	if (!dis_time.loadFromFile("Chocolate Bar Demo.otf"))
	{
		cout << "error font";
	}
	//timer stuff
	int display_timer;
	Clock c, timer_clock;
	Time times, timer;
	//map stuff
	Texture wall, ground, wallpaper, ground2, breakable, zone_warning_text, hurt, tomb;
	Texture death;
	Sprite Death;
	if (!death.loadFromFile("death.png"))
		cout << "error to load death " << endl;

	if (!tomb.loadFromFile("RIP.png"))
		cout << "error to load death " << endl;



	Death.setTexture(death);
	Death.setScale(0.3, 0.3);
	// songs load
	Music ingame; ingame.openFromFile("ingame.wav");
	ingame.play();
	ingame.setLoop(1);

	// zone stuff
	Sprite zone_warning_sprt, Hurt;
	if (!zone_warning_text.loadFromFile("zone.png")) {
		cout << "error to load zone " << endl;
	}
	zone_warning_sprt.setTexture(zone_warning_text);
	zone_warning_sprt.setScale(0.5, 0.5);
	zone_warning_sprt.setPosition(440, 520);
	int zone_limit = 1;
	if (!breakable.loadFromFile("breakable.png")) {
		cout << "error to load breakable " << endl;
	}
	if (!hurt.loadFromFile("hurt.png")) {
		cout << "error to load breakable " << endl;
	}
	Hurt.setTexture(hurt);
	if (!wallpaper.loadFromFile("wallpaper.jpg")) {
		cout << "error to load wallpaper " << endl;
	}

	if (!wall.loadFromFile("3d wall.png")) {
		cout << "error to load 3d wall " << endl;
	}
	if (!ground.loadFromFile("ground 1.png")) {
		cout << "error to load ground " << endl;
	}




	if (!ground2.loadFromFile("grass.png")) {
		cout << "error to load grass " << endl;
	}


	Sprite lifeS, rangeS, bombS, Wallpaper;

	Wallpaper.setTexture(wallpaper);


	// Distribution of wall & ground on the Grid

	const int length = 9, width = 17;

	Sprite Grid[width + 1][length + 1];

	for (int i = 1; i <= width; i++)
	{
		for (int j = 1; j <= length; j++)
		{
			if (i % 2 != 0 && j % 2 != 0)
			{

				Grid[i][j].setTexture(wall);
				Grid[i][j].setPosition((i + 1.97) * 53, (j + 0.255) * 48);
				Grid[i][j].setScale(Vector2f(0.1009, 0.165));

			}

			else if (i == 1 || i == width)
			{
				Grid[i][j].setTexture(wall);
				Grid[i][j].setPosition((i + 1.97) * 53, (j + 0.255) * 48);
				Grid[i][j].setScale(Vector2f(0.1009, 0.165));
			}
			else if (j == 1 || j == length)
			{
				Grid[i][j].setTexture(wall);
				Grid[i][j].setPosition((i + 1.97) * 53, (j + 0.255) * 48);
				Grid[i][j].setScale(Vector2f(0.1009, 0.165));
			}
			else
			{
				if (i % 2 == j % 2)
				{
					Grid[i][j].setTexture(ground);
				}
				else
				{
					Grid[i][j].setTexture(ground2);
				}
				Grid[i][j].setPosition((i + 2) * 53, (j + 0.8) * 48);
				Grid[i][j].setScale(0.1009, 0.101);
			}
		}
	}
	for (int i = 1; i <= 30; i++)
	{
		int x = rand() % 14 + 2, y = rand() % 7 + 2;
		if (x % 2 != 0 && y % 2 != 0)
		{
			x++;
		}
		Grid[x][y].setTexture(breakable);
		Grid[x][y].setPosition((x + 1.97) * 53, (y + 0.255) * 48);
		Grid[x][y].setScale(Vector2f(0.1009, 0.165));
	}

	// player 1 creation
	const int players_speed = 1;
	//###############################################################################
	// Making of Player1
	RectangleShape player1(Vector2f(90, 110));
	Sprite dead;
	dead.setScale(0.1, 0.1);
	dead.setTexture(tomb);
	Texture player1_texture;
	Vector2u player1_size;
	Clock player1_clock;
	Time player1_time;
	float player1_position = 0;
	Generate_Charcter(player1, player1_texture, player1_size, "mainplayer.png", 10, 2);

	//###############################################################################
  // Making of Player2
	RectangleShape player2(Vector2f(90, 110));
	Texture player2_texture;
	Vector2u player2_size;
	Clock player2_clock;
	Time player2_time;
	float player2_position = 0;
	Generate_Charcter(player2, player2_texture, player2_size, "mage walking poses sheet copyRR.png", 2, 2);


	//###############################################################################
   // Movement stuff
	leftmovement left; rightmovement right; upmovement up; downmovement down;
	int player1_xindx = 0, player1_yindx = 0, player2_xindx = 0, player2_yindx = 0;
	//###############################################################################



	//bomb stuff
	Texture bombtext, exptext;
	bombtext.loadFromFile("BOOM.png");
	exptext.loadFromFile("exp.png");
	Sprite exp;
	exp.setTexture(exptext);
	exp.setScale(0.25, 0.25);
	bomb bomb_template;
	bomb_template.sprt.setTexture(bombtext);
	bomb_template.sprt.setScale(0.15f, 0.15f);
	vector<bomb>bombs;

	Vector2u text_size_bomb = bombtext.getSize();
	Vector2u text_size_exp = exptext.getSize();
	text_size_bomb.y /= 4;
	text_size_exp.x /= 3;
	text_size_exp.y /= 3;
	int range_of_bomb = 3;



	Sprite P1_wins, P2_wins, P_Draw;
	Texture P1_wins_txt, P2_wins_txt, P_draw_txt;
	P1_wins_txt.loadFromFile("player 1 wins.png");
	P2_wins_txt.loadFromFile("player 2 wins.png");
	P_draw_txt.loadFromFile("Draw.png");
	P1_wins.setTexture(P1_wins_txt);
	P2_wins.setTexture(P2_wins_txt);
	P_Draw.setTexture(P_draw_txt);


	Time music_time;
	Clock music_clock;

	int death_music_duration = 0;

	bool player1_wins_flag = 0, player2_wins_flag = 0;

	int bomb_placement_timer_1 = 0, bomb_placement_timer_2 = 0;

	//Game Window
	RenderWindow window(VideoMode(1280, 768), "Fcis In Danger");
	while (window.isOpen())
	{



		Event event;
		while (window.pollEvent(event))
		{
			if (event.type == sf::Event::Closed)
				window.close();
		}





		music_time = music_clock.getElapsedTime();
		death_music_duration = music_time.asSeconds();




		// Timer
		Text txt;
		timer = timer_clock.getElapsedTime();
		display_timer = timer.asSeconds();
		string s1, s2;
		//int  minu=display_timer/60;
		s1 = to_string(30 - (display_timer));
		//s2=to_string(1-minu);
		//s2+=" : ";
		//s2+=s1;

		txt.setFont(dis_time);
		txt.setString("0:" + s1);
		txt.setPosition(512, 22);


		if (display_timer >= 30)
		{
			timer_clock.restart();


			for (int j = 2; j < length; j++)
			{
				Grid[zone_limit + 1][j].setTexture(wall);

				Grid[width - zone_limit][j].setTexture(wall);

				Grid[zone_limit + 1][j].setPosition((zone_limit + 1 + 1.97) * 53, (j + 0.255) * 48);

				if ((GetXindx(player1.getPosition().x)) == zone_limit + 1) {
					dead.setPosition(player1.getPosition().x, player1.getPosition().y);
					player2_wins_flag = 1;
					music_clock.restart();
					ingame.stop();

				}

				if ((GetXindx(player2.getPosition().x)) == zone_limit + 1) {
					dead.setPosition(player2.getPosition().x, player2.getPosition().y);
					player1_wins_flag = 1;
					music_clock.restart();
					ingame.stop();
				}

				Grid[width - zone_limit][j].setPosition((width - zone_limit + 1.97) * 53, (j + 0.255) * 48);

				Grid[zone_limit + 1][j].setScale(Vector2f(0.1009, 0.165));

				Grid[width - zone_limit][j].setScale(Vector2f(0.1009, 0.165));



				if ((GetXindx(player1.getPosition().x)) == width - zone_limit) {
					dead.setPosition(player1.getPosition().x, player1.getPosition().y);
					player2_wins_flag = 1;
					music_clock.restart();
					ingame.stop();

				}

				if ((GetXindx(player2.getPosition().x)) == width - zone_limit) {
					dead.setPosition(player2.getPosition().x, player2.getPosition().y);
					player1_wins_flag = 1;
					music_clock.restart();
					ingame.stop();

				}






			}
			zone_limit++;
		}


		
		


		// player1 1 Movement
		player1_xindx = (float)((float)(player1.getPosition().x - 183) / 53 + 2);
		player1_yindx = (float)((float)(player1.getPosition().y - 74) / 48 + 2);
		right.i = GetXindx(player1.getPosition().x + 20); right.j = player1_yindx;
		left.i = GetXindx(player1.getPosition().x - 20); left.j = player1_yindx;
		up.i = player1_xindx; up.j = GetYindx(player1.getPosition().y - 7);
		down.i = player1_xindx; down.j = GetYindx(player1.getPosition().y + 7);

		Vector2i movejoy1(Joystick::getAxisPosition(0, Joystick::X), Joystick::getAxisPosition(0, Joystick::Y));
		if ((Keyboard::isKeyPressed(Keyboard::Key::Right)||movejoy1.x>0) && (Grid[right.i][right.j].getTexture() == &ground || Grid[right.i][right.j].getTexture() == &ground2))      // Moving Right
		{
			bool check_bomb = 0;
			for (int io = 0; io < bombs.size(); io++)
			{
				if (bombs[io].x_index == GetXindx(player1.getPosition().x + 1) && bombs[io].x_index != GetXindx(player1.getPosition().x) && bombs[io].y_index == right.j)
				{
					check_bomb = 1; break;
				}
			}
			if (!check_bomb)
				Movement_Right(player1, player1_time, player1_clock, player1_position, player1_size, players_speed);
		}
		else if ((Keyboard::isKeyPressed(Keyboard::Key::Left) || movejoy1.x < 0) && (Grid[left.i][left.j].getTexture() == &ground || Grid[left.i][left.j].getTexture() == &ground2))         // Moving Left
		{
			bool check_bomb = 0;
			for (int io = 0; io < bombs.size(); io++)
			{
				if (bombs[io].x_index == GetXindx(player1.getPosition().x - 1) && bombs[io].x_index != GetXindx(player1.getPosition().x) && bombs[io].y_index == left.j)
				{
					check_bomb = 1; break;
				}
			}
			if (!check_bomb)

				Movement_Left(player1, player1_time, player1_clock, player1_position, player1_size, players_speed);
		}
		else if ((Keyboard::isKeyPressed(Keyboard::Key::Up) || movejoy1.y < 0) && (Grid[GetXindx(player1.getPosition().x + 25)][up.j].getTexture() == &ground || Grid[up.i][up.j].getTexture() == &ground2))           // Moving Up
		{
			bool check_bomb = 0;
			for (int io = 0; io < bombs.size(); io++)
			{
				if (bombs[io].y_index == GetYindx(player1.getPosition().y - 1) && bombs[io].y_index != GetYindx(player1.getPosition().y) && bombs[io].x_index == up.i)
				{
					check_bomb = 1; break;
				}
			}
			if (!check_bomb)


				Movement_Upward(player1, player1_time, player1_clock, player1_position, player1_size, players_speed);
		}
		else if ((Keyboard::isKeyPressed(Keyboard::Key::Down) || movejoy1.y > 0) && (Grid[GetXindx(player1.getPosition().x + 25)][down.j].getTexture() == &ground || Grid[down.i][down.j].getTexture() == &ground2))         // Moving Down
		{
			bool check_bomb = 0;
			for (int io = 0; io < bombs.size(); io++)
			{
				if (bombs[io].y_index == GetYindx(player1.getPosition().y + 1) && bombs[io].y_index != GetYindx(player1.getPosition().y) && bombs[io].x_index == up.i)
				{
					check_bomb = 1; break;
				}
			}
			if (!check_bomb)

				Movement_Downward(player1, player1_time, player1_clock, player1_position, player1_size, players_speed);
		}
		if ((Keyboard::isKeyPressed(Keyboard::Key::Enter)||Joystick::isButtonPressed(0,2)) && bomb_placement_timer_1 >= 80) {
			bombs.push_back(bomb_template);
			bombs[bombs.size() - 1].bomb_clock.restart();
			bombs[bombs.size() - 1].explosion_clock.restart();
			bombs[bombs.size() - 1].start_clock.restart();
			bombs[bombs.size() - 1].bomb_placed = 1;
			bombs[bombs.size() - 1].x_pos = GetXpostion(player1_xindx) + 12;
			bombs[bombs.size() - 1].y_pos = GetYpostion(player1_yindx) + 12;
			bombs[bombs.size() - 1].x_index = GetXindx(bombs[bombs.size() - 1].x_pos);
			bombs[bombs.size() - 1].y_index = GetYindx(bombs[bombs.size() - 1].y_pos);
			bombs[bombs.size() - 1].sprt.setPosition(bombs[bombs.size() - 1].x_pos, bombs[bombs.size() - 1].y_pos);
			bombs[bombs.size() - 1].sprt.setTextureRect(IntRect(0, text_size_bomb.y*bombs[bombs.size() - 1].bomb_animation_y, text_size_bomb.x, text_size_bomb.y));
			bomb_placement_timer_1 = 0;
		}

		if (bomb_placement_timer_1 < 80) { bomb_placement_timer_1++; }
		if (bomb_placement_timer_2 < 80) { bomb_placement_timer_2++; }






		// Player 2 Movement
		player2_xindx = (float)((float)(player2.getPosition().x - 183) / 53 + 2);
		player2_yindx = (float)((float)(player2.getPosition().y - 74) / 48 + 2);
		right.i = GetXindx(player2.getPosition().x + 20); right.j = player2_yindx;
		left.i = GetXindx(player2.getPosition().x - 20); left.j = player2_yindx;
		up.i = player2_xindx; up.j = GetYindx(player2.getPosition().y - 7);
		down.i = player2_xindx; down.j = GetYindx(player2.getPosition().y + 7);
		Vector2i movejoy2(Joystick::getAxisPosition(1, Joystick::X), Joystick::getAxisPosition(1, Joystick::Y));

		if ((Keyboard::isKeyPressed(Keyboard::Key::D)|| movejoy2.x>0) && (Grid[right.i][right.j].getTexture() == &ground || Grid[right.i][right.j].getTexture() == &ground2))      // Moving Right
		{
			bool check_bomb = 0;
			for (int io = 0; io < bombs.size(); io++)
			{
				if (bombs[io].x_index == GetXindx(player2.getPosition().x + 1) && bombs[io].x_index != GetXindx(player2.getPosition().x) && bombs[io].y_index == GetYindx(player2.getPosition().y))
				{
					check_bomb = 1; break;
				}
			}
			if (!check_bomb)

				Movement_Right(player2, player2_time, player2_clock, player2_position, player2_size, players_speed);
		}
		else if ((Keyboard::isKeyPressed(Keyboard::Key::A) || movejoy2.x < 0) && (Grid[left.i][left.j].getTexture() == &ground || Grid[left.i][left.j].getTexture() == &ground2))         // Moving Left
		{
			bool check_bomb = 0;
			for (int io = 0; io < bombs.size(); io++)
			{
				if (bombs[io].x_index == GetXindx(player2.getPosition().x - 1) && bombs[io].x_index != GetXindx(player2.getPosition().x) && bombs[io].y_index == GetYindx(player2.getPosition().y))
				{
					check_bomb = 1; break;
				}
			}
			if (!check_bomb)


				Movement_Left(player2, player2_time, player2_clock, player2_position, player2_size, players_speed);
		}
		else if ((Keyboard::isKeyPressed(Keyboard::Key::W) || movejoy2.y < 0) && (Grid[GetXindx(player2.getPosition().x + 25)][up.j].getTexture() == &ground || Grid[up.i][up.j].getTexture() == &ground2))           // Moving Up
		{

			bool check_bomb = 0;
			for (int io = 0; io < bombs.size(); io++)
			{
				if (bombs[io].y_index == GetYindx(player2.getPosition().y - 1) && bombs[io].y_index != GetYindx(player2.getPosition().y) && bombs[io].x_index == GetXindx(player2.getPosition().x))
				{
					check_bomb = 1; break;
				}
			}
			if (!check_bomb)

				Movement_Upward(player2, player2_time, player2_clock, player2_position, player2_size, players_speed);
		}
		else if ((Keyboard::isKeyPressed(Keyboard::Key::S) || movejoy2.y > 0) && (Grid[GetXindx(player2.getPosition().x + 25)][down.j].getTexture() == &ground || Grid[down.i][down.j].getTexture() == &ground2))         // Moving Down
		{


			bool check_bomb = 0;
			for (int io = 0; io < bombs.size(); io++)
			{
				if (bombs[io].y_index == GetYindx(player2.getPosition().y + 1) && bombs[io].y_index != GetYindx(player2.getPosition().y) && bombs[io].x_index == GetXindx(player2.getPosition().x))
				{
					check_bomb = 1; break;
				}
			}
			if (!check_bomb)


				Movement_Downward(player2, player2_time, player2_clock, player2_position, player2_size, players_speed);
		}
		else if ((Keyboard::isKeyPressed(Keyboard::Key::Space) || Joystick::isButtonPressed(1, 2)) && bomb_placement_timer_2 >= 80) {
			bombs.push_back(bomb_template);
			bombs[bombs.size() - 1].bomb_clock.restart();
			bombs[bombs.size() - 1].explosion_clock.restart();
			bombs[bombs.size() - 1].start_clock.restart();
			bombs[bombs.size() - 1].bomb_placed = 1;
			bombs[bombs.size() - 1].x_pos = GetXpostion(player2_xindx) + 12;
			bombs[bombs.size() - 1].y_pos = GetYpostion(player2_yindx) + 12;
			bombs[bombs.size() - 1].x_index = GetXindx(bombs[bombs.size() - 1].x_pos);
			bombs[bombs.size() - 1].y_index = GetYindx(bombs[bombs.size() - 1].y_pos);
			bombs[bombs.size() - 1].y_index = GetYindx(bombs[bombs.size() - 1].y_pos);

			bombs[bombs.size() - 1].sprt.setPosition(bombs[bombs.size() - 1].x_pos, bombs[bombs.size() - 1].y_pos);

			bombs[bombs.size() - 1].sprt.setTextureRect(IntRect(0, text_size_bomb.y*bombs[bombs.size() - 1].bomb_animation_y, text_size_bomb.x, text_size_bomb.y));

			bomb_placement_timer_2 = 0;
		}




		for (int io = 0; io < bombs.size(); io++)
		{

			if (bombs[io].bomb_placed == 1) {
				bombs[io].bomb_time = bombs[io].bomb_clock.getElapsedTime();
				bombs[io].start_explosion = bombs[io].start_clock.getElapsedTime();
				if (bombs[io].bomb_time.asSeconds() >= 0.2) {
					bombs[io].bomb_clock.restart();
					bombs[io].sprt.setPosition(bombs[io].x_pos, bombs[io].y_pos + 25);
					bombs[io].sprt.setTextureRect(IntRect(0, text_size_bomb.y*bombs[io].bomb_animation_y, text_size_bomb.x, text_size_bomb.y));
					bombs[io].bomb_animation_y += 2;
					if (bombs[io].bomb_animation_y > 2)
						bombs[io].bomb_animation_y = 0;
				}
				if (bombs[io].start_explosion.asSeconds() >= 3) {
					bombs[io].bomb_exploded = 1;
					bombs[io].bomb_placed = 0;

					for (int k = 0; k <= range_of_bomb; k++)
					{
						bombs[io].range_right.push_back(exp);
						bombs[io].range_left.push_back(exp);
						bombs[io].range_up.push_back(exp);
						bombs[io].range_down.push_back(exp);

					}
				}
			}

			if (bombs[io].bomb_exploded) {
				bombs[io].explosion_time = bombs[io].explosion_clock.getElapsedTime();
				if (bombs[io].explosion_time.asSeconds() >= 0.08) {
					bombs[io].explosion_clock.restart();
					for (int n = 0, k = 0; k <= range_of_bomb, n <= range_of_bomb * 50; k++, n += 50)
						bombs[io].range_right[k].setPosition(bombs[io].x_pos + n, bombs[io].y_pos);

					for (int n = 0, k = 0; k <= range_of_bomb, n <= range_of_bomb * 50; k++, n += 50)
						bombs[io].range_right[k].setTextureRect(IntRect(text_size_exp.x*bombs[io].exp_animation_x, text_size_exp.y*bombs[io].exp_animation_y, text_size_exp.x, text_size_exp.y));

					for (int n = 0, k = 0; k <= range_of_bomb, n <= range_of_bomb * 50; k++, n += 50)
						bombs[io].range_left[k].setPosition(bombs[io].x_pos - n, bombs[io].y_pos);

					for (int n = 0, k = 0; k <= range_of_bomb, n <= range_of_bomb * 50; k++, n += 50)
						bombs[io].range_left[k].setTextureRect(IntRect(text_size_exp.x*bombs[io].exp_animation_x, text_size_exp.y*bombs[io].exp_animation_y, text_size_exp.x, text_size_exp.y));

					for (int n = 0, k = 0; k <= range_of_bomb, n <= range_of_bomb * 50; k++, n += 50)
						bombs[io].range_up[k].setPosition(bombs[io].x_pos, bombs[io].y_pos - n);

					for (int n = 0, k = 0; k <= range_of_bomb, n <= range_of_bomb * 50; k++, n += 50)
						bombs[io].range_up[k].setTextureRect(IntRect(text_size_exp.x*bombs[io].exp_animation_x, text_size_exp.y*bombs[io].exp_animation_y, text_size_exp.x, text_size_exp.y));

					for (int n = 0, k = 0; k <= range_of_bomb, n <= range_of_bomb * 50; k++, n += 50)
						bombs[io].range_down[k].setPosition(bombs[io].x_pos, bombs[io].y_pos + n);

					for (int n = 0, k = 0; k <= range_of_bomb, n <= range_of_bomb * 50; k++, n += 50)
						bombs[io].range_down[k].setTextureRect(IntRect(text_size_exp.x*bombs[io].exp_animation_x, text_size_exp.y*bombs[io].exp_animation_y, text_size_exp.x, text_size_exp.y));


					bombs[io].exp_animation_x++;
					if (bombs[io].exp_animation_x == 3) {
						bombs[io].exp_animation_x = 0, bombs[io].exp_animation_y++;
						if (bombs[io].exp_animation_y == 3) {
							bombs[io].bomb_exploded = 0;


							bombs.erase(bombs.begin());

						}
					}
				}
			}
		}




		window.clear();
		// Drawing of the Wallpaper
		window.draw(Wallpaper);

		//Drawing of The Map
		for (int i = 2; i < width; i++)
		{
			for (int j = 2; j < length; j++)
			{
				if (Grid[i][j].getTexture() == &ground || Grid[i][j].getTexture() == &ground2)
				{
					window.draw(Grid[i][j]);
				}
			}

		}

		for (int j = 1; j <= length; j++)
		{
			for (int i = 1; i <= width; i++)
			{
				if (Grid[i][j].getTexture() == &ground || Grid[i][j].getTexture() == &ground2)
				{
					continue;
				}

				window.draw(Grid[i][j]);

				if (GetYindx(player1.getPosition().y) >= j)
				{
					window.draw(player1);
				}


				if (GetYindx(player2.getPosition().y) >= j)
				{
					window.draw(player2);
				}

				for (int io = 0; io < bombs.size(); io++)
				{
					if (bombs[io].bomb_placed && bombs[io].y_index > j)
						window.draw(bombs[io].sprt);
				}

			}
		}

		for (int io = 0; io < bombs.size(); io++)
		{

			if (bombs[io].bomb_exploded)
			{
				//Right_Explosion
				for (int g = 0; g <= range_of_bomb; g++)
				{


					if ((Grid[bombs[io].x_index + g][bombs[io].y_index].getTexture() == &breakable) && bombs[io].flag_range_right)
					{
						destroy_breakable_walls(bombs[io].x_index + g, bombs[io].y_index, Grid, ground, ground2);
						bombs[io].flag_range_right = 0;
						break;
					}
					else if (Grid[bombs[io].x_index + g][bombs[io].y_index].getTexture() == &wall)
					{

						bombs[io].flag_range_right = 0;
						break;
					}

					if (bombs[io].x_index + g == player1_xindx && bombs[io].y_index == player1_yindx && bombs[io].flag_range_right)
					{
						player1.setFillColor(Color::Transparent);
						dead.setPosition(player1.getPosition().x, player1.getPosition().y);
						player2_wins_flag = 1;
						music_clock.restart();
						ingame.stop();


					}

					if (bombs[io].x_index + g == player2_xindx && bombs[io].y_index == player2_yindx && bombs[io].flag_range_right)
					{
						player2.setFillColor(Color::Transparent);
						dead.setPosition(player2.getPosition().x, player2.getPosition().y);
						player1_wins_flag = 1;
						music_clock.restart();
						ingame.stop();
					}

					window.draw(bombs[io].range_right[g]);

				}

				//Left_Explosion
				for (int g = 0; g <= range_of_bomb; g++)
				{

					if ((Grid[bombs[io].x_index - g][bombs[io].y_index].getTexture() == &breakable) && bombs[io].flag_range_left)
					{
						destroy_breakable_walls(bombs[io].x_index - g, bombs[io].y_index, Grid, ground, ground2);

						bombs[io].flag_range_left = 0;
						break;
					}
					else if (Grid[bombs[io].x_index - g][bombs[io].y_index].getTexture() == &wall)
					{
						bombs[io].flag_range_left = 0;
						break;
					}

					if (bombs[io].x_index - g == player1_xindx && bombs[io].y_index == player1_yindx && bombs[io].flag_range_left)
					{
						player1.setFillColor(Color::Transparent);
						dead.setPosition(player1.getPosition().x, player1.getPosition().y);

						player2_wins_flag = 1;
						music_clock.restart();
						ingame.stop();

					}

					if (bombs[io].x_index - g == player2_xindx && bombs[io].y_index == player2_yindx && bombs[io].flag_range_left)
					{

						player2.setFillColor(Color::Transparent);
						dead.setPosition(player2.getPosition().x, player2.getPosition().y);

						player1_wins_flag = 1;
						music_clock.restart();
						ingame.stop();

					}
					window.draw(bombs[io].range_left[g]);



				}

				//UP_Explosion

				for (int g = 0; g <= range_of_bomb; g++)
				{


					if ((Grid[bombs[io].x_index][bombs[io].y_index - g].getTexture() == &breakable) && bombs[io].flag_range_up)
					{


						destroy_breakable_walls(bombs[io].x_index, bombs[io].y_index - g, Grid, ground, ground2);

						bombs[io].flag_range_up = 0;
						break;

					}
					else if (Grid[bombs[io].x_index][bombs[io].y_index - g].getTexture() == &wall)
					{

						bombs[io].flag_range_up = 0;
						break;
					}


					if (bombs[io].x_index == player1_xindx && bombs[io].y_index - g == player1_yindx && bombs[io].flag_range_up)
					{
						player1.setFillColor(Color::Transparent);
						dead.setPosition(player1.getPosition().x, player1.getPosition().y);
						player2_wins_flag = 1;
						music_clock.restart();
						ingame.stop();
					}

					if (bombs[io].x_index == player2_xindx && bombs[io].y_index - g == player2_yindx && bombs[io].flag_range_up)
					{
						player2.setFillColor(Color::Transparent);
						dead.setPosition(player2.getPosition().x, player2.getPosition().y);
						player1_wins_flag = 1;
						music_clock.restart();
						ingame.stop();
					}

					window.draw(bombs[io].range_up[g]);
				}


				//Down_Explosion

				for (int g = 0; g <= range_of_bomb; g++)
				{

					if ((Grid[bombs[io].x_index][bombs[io].y_index + g].getTexture() == &breakable) && bombs[io].flag_range_down)
					{
						destroy_breakable_walls(bombs[io].x_index, bombs[io].y_index + g, Grid, ground, ground2);
						bombs[io].flag_range_down = 0;
						break;
					}
					else if (Grid[bombs[io].x_index][bombs[io].y_index + g].getTexture() == &wall)
					{

						bombs[io].flag_range_down = 0;
						break;
					}
					if (bombs[io].x_index == player1_xindx && bombs[io].y_index + g == player1_yindx && bombs[io].flag_range_down)
					{
						player1.setFillColor(Color::Transparent);
						dead.setPosition(player1.getPosition().x, player1.getPosition().y);

						player2_wins_flag = 1;
						music_clock.restart();
						ingame.stop();
					}

					if (bombs[io].x_index == player2_xindx && bombs[io].y_index + g == player2_yindx && bombs[io].flag_range_down)
					{
						player2.setFillColor(Color::Transparent);
						dead.setPosition(player2.getPosition().x, player2.getPosition().y);
						player1_wins_flag = 1;
						music_clock.restart();
						ingame.stop();
					}


					window.draw(bombs[io].range_down[g]);
				}
			}
		}




		window.draw(txt);
		if (display_timer >= 25)
		{
			window.draw(zone_warning_sprt);
			window.draw(Hurt);
		}
		//window.draw(Death);

		if (death_music_duration <= 6)
		{

			if (player1_wins_flag && player2_wins_flag)
			{

				if (death_music_duration >= 3)
				{
					window.clear();
					window.draw(P_Draw);
					if (death_music_duration == 5)
					{
						exit(0);
					}
				}
				else
				{
					window.draw(dead);
				}
			}

			else if (player1_wins_flag)
			{

				if (death_music_duration >= 3)
				{
					window.clear();
					window.draw(P1_wins);
					if (death_music_duration == 4)
					{
						exit(0);
					}
				}
				else
				{
					window.draw(dead);
				}
			}




			else if (player2_wins_flag)
			{
				if (death_music_duration >= 3)
				{
					window.clear();
					window.draw(P2_wins);
					if (death_music_duration == 4)
					{
						exit(0);
					}
				}
				else
				{
					window.draw(dead);
				}
			}
		}
		//if (death_flag && death_music_duration>16) {exit(0);}
		window.display();

	}



}



void levelsmenu(Music &outgame)
{


    	Texture backgroundwall;
	backgroundwall.loadFromFile("FantasyLand.png");
	Sprite Menuwall; Menuwall.setTexture(backgroundwall);
	//Menu choices
	Sprite Choice, Choice2;
	Texture choice;
	if (!choice.loadFromFile("Untitled-1-Recovered.png"))
	{
		cout << "choice not found";
	}
	Choice.setTexture(choice);
	Choice.setPosition(0, 415);
	Choice.setScale(0.3, 0.27);

	Choice2.setTexture(choice);
	Choice2.setPosition(1024 - (0.3 * 1307), 315);
	Choice2.setScale(0.3, 0.27);
	//###############################################################################


	RenderWindow levels(VideoMode(1024, 680), "levels");
	setlevelsstatus(levels.getSize().x, levels.getSize().y);
	Texture levelwall;
	levelwall.loadFromFile("levelwall.png");
	Sprite levelwallS; levelwallS.setTexture(levelwall);
	Choice2.setPosition(1024 - (0.3 * 1307), 315); Choice.setPosition(0, 415);
	while (levels.isOpen())
	{
		// Menu choices Animation
		if (Choice.getPosition().x < 310)
		{
			Choice.move(0.5, 0);
		}
		if (Choice2.getPosition().x > 310)
		{
			Choice2.move(-0.5, 0);
		}

		Event evntlev;
		while (levels.pollEvent(evntlev))
		{
			switch (evntlev.type)
			{
			case Event::KeyReleased:
				switch (evntlev.key.code)
				{
				case Keyboard::Up:
					MoveUplev();
					break;

				case Keyboard::Down:
					MoveDownlev();
					break;

				case Keyboard::Return:
					switch (GetPressedlevel())
					{
					case 0:
					    outgame.stop();
						levels.close();
						Beginner_Mode();
						break;
					case 1:
						outgame.stop();
						levels.close();
						Advanced_Level();
						break;
					case 2:
					    outgame.stop();
						levels.close();
						Legendary_Mode();
						break;
					case 3:
					    outgame.stop();
                        levels.close();
                        mainmenu();
					}

					break;
				}

				break;
			case Event::Closed:
				return ;
				break;
			}


		}
		if ((Mouse::isButtonPressed(Mouse::Left)) && (Mouse::getPosition().x >= 615 && Mouse::getPosition().x <= 680)) {
			break;

		}

		levels.clear();
		levels.draw(levelwallS);
		levels.draw(Choice);
		levels.draw(Choice2);
		drawlev(levels);
		levels.display();

	}

}


void mainmenu ()
{

Music outgame; outgame.openFromFile("Main menu.wav");
	outgame.play();
	outgame.setLoop(1);
	//###############################################################################
	// Menu Shits
	RenderWindow background(VideoMode(1024, 680), "Bomber Man");
	setmenustatus(background.getSize().x, background.getSize().y);


	Texture backgroundwall;
	backgroundwall.loadFromFile("FantasyLand.png");
	Sprite Menuwall;
	Menuwall.setTexture(backgroundwall);
	//Menu choices
	Sprite Choice, Choice2;
	Texture choice;
	if (!choice.loadFromFile("Untitled-1-Recovered.png"))
	{
		cout << "choice not found";
	}
	Choice.setTexture(choice);
	Choice.setPosition(0, 415);
	Choice.setScale(0.3, 0.27);

	Choice2.setTexture(choice);
	Choice2.setPosition(1024 - (0.3 * 1307), 315);
	Choice2.setScale(0.3, 0.27);
	//###############################################################################
while (background.isOpen())
	{
		// Menu choices Animation
		if (Choice.getPosition().x < 310)
		{
			Choice.move(0.5, 0);
		}
		if (Choice2.getPosition().x > 310)
		{
			Choice2.move(-0.5, 0);
		}

		Event evnt;
		while (background.pollEvent(evnt))
		{
			switch (evnt.type)
			{
			case sf::Event::KeyReleased:
				switch (evnt.key.code)
				{
				case sf::Keyboard::Up:
					MoveUp();
					break;

				case sf::Keyboard::Down:
					MoveDown();
					break;

				case sf::Keyboard::Return:
					switch (GetPressedItem())
					{
					case 0:
						background.close();
                        levelsmenu(outgame);
						break;
					case 1:
						outgame.stop();
						background.close();
						Multiplayer();
						break;
					case 2:
						cout << "About Us button has been pressed \n";
					case 3:
						background.close();
						//window.close();
						return;
						break;
					}

					break;
				}

				break;
			case Event::Closed:
				background.close();
				return ;
				break;
			}
		}


		background.clear();
		background.draw(Menuwall);
		background.draw(Choice);
		background.draw(Choice2);
		drawmenu(background);
		background.display();

	}

}




int main()
{
	

	mainmenu();

}
