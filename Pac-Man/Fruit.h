#pragma once

#include <SFML/Graphics.hpp>

class Fruit
{
private:
	sf::RectangleShape _fruit;
	sf::IntRect _rectSprite;
	bool _isShowed = 0;
public:
	Fruit();
	void init(int id);

	sf::FloatRect getCollisionBox() const;
	sf::RectangleShape& getFruitShape();
	bool getIsShowed()const;

	void setPosition(const int posX, const int posY);
	void setIsShowed(const bool isShowed);

	void draw(sf::RenderWindow& window);
};