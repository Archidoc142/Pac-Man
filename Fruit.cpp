#include "Fruit.h"

Fruit::Fruit()
{
    // There's nothing to wathch here. Pass you're way :P
}

void Fruit::init(int id)
{
    _fruit.setSize(sf::Vector2f(36, 36));

    sf::IntRect rectTemp(489 + (16 * id), 49, 13, 13);
    _rectSprite = rectTemp;
    _fruit.setTextureRect(_rectSprite);
}

sf::FloatRect Fruit::getCollisionBox() const
{
    return sf::FloatRect::Rect(_fruit.getGlobalBounds().left + 4, _fruit.getGlobalBounds().top + 4,
        _fruit.getGlobalBounds().width - 4, _fruit.getGlobalBounds().height - 4);
}

sf::RectangleShape& Fruit::getFruitShape()
{
    return _fruit;
}

bool Fruit::getIsShowed() const
{
    return _isShowed;
}

void Fruit::setPosition(const int posX, const int posY)
{
    _fruit.setPosition(posX, posY);
}

void Fruit::setIsShowed(const bool isShowed)
{
    _isShowed = isShowed;
}

void Fruit::draw(sf::RenderWindow& window)
{
    window.draw(_fruit);
}