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
short enemiesCreated = 0;
/*
float bXa;
float bXb;
float bYa;
float bYb;
float bX1 = 50.0f;
float bX2 = -50.0f;
float bX3 = 100.0f;
float bY1 = 10.0f;
float bY2 = 150.0f;
float bY3 = 150.0f;
float bi = 0;
*/

struct Coordinate {
	float x;
	float y;
};

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

	Bullet(float x1, float y1) {
		x = x1;
		y = y1;
	}
};

struct Explosion : public GameObject {
	short level = 1;
	int eTimer = 0;
	std::wstring sExplosionSprite = L"";
	Explosion(float x, float y) {
		pos = { x,y };
		width = 13;
		height = 13;
	}
};

std::vector<std::shared_ptr<Explosion>> explosions;

struct Player : public GameObject {
	short level = 1;
	std::wstring sPlayerSprite;
	Player() {
		pos = { 120.0f, 200.0f };
		health = 3;
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

			//LowEnemy temp = new LowEnemy(20, 8);
			//temp->createBezier(89.0f, 79.0f, 14.0f, 117.0f, 34.0f, 238.0f, 108.0f, 170.0f);
			//lowEnemies.push_back(std::unique_ptr<LowEnemy>(new LowEnemy(0, 0)));
			//lowEnemies.push_back(std::unique_ptr<LowEnemy>(new LowEnemy(45, 5)));
			/*
			lowEnemies.push_back(std::unique_ptr<LowEnemy>(new LowEnemy(45, 5)));
			lowEnemies.push_back(std::unique_ptr<LowEnemy>(new LowEnemy(70, 8)));
			lowEnemies.push_back(std::unique_ptr<LowEnemy>(new LowEnemy(95, 5)));
			lowEnemies.push_back(std::unique_ptr<LowEnemy>(new LowEnemy(120, 8)));
			lowEnemies.push_back(std::unique_ptr<LowEnemy>(new LowEnemy(145, 5)));
			lowEnemies.push_back(std::unique_ptr<LowEnemy>(new LowEnemy(170, 8)));
			//lowEnemies.push_back(std::unique_ptr<LowEnemy>(new LowEnemy(220, 5)));
			*/

			//for (auto& enemy : lowEnemies)
			//{
			//	enemy->bezier->setBezier(96.0f, 8.0f, 62.0f, 64.0f, 237.0f, 176.0f, 130.0f, 213.0f);
			//}

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

			std::wstring sStartScreen1 = OutputText("press 'enter' to start");

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
						[](auto& bullet) { return bullet.y >= -1; }
					),
					bullets.end()
				);
			}
			// ==== > DISPLAY < ====================================================

			// First we clear the screen

			Fill(0, 0, ScreenWidth(), ScreenHeight(), L' ', 0);
			// Then draw Title and Border at the top

			std::wstring sStartScreen1 = OutputText("You Win!");
			std::wstring sStartScreen2 = OutputText("Press 'Enter' to play again!");

			if (startScreenTimer > 0 && startScreenTimer < 15) {
				for (unsigned short i = 0; i < sStartScreen1.size(); i++) {
					if (sStartScreen1[i] == 'o') {
						Draw(i % (sStartScreen1.size() / 9) + 100, (floor(i / (sStartScreen1.size() / 9) + ScreenHeight() / 2)) - 40, PIXEL_SOLID, FG_GREEN);
					}
				}
			}
			else {
				for (unsigned short i = 0; i < sStartScreen1.size(); i++) {
					if (sStartScreen1[i] == 'o') {
						Draw(i % (sStartScreen1.size() / 9) + 100, (floor(i / (sStartScreen1.size() / 9) + ScreenHeight() / 2)) - 40, PIXEL_HALF, FG_GREEN);
					}
				}
			}

			if (startScreenTimer > 0 && startScreenTimer < 15) {
				for (unsigned short i = 0; i < sStartScreen2.size(); i++) {
					if (sStartScreen2[i] == 'o') {
						Draw(i % (sStartScreen2.size() / 9) + 28, (floor(i / (sStartScreen2.size() / 9) + ScreenHeight() / 2)), PIXEL_SOLID, FG_GREEN);
					}
				}
			}
			else {
				for (unsigned short i = 0; i < sStartScreen2.size(); i++) {
					if (sStartScreen2[i] == 'o') {
						Draw(i % (sStartScreen2.size() / 9) + 28, (floor(i / (sStartScreen2.size() / 9) + ScreenHeight() / 2)), PIXEL_HALF, FG_GREEN);
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
				Bullet bullet = { player.pos.x,player.pos.y };
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
			/*
			else if (player.level >= 2) {
				for (auto& bullet : bullets)
				{
					if (bullet.x != -1 && bullet.y != -1) {
						bullet.y -= 1; // continue to shift bullet up one pixel to the top of screen

						//level two bullets are twice the size
						for (auto& enemy : lowEnemies)
						{
							if ((bullet.x + 2 >= enemy->pos.x - enemy->size && bullet.x - 2 <= enemy->pos.x + enemy->size) && enemy->pos.y == bullet.y && enemy->health != 0)
								enemy->health -= 1;
						}
					}
				}
			}
			*/

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

			for (auto& enemy : lowEnemies)
			{
				if (enemy->pos.x > 150) {
					/*
					bX1 = enemy->pos.x;
					bX2 = 150.0f;
					bX3 = 50.0f;
					bY1 = enemy->pos.y;
					bY2 = 20.0f;
					bY3 = -50.0f;
					bi = 0.0f;
					switchy = false;
					*/
				}
			}

			/*
			if (timer == 150) {
				for (auto& enemy : lowEnemies)
				{
					bXa = 0;
					bYa = 0;
					bXb = 0;
					bYb = 0;
					bi = 0;
					enemy->pos.x = 0;
					enemy->pos.y = 0;
				}
			}
			*/

			/*
			if (timer <= 50) {
				for (auto& enemy : lowEnemies)
				{
					enemy->pos.x += 1;
				}
				for (auto& enemy : medEnemies)
				{
					enemy->pos.x += .5;
				}
				for (auto& enemy : highEnemies)
				{
					enemy->pos.x += 1;
				}
			}

			else if (timer <= 75) {
				for (auto& enemy : lowEnemies)
				{
					enemy->pos.y += 1;
				}
			}
			else if (timer <= 125) {
				for (auto& enemy : lowEnemies)
				{
					enemy->pos.x -= 1;
				}
				for (auto& enemy : medEnemies)
				{
					enemy->pos.x -= .5;
				}
				for (auto& enemy : highEnemies)
				{
					enemy->pos.x -= 1;
				}
			}
			else if (timer <= 150) {
				for (auto& enemy : lowEnemies)
				{
					enemy->pos.y -= 1;
				}
			}
			*/

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

			if ((int)(timer) == 150) {
				timer = 0;
			}

			// ==== DISPLAY ========================================================

			// Erase All
			Fill(0, 0, ScreenWidth(), ScreenHeight(), L' ', 0);

			//float x9;
			//float y9;

			/*
			for (float i = 0; i < 1; i += 0.01)
			{
				// The Green Lines
				xGreen1 = bezierPoint(ex1, cx1, i);
				yGreen1 = bezierPoint(ey1, cy1, i);
				xGreen2 = bezierPoint(cx1, cx2, i);
				yGreen2 = bezierPoint(cy1, cy2, i);
				xGreen3 = bezierPoint(cx2, ex2, i);
				yGreen3 = bezierPoint(cy2, ey2, i);

				// The Blue Line
				xBlue1 = bezierPoint(xGreen1, xGreen2, i);
				yBlue1 = bezierPoint(yGreen1, yGreen2, i);
				xBlue2 = bezierPoint(xGreen2, xGreen3, i);
				yBlue2 = bezierPoint(yGreen2, yGreen3, i);

				// The Black Dot
				x9 = bezierPoint(xBlue1, xBlue2, i);
				y9 = bezierPoint(yBlue1, yBlue2, i);

				//x9 = bezierPoint(xm, xn, i);
				//y9 = bezierPoint(ym, yn, i);

				/*
				xFinal = bezierPoint(xm, xn, i);
				yFinal = bezierPoint(ym, yn, i);
				*/
				// The Black Dot
			/*
				Draw(x9, y9);
			}
			*/

			// draw bullets
			for (auto& bullet : bullets) // access by reference to avoid copying
			{
				if (bullet.y != -1 && bullet.y > 2) {
					if (player.level == 1) {
						Draw((int)(bullet.x) + 6, (int)(bullet.y), PIXEL_SOLID);
						Draw((int)(bullet.x) + 6, (int)(bullet.y + 1), PIXEL_SOLID);
						Draw((int)(bullet.x) + 6, (int)(bullet.y + 2), PIXEL_SOLID);
						Draw((int)(bullet.x) + 6, (int)(bullet.y + 3), PIXEL_HALF);
						Draw((int)(bullet.x) + 6, (int)(bullet.y + 4), PIXEL_HALF);
						Draw((int)(bullet.x) + 6, (int)(bullet.y + 5), PIXEL_HALF);

						Draw((int)(bullet.x - 1) + 6, (int)(bullet.y), PIXEL_SOLID);
						Draw((int)(bullet.x - 1) + 6, (int)(bullet.y + 1), PIXEL_SOLID);
						Draw((int)(bullet.x - 1) + 6, (int)(bullet.y + 2), PIXEL_SOLID);
						Draw((int)(bullet.x - 1) + 6, (int)(bullet.y + 3), PIXEL_HALF);
						Draw((int)(bullet.x - 1) + 6, (int)(bullet.y + 4), PIXEL_HALF);
						Draw((int)(bullet.x - 1) + 6, (int)(bullet.y + 5), PIXEL_HALF);
						Draw((int)(bullet.x + 1) + 6, (int)(bullet.y), PIXEL_SOLID);
						Draw((int)(bullet.x + 1) + 6, (int)(bullet.y + 1), PIXEL_SOLID);
						Draw((int)(bullet.x + 1) + 6, (int)(bullet.y + 2), PIXEL_SOLID);
						Draw((int)(bullet.x + 1) + 6, (int)(bullet.y + 3), PIXEL_HALF);
						Draw((int)(bullet.x + 1) + 6, (int)(bullet.y + 4), PIXEL_HALF);
						Draw((int)(bullet.x + 1) + 6, (int)(bullet.y + 5), PIXEL_HALF);
					}
				}
			}

			for (auto& explosion : explosions) // access by reference to avoid copying
			{
				for (unsigned short i = 0; i < explosion->sExplosionSprite.size(); i++) {
					if (explosion->sExplosionSprite[i] == 'o') {
						Draw(i % 13 + (int)(explosion->pos.x), floor(i / 13) + (int)(explosion->pos.y));
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
	game.ConstructConsole(250, 225, 2, 2);
	game.Start();

	return 0;
}