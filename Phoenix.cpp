/*
Special thanks to @Javidx9 OneLoneCoder.com

Controls
~~~~~~~~
Left Arrow, Right Arrow, 'Z', 'ESC'

Author
~~~~~~
Twitter: @antony_blyakher

Last Updated: 7/1/2020
*/

#include <iostream>
#include <string>
#include "ConsoleEngine.h"
#include "TextGenerator.h"
#include <stdio.h>
#include <stdlib.h>
#include<time.h>

bool leftCollision = false;
bool rightCollision = false;
bool topCollision = false;
bool bottomCollision = false;
int timer = 0;
int startScreenTimer = 0;
unsigned short gameLevel = 0;
bool gameStart = false;
bool gameComplete = false;
bool gamePause = false;
bool levelComplete = true;
unsigned int nScore = 0;
unsigned int nMoney = 0;
short enemiesCreated = 0;
int r;

std::wstring sStartScreen1 = OutputText("press 'enter' to start");
std::wstring EndScreen1 = OutputText("You Win!");
std::wstring EndScreen2 = OutputText("Press 'Enter' to play again!");
std::wstring MoneyTitle = OutputText("Money $");
std::wstring Money = OutputText("0");

struct Coordinate {
	float x;
	float y;
};

// helped me implement https://stackoverflow.com/questions/785097/how-do-i-implement-a-b%c3%a9zier-curve-in-c/11435243#11435243
// tool to draw bezier curves https://www.useragentman.com/tests/textpath/bezier-curve-construction-set.html#path=M%20126%2C%208%20C%20169%2C%2063%2C%200%2C%20120%2C%20129%2C%20178&imageURL=http://www.useragentman.com/tests/textpath/images/grid.png
struct Bezier {
	float ex1;
	float ey1;

	// two control points of bezier curve
	float cx1;
	float cy1;
	float cx2;
	float cy2;

	// 2nd end point of bezier curve
	float ex2;
	float ey2;

	//bezier incrementer
	float bi;

	// The green line cooridinates
	float xGreen1;
	float yGreen1;
	float xGreen2;
	float yGreen2;
	float xGreen3;
	float yGreen3;

	// The Blue Line coordinates
	float xBlue1;
	float yBlue1;
	float xBlue2;
	float yBlue2;

	// The final calculated coordinate on the bezier curve
	float xFinal;
	float yFinal;

	bool switchy;
	Bezier() {
		// 1st end point of bezier curve
		ex1 = 0.0f;
		ey1 = 0.0f;

		// two control points of bezier curve
		cx1 = 0.0f;
		cy1 = 0.0f;
		cx2 = 0.0f;
		cy2 = 0.0f;

		// 2nd end point of bezier curve
		ex2 = 0.0f;
		ey2 = 0.0f;

		//bezier incrementer
		bi = 0.0f;

		// The green line cooridinates
		xGreen1 = 0.0f;
		yGreen1 = 0.0f;
		xGreen2 = 0.0f;
		yGreen2 = 0.0f;
		xGreen3 = 0.0f;
		yGreen3 = 0.0f;

		// The Blue Line coordinates
		xBlue1 = 0.0f;
		yBlue1 = 0.0f;
		xBlue2 = 0.0f;
		yBlue2 = 0.0f;

		// The final calculated coordinate on the bezier curve
		xFinal = 0.0f;
		yFinal = 0.0f;

		switchy = true;
	}

	void setBezier(float a, float b, float c, float d, float e, float f, float g, float h) {
		// 1st end point of bezier curve
		ex1 = a;
		ey1 = b;

		// two control points of bezier curve
		cx1 = c;
		cy1 = d;
		cx2 = e;
		cy2 = f;

		// 2nd end point of bezier curve
		ex2 = g;
		ey2 = h;
	}
};

struct GameObject {
	short health;
	Coordinate pos;
	short width;
	short height;
};

struct Bullet {
	bool used = false;
	float x;
	float y;
	bool isEnemy;

	Bullet(float x1, float y1, bool isEnemy) {
		x = x1;
		y = y1;
		this->isEnemy = isEnemy;
	}
};

struct Explosion : public GameObject {
	short level = 1;
	int eTimer = 0;
	std::wstring sExplosionSprite = L"";
	Explosion(float x, float y) {
		pos = { x,y };
		width = 13;
	}
};

struct MoneyDrop : public GameObject {
	int eTimer = 0;
	bool used = false;
	//std::wstring sMoneySprite = L"";
	std::wstring sMoneySprite;
	MoneyDrop(float x, float y) {
		pos = { x,y };
		width = 16;
		height = 5;
		sMoneySprite += L".o..............";
		sMoneySprite += L"ooo..o.ooo.ooo..";
		sMoneySprite += L"o...oo.o.o.o.o..";
		sMoneySprite += L"ooo..o.o.o.o.o..";
		sMoneySprite += L"..o..o.o.o.o.o..";
		sMoneySprite += L"ooo..o.ooo.ooo..";
		sMoneySprite += L".o..............";
	}
};

std::vector<std::shared_ptr<Explosion>> explosions;
std::vector<std::shared_ptr<MoneyDrop>> moneyDrops;

struct Player : public GameObject {
	short level = 1;
	std::wstring sPlayerSprite;
	Player() {
		pos = { 120.0f, 200.0f };
		health = 5;
		width = 13;
		height = 14;

		sPlayerSprite += L"......o......";
		sPlayerSprite += L"......o......";
		sPlayerSprite += L".....ooo.....";
		sPlayerSprite += L".....ooo.....";
		sPlayerSprite += L".....ooo.....";
		sPlayerSprite += L"...o.ooo.o...";
		sPlayerSprite += L"...o.ooo.o...";
		sPlayerSprite += L"...o.ooo.o...";
		sPlayerSprite += L"o..ooooooo..o";
		sPlayerSprite += L"o..ooooooo..o";
		sPlayerSprite += L"o.ooooooooo.o";
		sPlayerSprite += L"ooooooooooooo";
		sPlayerSprite += L"oo...ooo...oo";
		sPlayerSprite += L"o.....o.....o";
	}
};

struct LowEnemy : public GameObject {
	std::wstring sLEnemySprite;
	bool initial = true;
	short group = 0;
	bool hurt = false;
	short hurtTimer = 0;
	short animationNum = 0;
	short pixelsMoved = 0;
	Bezier* bezier;
	LowEnemy(float x, float y) : bezier() {
		pos = { x,y };
		health = 3;
		width = 13;
		height = 11;
		sLEnemySprite += L"oo...o.o...oo";
		sLEnemySprite += L".oo.oo.oo.oo.";
		sLEnemySprite += L".ooooooooooo.";
		sLEnemySprite += L"..ooooooooo..";
		sLEnemySprite += L"..ooooooooo..";
		sLEnemySprite += L"...o.ooo.o...";
		sLEnemySprite += L"...o..o..o...";
		sLEnemySprite += L"...oo...oo...";
		sLEnemySprite += L"....o...o....";
		sLEnemySprite += L"....o...o....";
		sLEnemySprite += L"....o...o....";
		bezier = new Bezier();
	}

	~LowEnemy() {
		explosions.push_back(std::unique_ptr<Explosion>(new Explosion(this->pos.x, this->pos.y)));
		r = (rand() % 100);
		if (r < 25) {
			moneyDrops.push_back(std::unique_ptr<MoneyDrop>(new MoneyDrop(this->pos.x, this->pos.y)));
		}

		delete this->bezier;
		bezier = nullptr;
		nScore += 100;
	}
};

struct MedEnemy : public GameObject {
	std::wstring sMEnemySprite;
	bool hurt = false;
	short hurtTimer = 0;
	MedEnemy(float x, float y) {
		pos = { x,y };
		health = 5;
		width = 17;
		height = 22;
		sMEnemySprite += L".......ooo.......";
		sMEnemySprite += L"..ooo..ooo..ooo..";
		sMEnemySprite += L"..ooo..ooo..ooo..";
		sMEnemySprite += L".ooooooooooooooo.";
		sMEnemySprite += L"ooooooooooooooooo";
		sMEnemySprite += L"ooooooooooooooooo";
		sMEnemySprite += L"ooooooooooooooooo";
		sMEnemySprite += L"ooooooooooooooooo";
		sMEnemySprite += L"ooooooooooooooooo";
		sMEnemySprite += L"ooooooooooooooooo";
		sMEnemySprite += L"ooooooooooooooooo";
		sMEnemySprite += L"ooo..ooooooo..ooo";
		sMEnemySprite += L"ooo..ooooooo..ooo";
		sMEnemySprite += L".o...ooooooo...o.";
		sMEnemySprite += L".o....ooooo....o.";
		sMEnemySprite += L"......ooooo......";
		sMEnemySprite += L"......ooooo......";
		sMEnemySprite += L".......ooo.......";
		sMEnemySprite += L".......ooo.......";
		sMEnemySprite += L".......ooo.......";
		sMEnemySprite += L"........o........";
		sMEnemySprite += L"........o........";
	}
	~MedEnemy() {
		explosions.push_back(std::unique_ptr<Explosion>(new Explosion(this->pos.x, this->pos.y)));
		nScore += 500;
	}
};

struct HighEnemy : public GameObject {
	std::wstring sHEnemySprite;
	bool hurt = false;
	short hurtTimer = 0;
	HighEnemy(float x, float y) {
		pos = { x,y };
		health = 5;
		width = 17;
		height = 22;
		sHEnemySprite += L".......ooo.......";
		sHEnemySprite += L"..ooo..ooo..ooo..";
		sHEnemySprite += L"..ooo..ooo..ooo..";
		sHEnemySprite += L".ooooooooooooooo.";
		sHEnemySprite += L"ooooooooooooooooo";
		sHEnemySprite += L"ooooooooooooooooo";
		sHEnemySprite += L"ooooooooooooooooo";
		sHEnemySprite += L"ooooooooooooooooo";
		sHEnemySprite += L"ooooooooooooooooo";
		sHEnemySprite += L"ooooooooooooooooo";
		sHEnemySprite += L"ooooooooooooooooo";
		sHEnemySprite += L"ooo..ooooooo..ooo";
		sHEnemySprite += L"ooo..ooooooo..ooo";
		sHEnemySprite += L".o...ooooooo...o.";
		sHEnemySprite += L".o....ooooo....o.";
		sHEnemySprite += L"......ooooo......";
		sHEnemySprite += L"......ooooo......";
		sHEnemySprite += L".......ooo.......";
		sHEnemySprite += L".......ooo.......";
		sHEnemySprite += L".......ooo.......";
		sHEnemySprite += L"........o........";
		sHEnemySprite += L"........o........";
	}
	~HighEnemy() {
		explosions.push_back(std::unique_ptr<Explosion>(new Explosion(this->pos.x, this->pos.y)));
		nScore += 1000;
	}
};

int bezierPoint(float n1, float n2, float perc)
{
	float diff = n2 - n1;

	return n1 + (diff * perc);
}

// initializing vectors and player object

std::vector<std::shared_ptr<LowEnemy>> lowEnemies;
std::vector<std::shared_ptr<MedEnemy>> medEnemies;
std::vector<std::shared_ptr<HighEnemy>> highEnemies;
std::vector<Bullet> bullets;
std::vector<Bullet> enemyBullets;
Player player;

class Phoenix : public ConsoleEngine
{
public:
	Phoenix()
	{
		m_sAppName = L"Phoenix";
		m_sAppVersion = L"v0.1.5";
	}

public:

	void updateLevel()
	{
		switch (gameLevel)
		{
		case 1: {
			if (enemiesCreated < 12) {
				lowEnemies.push_back(std::unique_ptr<LowEnemy>(new LowEnemy(0, 0)));
				for (auto& enemy : lowEnemies)
				{
					if (enemy->initial) {
						enemy->bezier->setBezier(96.0f, 0.0f, 62.0f, 64.0f, 237.0f, 106.0f, 130.0f, 143.0f);
						enemy->group = 1;
						enemy->initial = false;
					}
				}
				lowEnemies.push_back(std::unique_ptr<LowEnemy>(new LowEnemy(0, 0)));
				for (auto& enemy : lowEnemies)
				{
					if (enemy->initial) {
						enemy->bezier->setBezier(126.0f, -20.0f, 160.0f, 64.0f, -15.0f, 106.0f, 130.0f, 143.0f);
						enemy->group = 2;
						enemy->initial = false;
					}
				}

				enemiesCreated += 2;
			}

			break;
		}
		case 2:
			if (lowEnemies.size() == 0)
				timer = 1;

			if (enemiesCreated < 24) {
				lowEnemies.push_back(std::unique_ptr<LowEnemy>(new LowEnemy(0, 0)));
				for (auto& enemy : lowEnemies)
				{
					if (enemy->initial) {
						enemy->bezier->setBezier(96.0f, 0.0f, 62.0f, 64.0f, 237.0f, 106.0f, 130.0f, 143.0f);
						enemy->group = 1;
						enemy->initial = false;
					}
				}
				lowEnemies.push_back(std::unique_ptr<LowEnemy>(new LowEnemy(0, 0)));
				for (auto& enemy : lowEnemies)
				{
					if (enemy->initial) {
						enemy->bezier->setBezier(126.0f, -20.0f, 160.0f, 64.0f, -15.0f, 106.0f, 130.0f, 143.0f);
						enemy->group = 2;
						enemy->initial = false;
					}
				}
				enemiesCreated += 2;
			}
			break;

		case 3:
			if (lowEnemies.size() == 0)
				timer = 1;

			if (enemiesCreated < 36) {
				lowEnemies.push_back(std::unique_ptr<LowEnemy>(new LowEnemy(0, 0)));
				for (auto& enemy : lowEnemies)
				{
					if (enemy->initial) {
						enemy->bezier->setBezier(96.0f, 0.0f, 62.0f, 64.0f, 237.0f, 106.0f, 130.0f, 143.0f);
						enemy->group = 1;
						enemy->initial = false;
					}
				}
				lowEnemies.push_back(std::unique_ptr<LowEnemy>(new LowEnemy(0, 0)));
				for (auto& enemy : lowEnemies)
				{
					if (enemy->initial) {
						enemy->bezier->setBezier(126.0f, -20.0f, 160.0f, 64.0f, -15.0f, 106.0f, 130.0f, 143.0f);
						enemy->group = 2;
						enemy->initial = false;
					}
				}
				enemiesCreated += 2;
			}
			break;
		}
	}

	// Called by ConsoleEngine, creates and loads all game resources and data
	virtual bool OnUserCreate()

	{
		srand((int)time(0));
		return true;
	}

	// Called by ConsoleEngine, updates the console display every frame
	virtual bool OnUserUpdate(float fElapsedTime)
	{
		// Start screen
		if (!gameStart) {
			startScreenTimer++;
			if (m_keys[VK_RETURN].bPressed) {
				gameStart = true;
			}
			// First we clear the screen

			// Then draw Title and Border at the top

			if (startScreenTimer > 0 && startScreenTimer <= 1) {
				Fill(0, 0, ScreenWidth(), ScreenHeight(), L' ', 0);
				for (unsigned short i = 0; i < sStartScreen1.size(); i++) {
					if (sStartScreen1[i] == 'o') {
						Draw(i % (sStartScreen1.size() / 9) + 50, floor(i / (sStartScreen1.size() / 9) + ScreenHeight() / 2));
					}
				}
			}

			else if (startScreenTimer > 15 && startScreenTimer <= 16) {
				Fill(0, 0, ScreenWidth(), ScreenHeight(), L' ', 0);
				for (unsigned short i = 0; i < sStartScreen1.size(); i++) {
					if (sStartScreen1[i] == 'o') {
						Draw(i % (sStartScreen1.size() / 9) + 50, floor(i / (sStartScreen1.size() / 9) + ScreenHeight() / 2), PIXEL_HALF);
					}
				}
			}

			if (startScreenTimer == 30)
				startScreenTimer = 0;
		}
		else if (gameComplete) {
			startScreenTimer++;
			if (m_keys[VK_RETURN].bPressed) {
				// reset all objects and variables before reset
				gameComplete = false;
				gameLevel = 0;
				timer = 0;
				levelComplete = true;
				nScore = 0;
				player.level = 1;
				player.pos.x = 120;
				player.pos.y = 200;
				player.health = 3;
				enemiesCreated = 0;
				bullets.erase(
					std::remove_if(
						bullets.begin(),
						bullets.end(),
						[](auto& bullet) { return (bullet.y >= -1); }
					),
					bullets.end()
				);
			}
			// ==== > DISPLAY < ====================================================

			// First we clear the screen

			// Then draw Title and Border at the top

			if (startScreenTimer > 0 && startScreenTimer <= 2) {
				Fill(0, 0, ScreenWidth(), ScreenHeight(), L' ', 0);
				for (unsigned short i = 0; i < EndScreen1.size(); i++) {
					if (EndScreen1[i] == 'o') {
						Draw(i % (EndScreen1.size() / 9) + 100, (floor(i / (EndScreen1.size() / 9) + ScreenHeight() / 2)) - 40, PIXEL_SOLID, FG_GREEN);
					}
				}
				for (unsigned short i = 0; i < EndScreen2.size(); i++) {
					if (EndScreen2[i] == 'o') {
						Draw(i % (EndScreen2.size() / 9) + 28, (floor(i / (EndScreen2.size() / 9) + ScreenHeight() / 2)), PIXEL_SOLID, FG_GREEN);
					}
				}
			}

			else if (startScreenTimer > 15 && startScreenTimer <= 17) {
				Fill(0, 0, ScreenWidth(), ScreenHeight(), L' ', 0);
				for (unsigned short i = 0; i < EndScreen1.size(); i++) {
					if (EndScreen1[i] == 'o') {
						Draw(i % (EndScreen1.size() / 9) + 100, (floor(i / (EndScreen1.size() / 9) + ScreenHeight() / 2)) - 40, PIXEL_HALF, FG_GREEN);
					}
				}
				for (unsigned short i = 0; i < EndScreen2.size(); i++) {
					if (EndScreen2[i] == 'o') {
						Draw(i % (EndScreen2.size() / 9) + 28, (floor(i / (EndScreen2.size() / 9) + ScreenHeight() / 2)), PIXEL_HALF, FG_GREEN);
					}
				}
			}

			if (startScreenTimer == 30)
				startScreenTimer = 0;
		}
		// Game started
		else {
			// ==== > LOGIC < ======================================================
			timer++;

			if (lowEnemies.size() == 0 && medEnemies.size() == 0 && highEnemies.size() == 0) {
				levelComplete = true;
			}

			//updates current level
			if (levelComplete == true) {
				gameLevel++;
				updateLevel();
				levelComplete = false;
			}

			if ((int)(timer) % 20 == 0) {
				updateLevel();
			}

			// player collisions
			if ((int)(player.pos.x) <= 0)
				leftCollision = true;
			else {
				leftCollision = false;
			}
			if ((int)(player.pos.x) >= ScreenWidth() - player.width - 1)
				rightCollision = true;
			else {
				rightCollision = false;
			}

			//player movement
			if (m_keys[VK_LEFT].bHeld && !leftCollision)
			{
				player.pos.x -= 100 * fElapsedTime;
			}

			if (m_keys[VK_RIGHT].bHeld && !rightCollision)
			{
				player.pos.x += 100 * fElapsedTime;
			}

			// laser shot
			//if (timer % 10 == 0) {
			if (m_keys[0x5A].bPressed)
			{
				Bullet bullet = { player.pos.x,player.pos.y,false };
				bullets.push_back(bullet);
			}
			//}
			// bullet damage to enemy
			if (player.level == 1) {
				for (auto& bullet : bullets) // access by reference to avoid copying
				{
					if (bullet.x != -1 && bullet.y != -1) {
						bullet.y -= 300 * fElapsedTime; // continue to shift bullet up one pixel to the top of screen
						for (auto& enemy : lowEnemies) // access by reference to avoid copying
						{
							if (bullet.x >= enemy->pos.x - (ceil(enemy->width / 2) + 1) && bullet.x <= enemy->pos.x + (ceil(enemy->width / 2) + 3) && bullet.y >= enemy->pos.y - enemy->height && bullet.y <= enemy->pos.y + enemy->height) {
								enemy->health -= 1;
								enemy->hurt = true;
								bullet.used = true;
							}
						}
						for (auto& enemy : medEnemies) // access by reference to avoid copying
						{
							if (bullet.x >= enemy->pos.x - (ceil(enemy->width / 2)) && bullet.x <= enemy->pos.x + (ceil(enemy->width / 2) + 5) && bullet.y >= enemy->pos.y - enemy->height && bullet.y <= enemy->pos.y + enemy->height) {
								enemy->health -= 1;
								enemy->hurt = true;
								bullet.used = true;
							}
						}
						for (auto& enemy : highEnemies) // access by reference to avoid copying
						{
							if (bullet.x >= enemy->pos.x - (ceil(enemy->width / 2)) && bullet.x <= enemy->pos.x + (ceil(enemy->width / 2) + 5) && bullet.y >= enemy->pos.y - enemy->height && bullet.y <= enemy->pos.y + enemy->height) {
								enemy->health -= 1;
								enemy->hurt = true;
								bullet.used = true;
							}
						}
					}
				}
			}

			for (auto& bullet : enemyBullets) // access by reference to avoid copying
			{
				bullet.y += 100 * fElapsedTime; // continue to shift bullet up one pixel to the top of screen

				if (bullet.x >= player.pos.x - (ceil(player.width / 2) + 2) && bullet.x <= player.pos.x + (ceil(player.width / 2) + 1) && bullet.y >= player.pos.y - (ceil(player.height / 2) - 3) && bullet.y <= player.pos.y + (ceil(player.height))) {
					player.health -= 1;
					//player.hurt = true;

					bullet.used = true;
				}
			}

			float test2 = player.pos.x - (ceil(player.width / 2) + 2);
			float test3 = player.pos.x + (ceil(player.width / 2) + 1);
			float test5 = player.pos.x + (ceil(player.width / 2) + 1);
			for (auto& moneyDrop : moneyDrops) // access by reference to avoid copying
			{
				moneyDrop->pos.y += 50 * fElapsedTime; // continue to shift bullet up one pixel to the top of screen
				if (moneyDrop->pos.x + (moneyDrop->width - 2) >= player.pos.x && moneyDrop->pos.x - (moneyDrop->width - 2) <= player.pos.x && moneyDrop->pos.y >= player.pos.y - (ceil(player.height / 2) - 3) && moneyDrop->pos.y <= player.pos.y + (ceil(player.height))) {
					nMoney += 100;
					float test7 = moneyDrop->pos.x;
					moneyDrop->used = true;
				}
			}

			// enemy movement
			for (auto& enemy : lowEnemies)

			{
				if (enemy->animationNum == 0) {
					enemy->bezier->xGreen1 = bezierPoint(enemy->bezier->ex1, enemy->bezier->cx1, enemy->bezier->bi);
					enemy->bezier->yGreen1 = bezierPoint(enemy->bezier->ey1, enemy->bezier->cy1, enemy->bezier->bi);
					enemy->bezier->xGreen2 = bezierPoint(enemy->bezier->cx1, enemy->bezier->cx2, enemy->bezier->bi);
					enemy->bezier->yGreen2 = bezierPoint(enemy->bezier->cy1, enemy->bezier->cy2, enemy->bezier->bi);
					enemy->bezier->xGreen3 = bezierPoint(enemy->bezier->cx2, enemy->bezier->ex2, enemy->bezier->bi);
					enemy->bezier->yGreen3 = bezierPoint(enemy->bezier->cy2, enemy->bezier->ey2, enemy->bezier->bi);

					// The Blue Line
					enemy->bezier->xBlue1 = bezierPoint(enemy->bezier->xGreen1, enemy->bezier->xGreen2, enemy->bezier->bi);
					enemy->bezier->yBlue1 = bezierPoint(enemy->bezier->yGreen1, enemy->bezier->yGreen2, enemy->bezier->bi);
					enemy->bezier->xBlue2 = bezierPoint(enemy->bezier->xGreen2, enemy->bezier->xGreen3, enemy->bezier->bi);
					enemy->bezier->yBlue2 = bezierPoint(enemy->bezier->yGreen2, enemy->bezier->yGreen3, enemy->bezier->bi);

					// The Black Dot
					enemy->pos.x = bezierPoint(enemy->bezier->xBlue1, enemy->bezier->xBlue2, enemy->bezier->bi);
					enemy->pos.y = bezierPoint(enemy->bezier->yBlue1, enemy->bezier->yBlue2, enemy->bezier->bi);

					enemy->bezier->bi += 0.02;

					if (enemy->pos.x <= 132 && enemy->pos.y >= 143) {
						enemy->animationNum = 1;
						enemy->bezier->setBezier(132.0f, 143.0f, 9.0f, 178.0f, 150.0f, 82.0f, 142.0f, 34.0f);
						enemy->bezier->bi = 0;
					}
					else if (enemy->pos.x >= 132 && enemy->pos.y >= 143) {
						enemy->animationNum = 1;
						enemy->bezier->setBezier(132.0f, 143.0f, 240.0f, 178.0f, 130.0f, 82.0f, 162.0f, 34.0f);
						enemy->bezier->bi = 0;
					}
				}
				else if (enemy->animationNum == 1) {
					enemy->bezier->xGreen1 = bezierPoint(enemy->bezier->ex1, enemy->bezier->cx1, enemy->bezier->bi);
					enemy->bezier->yGreen1 = bezierPoint(enemy->bezier->ey1, enemy->bezier->cy1, enemy->bezier->bi);
					enemy->bezier->xGreen2 = bezierPoint(enemy->bezier->cx1, enemy->bezier->cx2, enemy->bezier->bi);
					enemy->bezier->yGreen2 = bezierPoint(enemy->bezier->cy1, enemy->bezier->cy2, enemy->bezier->bi);
					enemy->bezier->xGreen3 = bezierPoint(enemy->bezier->cx2, enemy->bezier->ex2, enemy->bezier->bi);
					enemy->bezier->yGreen3 = bezierPoint(enemy->bezier->cy2, enemy->bezier->ey2, enemy->bezier->bi);

					// The Blue Line
					enemy->bezier->xBlue1 = bezierPoint(enemy->bezier->xGreen1, enemy->bezier->xGreen2, enemy->bezier->bi);
					enemy->bezier->yBlue1 = bezierPoint(enemy->bezier->yGreen1, enemy->bezier->yGreen2, enemy->bezier->bi);
					enemy->bezier->xBlue2 = bezierPoint(enemy->bezier->xGreen2, enemy->bezier->xGreen3, enemy->bezier->bi);
					enemy->bezier->yBlue2 = bezierPoint(enemy->bezier->yGreen2, enemy->bezier->yGreen3, enemy->bezier->bi);

					// The Black Dot
					enemy->pos.x = bezierPoint(enemy->bezier->xBlue1, enemy->bezier->xBlue2, enemy->bezier->bi);
					enemy->pos.y = bezierPoint(enemy->bezier->yBlue1, enemy->bezier->yBlue2, enemy->bezier->bi);

					enemy->bezier->bi += 0.02;

					if (enemy->pos.y < 20 && enemy->group == 1) {
						enemy->animationNum = 2;
					}
					else if (enemy->pos.y < 40 && enemy->group == 2) {
						enemy->animationNum = 2;
					}
				}

				else if (enemy->animationNum == 2) {
					enemy->pos.x -= 1;
					if (enemy->pos.x <= 20) {
						for (auto& enemy : lowEnemies)
						{
							enemy->animationNum = 3;
						}
					}
				}

				else if (enemy->animationNum == 3) {
					enemy->pos.x += 1;
					enemy->pixelsMoved++;
					if (enemy->pixelsMoved == 85) {
						enemy->pixelsMoved = 0;
						enemy->animationNum = 4;
					}
				}

				else if (enemy->animationNum == 4) {
					enemy->pos.x -= 1;
					enemy->pixelsMoved++;
					if (enemy->pixelsMoved == 85) {
						enemy->pixelsMoved = 0;
						enemy->animationNum = 3;
					}
				}
			}

			// enemy bullet shot
			for (auto& enemy : lowEnemies) {
				r = (rand() % 1000);
				if (r <= 5) {
					Bullet bullet = { enemy->pos.x,enemy->pos.y + enemy->height,true };
					enemyBullets.push_back(bullet);
				}
			}

			for (auto& explosion : explosions) // access by reference to avoid copying
			{
				explosion->eTimer++;
				if (explosion->eTimer < 5) {
					explosion->sExplosionSprite = L"";
					explosion->sExplosionSprite += L"......o..o..o";
					explosion->sExplosionSprite += L".o.oo..o...o.";
					explosion->sExplosionSprite += L"..oo..o...o..";
					explosion->sExplosionSprite += L"...o.....o.o.";
					explosion->sExplosionSprite += L".oo....o..o..";
					explosion->sExplosionSprite += L"..oo.o.....o.";
					explosion->sExplosionSprite += L"o.o.o...o.o.o";
					explosion->sExplosionSprite += L"...o..o...o..";
					explosion->sExplosionSprite += L"....o.o..o...";
					explosion->sExplosionSprite += L"..o.o....oo..";
					explosion->sExplosionSprite += L"oo.o.oo.oo.o.";
					explosion->sExplosionSprite += L".......o..ooo";
				}
				else if (explosion->eTimer < 10) {
					explosion->sExplosionSprite = L"";
					explosion->sExplosionSprite += L"o.oo..o......";
					explosion->sExplosionSprite += L"............o";
					explosion->sExplosionSprite += L".oo...o...o..";
					explosion->sExplosionSprite += L"o........o.o.";
					explosion->sExplosionSprite += L"..o.......o..";
					explosion->sExplosionSprite += L"...o.......o.";
					explosion->sExplosionSprite += L"oo........o.o";
					explosion->sExplosionSprite += L"o.........o..";
					explosion->sExplosionSprite += L".............";
					explosion->sExplosionSprite += L".o...........";
					explosion->sExplosionSprite += L"...o...o....o";
					explosion->sExplosionSprite += L"o....o.o...o.";
				}
				else if (explosion->eTimer < 15) {
					explosion->sExplosionSprite = L"";
					explosion->sExplosionSprite += L".............";
					explosion->sExplosionSprite += L".o........o..";
					explosion->sExplosionSprite += L"......o...o..";
					explosion->sExplosionSprite += L"....oo.....o.";
					explosion->sExplosionSprite += L"..o..o..o.o..";
					explosion->sExplosionSprite += L"...o.o...o...";
					explosion->sExplosionSprite += L"....o.....o..";
					explosion->sExplosionSprite += L"......oo..o..";
					explosion->sExplosionSprite += L"..o..........";
					explosion->sExplosionSprite += L".............";
					explosion->sExplosionSprite += L"......o......";
					explosion->sExplosionSprite += L".............";
				}
				else if (explosion->eTimer < 20) {
					explosion->sExplosionSprite = L"";
					explosion->sExplosionSprite += L".............";
					explosion->sExplosionSprite += L".............";
					explosion->sExplosionSprite += L".............";
					explosion->sExplosionSprite += L".....o.o.....";
					explosion->sExplosionSprite += L"..o..o..o.o..";
					explosion->sExplosionSprite += L".....ooo.o...";
					explosion->sExplosionSprite += L"....o..oo....";
					explosion->sExplosionSprite += L"......o......";
					explosion->sExplosionSprite += L".............";
					explosion->sExplosionSprite += L".............";
					explosion->sExplosionSprite += L".............";
					explosion->sExplosionSprite += L".............";
				}
				else if (explosion->eTimer < 25) {
					explosion->sExplosionSprite = L"";
					explosion->sExplosionSprite += L".............";
					explosion->sExplosionSprite += L".............";
					explosion->sExplosionSprite += L".............";
					explosion->sExplosionSprite += L".....o.o.....";
					explosion->sExplosionSprite += L"........o....";
					explosion->sExplosionSprite += L".....o..oo...";
					explosion->sExplosionSprite += L".......o.....";
					explosion->sExplosionSprite += L".............";
					explosion->sExplosionSprite += L".............";
					explosion->sExplosionSprite += L".............";
					explosion->sExplosionSprite += L".............";
					explosion->sExplosionSprite += L".............";
				}
				else {
					explosion->sExplosionSprite = L"";
				}
			}

			for (auto& enemy : lowEnemies) // access by reference to avoid copying
			{
				if (enemy->hurt)
					enemy->hurtTimer++;
				if (enemy->hurtTimer == 10) {
					enemy->hurt = false;
					enemy->hurtTimer = 0;
				}
			}

			for (auto& enemy : medEnemies) // access by reference to avoid copying
			{
				if (enemy->hurt)
					enemy->hurtTimer++;
				if (enemy->hurtTimer == 10) {
					enemy->hurt = false;
					enemy->hurtTimer = 0;
				}
			}

			for (auto& enemy : highEnemies) // access by reference to avoid copying
			{
				if (enemy->hurt)
					enemy->hurtTimer++;
				if (enemy->hurtTimer == 10) {
					enemy->hurt = false;
					enemy->hurtTimer = 0;
				}
			}

			//bullet objects deleted if reaches end of screen
			bullets.erase(
				std::remove_if(
					bullets.begin(),
					bullets.end(),
					[](auto& bullet) { return bullet.y == -1; }
				),
				bullets.end()
			);

			bullets.erase(
				std::remove_if(
					bullets.begin(),
					bullets.end(),
					[](auto& bullet) { return bullet.used == true; }
				),
				bullets.end()
			);

			enemyBullets.erase(
				std::remove_if(
					enemyBullets.begin(),
					enemyBullets.end(),
					[](auto& enemyBullet) { return enemyBullet.y > 225; }
				),
				enemyBullets.end()
			);

			enemyBullets.erase(
				std::remove_if(
					enemyBullets.begin(),
					enemyBullets.end(),
					[](auto& enemyBullet) { return enemyBullet.used == true; }
				),
				enemyBullets.end()
			);

			// enemy objects deleted if health becomes 0
			lowEnemies.erase(
				std::remove_if(
					lowEnemies.begin(),
					lowEnemies.end(),
					[](auto& enemy) {
						return enemy->health == 0;
					}
				),
				lowEnemies.end()
						);
			medEnemies.erase(
				std::remove_if(
					medEnemies.begin(),
					medEnemies.end(),
					[](auto& enemy) {
						return enemy->health == 0;
					}
				),
				medEnemies.end()
						);
			highEnemies.erase(
				std::remove_if(
					highEnemies.begin(),
					highEnemies.end(),
					[](auto& enemy) {
						return enemy->health == 0;
					}
				),
				highEnemies.end()
						);

			explosions.erase(
				std::remove_if(
					explosions.begin(),
					explosions.end(),
					[](auto& explosion) {
						return explosion->eTimer > 30;
					}
				),
				explosions.end()
						);

			moneyDrops.erase(
				std::remove_if(
					moneyDrops.begin(),
					moneyDrops.end(),
					[](auto& moneyDrop) {
						return moneyDrop->pos.y > 225;
					}
				),
				moneyDrops.end()
						);

			moneyDrops.erase(
				std::remove_if(
					moneyDrops.begin(),
					moneyDrops.end(),
					[](auto& moneyDrop) {
						return moneyDrop->used == true;
					}
				),
				moneyDrops.end()
						);

			if (player.health == 0) {
				gameComplete = true;
			}

			if ((int)(timer) == 150) {
				timer = 0;
			}

			// ==== DISPLAY ========================================================

			// Erase All
			Fill(0, 0, ScreenWidth(), ScreenHeight(), L' ', 0);

			// draw bullets
			for (auto& bullet : bullets) // access by reference to avoid copying
			{
				if (bullet.y != -1 && bullet.y > 2) {
					if (player.level == 1) {
						Draw((int)(bullet.x) + 6, (int)(bullet.y), PIXEL_SOLID);
						Draw((int)(bullet.x) + 6, (int)(bullet.y + 1), PIXEL_SOLID);
						Draw((int)(bullet.x) + 6, (int)(bullet.y + 2), PIXEL_SOLID);
						Draw((int)(bullet.x) + 6, (int)(bullet.y + 3), PIXEL_SOLID);
						Draw((int)(bullet.x) + 6, (int)(bullet.y + 4), PIXEL_SOLID);
						Draw((int)(bullet.x) + 6, (int)(bullet.y + 5), PIXEL_SOLID);

						Draw((int)(bullet.x - 1) + 6, (int)(bullet.y), PIXEL_SOLID);
						Draw((int)(bullet.x - 1) + 6, (int)(bullet.y + 1), PIXEL_SOLID);
						Draw((int)(bullet.x - 1) + 6, (int)(bullet.y + 2), PIXEL_SOLID);
						Draw((int)(bullet.x - 1) + 6, (int)(bullet.y + 3), PIXEL_SOLID);
						Draw((int)(bullet.x - 1) + 6, (int)(bullet.y + 4), PIXEL_SOLID);
						Draw((int)(bullet.x - 1) + 6, (int)(bullet.y + 5), PIXEL_SOLID);
						Draw((int)(bullet.x + 1) + 6, (int)(bullet.y), PIXEL_SOLID);
						Draw((int)(bullet.x + 1) + 6, (int)(bullet.y + 1), PIXEL_SOLID);
						Draw((int)(bullet.x + 1) + 6, (int)(bullet.y + 2), PIXEL_SOLID);
						Draw((int)(bullet.x + 1) + 6, (int)(bullet.y + 3), PIXEL_SOLID);
						Draw((int)(bullet.x + 1) + 6, (int)(bullet.y + 4), PIXEL_SOLID);
						Draw((int)(bullet.x + 1) + 6, (int)(bullet.y + 5), PIXEL_SOLID);
					}
				}
			}

			for (auto& bullet : enemyBullets) // access by reference to avoid copying
			{
				if (bullet.y != -1 && bullet.y > 2) {
					Draw((int)(bullet.x) + 6, (int)(bullet.y - 1), PIXEL_HALF);
					Draw((int)(bullet.x) + 6, (int)(bullet.y), PIXEL_SOLID);
					Draw((int)(bullet.x) + 6, (int)(bullet.y + 1), PIXEL_SOLID);
					Draw((int)(bullet.x) + 6, (int)(bullet.y + 2), PIXEL_SOLID);
					Draw((int)(bullet.x) + 6, (int)(bullet.y + 3), PIXEL_HALF);
					//Draw((int)(bullet.x) + 6, (int)(bullet.y + 4), PIXEL_HALF);
					//Draw((int)(bullet.x) + 6, (int)(bullet.y + 5), PIXEL_HALF);

					Draw((int)(bullet.x - 1) + 6, (int)(bullet.y - 1), PIXEL_HALF);
					Draw((int)(bullet.x + 1) + 6, (int)(bullet.y - 1), PIXEL_HALF);

					Draw((int)(bullet.x - 1) + 6, (int)(bullet.y), PIXEL_HALF);
					Draw((int)(bullet.x - 1) + 6, (int)(bullet.y + 1), PIXEL_HALF);
					Draw((int)(bullet.x - 1) + 6, (int)(bullet.y + 2), PIXEL_HALF);
					Draw((int)(bullet.x - 1) + 6, (int)(bullet.y + 3), PIXEL_HALF);

					Draw((int)(bullet.x + 1) + 6, (int)(bullet.y), PIXEL_HALF);
					Draw((int)(bullet.x + 1) + 6, (int)(bullet.y + 1), PIXEL_HALF);
					Draw((int)(bullet.x + 1) + 6, (int)(bullet.y + 2), PIXEL_HALF);
					Draw((int)(bullet.x + 1) + 6, (int)(bullet.y + 3), PIXEL_HALF);
				}
			}

			for (auto& explosion : explosions) // access by reference to avoid copying
			{
				for (unsigned short i = 0; i < explosion->sExplosionSprite.size(); i++) {
					if (explosion->sExplosionSprite[i] == 'o') {
						Draw(i % explosion->width + (int)(explosion->pos.x), floor(i / explosion->width) + (int)(explosion->pos.y));
					}
				}
			}

			for (auto& moneyDrop : moneyDrops) // access by reference to avoid copying
			{
				for (unsigned short i = 0; i < moneyDrop->sMoneySprite.size(); i++) {
					if (moneyDrop->sMoneySprite[i] == 'o') {
						Draw(i % moneyDrop->width + (int)(moneyDrop->pos.x), floor(i / moneyDrop->width) + (int)(moneyDrop->pos.y));
					}
				}
			}

			for (unsigned short i = 0; i < player.sPlayerSprite.size(); i++) {
				if (player.sPlayerSprite[i] == 'o') {
					Draw(i % 13 + (int)(player.pos.x), floor(i / 13) + (int)(player.pos.y));
				}
			}

			for (auto& enemy : lowEnemies) // access by reference to avoid copying
			{
				if (enemy->pos.x != 0.0f) {
					for (unsigned short i = 0; i < enemy->sLEnemySprite.size(); i++) {
						if (enemy->sLEnemySprite[i] == 'o') {
							if (enemy->hurt == false)
								Draw(i % enemy->width + (enemy->pos.x), floor(i / enemy->width) + (enemy->pos.y));
							if (enemy->hurt)
								Draw(i % enemy->width + (enemy->pos.x), floor(i / enemy->width) + (enemy->pos.y), PIXEL_HALF);
						}
					}
				}
			}

			for (auto& enemy : medEnemies) // access by reference to avoid copying
			{
				for (unsigned short i = 0; i < enemy->sMEnemySprite.size(); i++) {
					if (enemy->sMEnemySprite[i] == 'o') {
						if (enemy->hurt == false)
							Draw(i % enemy->width + (int)(enemy->pos.x), floor(i / enemy->width) + (int)(enemy->pos.y));
						if (enemy->hurt)
							Draw(i % enemy->width + (int)(enemy->pos.x), floor(i / enemy->width) + (int)(enemy->pos.y), PIXEL_HALF);
					}
				}
			}

			for (auto& enemy : highEnemies) // access by reference to avoid copying
			{
				for (unsigned short i = 0; i < enemy->sHEnemySprite.size(); i++) {
					if (enemy->sHEnemySprite[i] == 'o') {
						if (enemy->hurt == false)
							Draw(i % enemy->width + (int)(enemy->pos.x), floor(i / enemy->width) + (int)(enemy->pos.y));
						if (enemy->hurt)
							Draw(i % enemy->width + (int)(enemy->pos.x), floor(i / enemy->width) + (int)(enemy->pos.y), PIXEL_HALF);
					}
				}
			}

			Fill(0, 225, ScreenWidth(), ScreenHeight(), PIXEL_SOLID, FG_DARK_GREY);

			for (unsigned short i = 0; i < MoneyTitle.size(); i++) {
				if (MoneyTitle[i] == 'o') {
					Draw(i % (MoneyTitle.size() / 9) + 5, floor(i / (MoneyTitle.size() / 9) + 233));
				}
			}

			std::string currentMoneyTotal = std::to_string(nMoney);

			Money = OutputText(currentMoneyTotal);
			for (unsigned short i = 0; i < Money.size(); i++) {
				if (Money[i] == 'o') {
					Draw(i % (Money.size() / 9) + 58, floor(i / (Money.size() / 9) + 233));
				}
			}

			if (lowEnemies.size() == 0 && medEnemies.size() == 0 && highEnemies.size() == 0 && gameLevel == 4) {
				gameComplete = true;
			}

			if (m_keys[VK_ESCAPE].bPressed)
			{
				exit(1);
			}
		}
		return true;
	}
};

int main()
{
	Phoenix game;
	game.ConstructConsole(250, 250, 2, 2);
	game.Start();

	return 0;
}