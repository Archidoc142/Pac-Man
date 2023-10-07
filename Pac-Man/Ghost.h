#pragma once

#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <random>
#include "Tile.h"

class Ghost
{
private:
	sf::RectangleShape _ghost;
	sf::IntRect _rectSprite;
	int _crntDir;
	int _nbPos;
	int _pos[3];
	int _frameAnim = 1;
	int _cpt = 0;
	bool _killable = 0;
	bool _isDead;
	bool _isStatic;

	sf::SoundBuffer _moveBackBuffer;
	sf::Sound _moveBackSound;

public:
	Ghost();
	void init(int id);

	sf::RectangleShape& getGhostShape();
	sf::FloatRect getCollisionBox() const;
	bool getKillable()const;
	bool getIsDead();
	bool getIsStatic()const;

	void setIsStatic(const bool isStatic);
	void setCrntDir(const int dir);
	void setKillable(const bool killable);
	void setIsDead(const bool isDead);
	void setPosition(const int x, const int y);
	void resetPosition(int id);

	void move(int id, std::vector< std::vector<Tile> > tileMap);
	void hunt(int id, std::vector< std::vector<Tile> > tileMap, int cellX, int cellY);
	void animation(int id);
	void draw(sf::RenderWindow& window);
};