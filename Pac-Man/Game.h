#pragma once
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <vector>
#include <random>
#include <string>
#include <fstream>
#include "PacMan.h"
#include "Ghost.h"
#include "Fruit.h"
#include "Tile.h"
#include "Menu.h"

class Game
{
private:
	sf::RenderWindow _window;
	sf::RectangleShape _backGround;
	std::vector< std::vector<Tile> > _tileMap;
	sf::Texture _spriteSheet;
	sf::Clock _clock;
	sf::Clock _killClk;
	sf::Clock _ghostClock;
	sf::Time _ghostTime;
	sf::Time _time;
	sf::Font _font;
	sf::Text _scoreText;
	sf::Event _ev;
	Menu _menu;

	sf::Text _ready;

	PacMan _pacman;
	Ghost _ghost[4];
	Fruit _fruit[8];

	sf::SoundBuffer _ghostMove;
	sf::SoundBuffer _ghostMoveFear;
	sf::SoundBuffer _ghostMoveDead;
	sf::Sound _ghostSound;

	sf::SoundBuffer _eatGhostSound;
	sf::Sound _eatGhost;

	sf::SoundBuffer _startSound;
	sf::Sound _start;

	enum direction { NONE, UP, DOWN, LEFT, RIGHT };
	int _dir = NONE;
	int _cptPoint = 0;
	int _cptSPG = 0;
	int _score = 0;
	int _vie;
	int _finalScore = 0;
	bool _collision = 0;
	bool _isHit = 0;
	bool _gameStarted = 0;
	float _ghostPitch = 1.0f;
	bool _ghostMovingBack = false;

public:
	Game();
	
	void init();
	void importTileMap();
	void importSpriteSheet();

	void exportScore();
	int importScore();

	void play();

	void draw();
	void drawLife();
	void drawScore();
	void drawTileMap();

	void resetPG();
	void resetMap();
	void resetGame();
};