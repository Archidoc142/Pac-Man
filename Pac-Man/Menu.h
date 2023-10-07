#pragma once
#include <string>
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>

class Menu
{
private:
	sf::RectangleShape _bg;
	sf::Texture _bgTexture;

	sf::RectangleShape _tuto;
	sf::Texture _tutoTexture;
	bool _isTuto = false;

	sf::RectangleShape _button[4];
	sf::Text _text[4];
	std::string _textStr[4] = { "START", "TUTORIEL", "QUITTER", "RETOUR" };
	sf::Font _font;

	sf::Text _highScoreTxt;

	sf::SoundBuffer _clickBuffer;
	sf::Sound _click;

public:
	Menu();
	int showMenu(sf::RenderWindow& win, const int score = 0);
	void draw(sf::RenderWindow& win, const int score);
};