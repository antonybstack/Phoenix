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
unsigned short gameLevel = 1;
bool gameStart = false;
bool gameComplete = false;
bool gamePause = false;
bool levelComplete = true;
unsigned int nScore = 0;

struct Coordinate {
	float x;
	float y;
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
		health = 1;
		width = 13;
		height = 13;
	}
};

std::vector<std::shared_ptr<Explosion>> explosions;

struct Player : public GameObject {
	short level = 1;
	std::wstring sPlayerSprite;
	Player() {
		pos = { 120.0f, 260.0f };
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
	bool hurt = false;
	short hurtTimer = 0;
	LowEnemy(float x, float y) {
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
	}
	~LowEnemy() {
		explosions.push_back(std::unique_ptr<Explosion>(new Explosion(this->pos.x, this->pos.y)));
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
			//lowEnemies.push_back(std::unique_ptr<LowEnemy>(new LowEnemy(20, 5)));
			lowEnemies.push_back(std::unique_ptr<LowEnemy>(new LowEnemy(20, 8)));
			lowEnemies.push_back(std::unique_ptr<LowEnemy>(new LowEnemy(45, 5)));
			lowEnemies.push_back(std::unique_ptr<LowEnemy>(new LowEnemy(70, 8)));
			lowEnemies.push_back(std::unique_ptr<LowEnemy>(new LowEnemy(95, 5)));
			lowEnemies.push_back(std::unique_ptr<LowEnemy>(new LowEnemy(120, 8)));
			lowEnemies.push_back(std::unique_ptr<LowEnemy>(new LowEnemy(145, 5)));
			lowEnemies.push_back(std::unique_ptr<LowEnemy>(new LowEnemy(170, 8)));
			//lowEnemies.push_back(std::unique_ptr<LowEnemy>(new LowEnemy(220, 5)));
			break;
		}
		case 2:
			timer = 0;
			lowEnemies.push_back(std::unique_ptr<LowEnemy>(new LowEnemy(20, 50)));
			lowEnemies.push_back(std::unique_ptr<LowEnemy>(new LowEnemy(45, 50)));
			lowEnemies.push_back(std::unique_ptr<LowEnemy>(new LowEnemy(70, 50)));
			lowEnemies.push_back(std::unique_ptr<LowEnemy>(new LowEnemy(95, 50)));
			lowEnemies.push_back(std::unique_ptr<LowEnemy>(new LowEnemy(120, 50)));
			lowEnemies.push_back(std::unique_ptr<LowEnemy>(new LowEnemy(145, 50)));
			lowEnemies.push_back(std::unique_ptr<LowEnemy>(new LowEnemy(170, 50)));
			medEnemies.push_back(std::unique_ptr<MedEnemy>(new MedEnemy(10, 5)));
			medEnemies.push_back(std::unique_ptr<MedEnemy>(new MedEnemy(60, 8)));
			medEnemies.push_back(std::unique_ptr<MedEnemy>(new MedEnemy(110, 8)));
			medEnemies.push_back(std::unique_ptr<MedEnemy>(new MedEnemy(160, 5)));
			break;

		case 3:
			lowEnemies.push_back(std::unique_ptr<LowEnemy>(new LowEnemy(200, 10)));

			medEnemies.push_back(std::unique_ptr<MedEnemy>(new MedEnemy(200, 40)));

			highEnemies.push_back(std::unique_ptr<HighEnemy>(new HighEnemy(200, 70)));

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

			Fill(0, 0, ScreenWidth(), ScreenHeight(), L' ', 0);
			// Then draw Title and Border at the top

			std::wstring sStartScreen1 = OutputText("press 'enter' to start");

			if (startScreenTimer > 0 && startScreenTimer < 15) {
				for (unsigned short i = 0; i < sStartScreen1.size(); i++) {
					if (sStartScreen1[i] == 'o') {
						Draw(i % (sStartScreen1.size() / 9) + 50, floor(i / (sStartScreen1.size() / 9) + ScreenHeight() / 2));
					}
				}
			}
			else {
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
			if (m_keys[VK_RETURN].bPressed) {
				// reset all objects and variables before reset
				gameComplete = false;
				gameLevel = 0;
				timer = 0;
				levelComplete = true;
				nScore = 0;
				player.level = 1;
				player.pos.x = 120;
				player.pos.y = 260;
				player.health = 3;
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

			// Then draw Title and Border at the top

			// draw title

			//blinking reset screen
		}
		// Game started
		else {
			// ==== > LOGIC < ======================================================
			timer++;
			//updates current level
			if (levelComplete == true) {
				gameLevel++;
				updateLevel();
				levelComplete = false;
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
						bullet.y -= 250 * fElapsedTime; // continue to shift bullet up one pixel to the top of screen
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
						Draw((int)(bullet.x) + 6, (int)(bullet.y), PIXEL_SOLID, FG_RED);
						Draw((int)(bullet.x) + 6, (int)(bullet.y + 1), PIXEL_SOLID, FG_RED);
						Draw((int)(bullet.x) + 6, (int)(bullet.y + 2), PIXEL_SOLID, FG_RED);
						Draw((int)(bullet.x) + 6, (int)(bullet.y + 3), PIXEL_HALF, FG_RED);
						Draw((int)(bullet.x) + 6, (int)(bullet.y + 4), PIXEL_HALF, FG_RED);
						Draw((int)(bullet.x) + 6, (int)(bullet.y + 5), PIXEL_HALF, FG_RED);

						Draw((int)(bullet.x - 1) + 6, (int)(bullet.y), PIXEL_SOLID, FG_RED);
						Draw((int)(bullet.x - 1) + 6, (int)(bullet.y + 1), PIXEL_SOLID, FG_RED);
						Draw((int)(bullet.x - 1) + 6, (int)(bullet.y + 2), PIXEL_SOLID, FG_RED);
						Draw((int)(bullet.x - 1) + 6, (int)(bullet.y + 3), PIXEL_HALF, FG_RED);
						Draw((int)(bullet.x - 1) + 6, (int)(bullet.y + 4), PIXEL_HALF, FG_RED);
						Draw((int)(bullet.x - 1) + 6, (int)(bullet.y + 5), PIXEL_HALF, FG_RED);
						Draw((int)(bullet.x + 1) + 6, (int)(bullet.y), PIXEL_SOLID, FG_RED);
						Draw((int)(bullet.x + 1) + 6, (int)(bullet.y + 1), PIXEL_SOLID, FG_RED);
						Draw((int)(bullet.x + 1) + 6, (int)(bullet.y + 2), PIXEL_SOLID, FG_RED);
						Draw((int)(bullet.x + 1) + 6, (int)(bullet.y + 3), PIXEL_HALF, FG_RED);
						Draw((int)(bullet.x + 1) + 6, (int)(bullet.y + 4), PIXEL_HALF, FG_RED);
						Draw((int)(bullet.x + 1) + 6, (int)(bullet.y + 5), PIXEL_HALF, FG_RED);
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
				for (unsigned short i = 0; i < enemy->sLEnemySprite.size(); i++) {
					if (enemy->sLEnemySprite[i] == 'o') {
						if (enemy->hurt == false)
							Draw(i % enemy->width + (int)(enemy->pos.x), floor(i / enemy->width) + (int)(enemy->pos.y));
						if (enemy->hurt)
							Draw(i % enemy->width + (int)(enemy->pos.x), floor(i / enemy->width) + (int)(enemy->pos.y), PIXEL_HALF);
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

			if (lowEnemies.size() == 0 && medEnemies.size() == 0 && highEnemies.size() == 0) {
				levelComplete = true;
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
	game.ConstructConsole(250, 300, 2, 2);
	game.Start();

	return 0;
}