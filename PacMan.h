#pragma once

#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <Windows.h>
#include "Tile.h"

class PacMan
{
private:
	sf::RectangleShape _pacman;
	sf::IntRect _rectSprite;

	sf::SoundBuffer _eatBuffer;
	sf::SoundBuffer _invincibleBuffer;
	sf::SoundBuffer _deathBuffer;

	sf::Sound _eatSound;
	sf::Sound _invincibleSound;
	sf::Sound _deathSound;

	int _crntDir = 0;
	int _cpt = 0;
	bool _isMoving;
	bool _canMove = 1;
	bool _canKill = 0;

public:
	PacMan(sf::Texture& _spriteSheet);

	int getX()const;
	int getY()const;
	bool getCanMove()const;
	bool getCanKill()const;
	sf::IntRect getRectSprite()const;
	const sf::Vector2f& getPosition()const;
	sf::RectangleShape& getPacManShape();
	sf::FloatRect getCollisionBox() const;

	void setPosition(const int posX, const int posY);
	void setCanMove(const bool canMove);
	void setCanKill(const bool canKill);
	void setIntRectLeft(const int left);

	bool eat(std::vector<std::vector<Tile>>& tileMap, sf::RenderWindow& window, int& score, int& pacDots);
	void move(int dir, std::vector< std::vector<Tile> > tileMap);
	void moveAnimation();
	void deathAnimation();

	void draw(sf::RenderWindow& window);
};