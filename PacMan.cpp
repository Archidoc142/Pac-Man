#include "PacMan.h"

PacMan::PacMan(sf::Texture& _spriteSheet)
{
	if (!(_eatBuffer.loadFromFile("Sound/gomme.wav")
		&& _invincibleBuffer.loadFromFile("Sound/invincible.ogg")
		&& _deathBuffer.loadFromFile("Sound/death.wav")))
	{
		exit(1);
	}

	_eatSound.setBuffer(_eatBuffer);
	_invincibleSound.setBuffer(_invincibleBuffer);
	_deathSound.setBuffer(_deathBuffer);

    _pacman.setSize(sf::Vector2f(36, 36));

    _pacman.setTexture(&_spriteSheet);

    sf::IntRect rectTemp(489, 1, 13, 13);
    _rectSprite = rectTemp;
    _pacman.setTextureRect(_rectSprite);
}

int PacMan::getX() const
{
	return _pacman.getPosition().x;
}
int PacMan::getY() const
{
	return _pacman.getPosition().y;
}
const sf::Vector2f& PacMan::getPosition() const
{
	return _pacman.getPosition();
}
sf::RectangleShape& PacMan::getPacManShape()
{
	return _pacman;
}

sf::FloatRect PacMan::getCollisionBox() const
{
    return sf::FloatRect::Rect(_pacman.getGlobalBounds().left + 6, _pacman.getGlobalBounds().top + 6,
        _pacman.getGlobalBounds().width - 6, _pacman.getGlobalBounds().height - 6);
}

void PacMan::setPosition(const int posX, const int posY)
{
	_pacman.setPosition(posX, posY);
}

bool PacMan::getCanMove() const
{
    return _canMove;
}

bool PacMan::getCanKill() const
{
	return _canKill;
}

void PacMan::move(int dir, std::vector< std::vector<Tile> > tileMap)
{
	if (_canMove)
	{
		int cellX = (getCollisionBox().left / 24);
		int cellY = (getCollisionBox().top - 72) / 24;
		bool canTurn = 0;
		sf::Vector2f posInit(_pacman.getPosition().x, _pacman.getPosition().y);

        if(cellX <= 0 || cellX >= 27)
        {
            if (dir == 3)
            {
                _pacman.move(-8, 0);
                if (_pacman.getPosition().x <= -38)
                    _pacman.setPosition(sf::Vector2f(674, 402));
            }
            else if(dir == 4)
            {
                _pacman.move(8, 0);
                if (_pacman.getPosition().x >= 674)
                    _pacman.setPosition(sf::Vector2f(-38, 402));
            }
        }
        else
        {
			switch (dir)
			{
			case 1:
				canTurn = tileMap[cellY - 1][cellX].getTileType() != 1 && getCollisionBox().left == tileMap[cellY][cellX].getTile().getPosition().x;
				break;
			case 2:
				canTurn = tileMap[cellY + 1][cellX].getTileType() != 1 && getCollisionBox().left == tileMap[cellY][cellX].getTile().getPosition().x;
				break;
			case 3:
				canTurn = tileMap[cellY][cellX - 1].getTileType() != 1 && getCollisionBox().top == tileMap[cellY][cellX].getTile().getPosition().y;
				break;
			case 4:
				canTurn = tileMap[cellY][cellX + 1].getTileType() != 1 && getCollisionBox().top == tileMap[cellY][cellX].getTile().getPosition().y;
				break;
			}

			if (canTurn)
				_crntDir = dir;

			switch (_crntDir)
			{
			case 1:
				if (tileMap[cellY - 1][cellX].getTileType() == 1) // Si la prochaine cellule est un mur
				{
					if (getCollisionBox().top != tileMap[cellY][cellX].getTile().getPosition().y) // Bouge jusqu'au bout de la cellule
					{
						_pacman.move(0, -8);
					}
				}
				else // Si pas un mur
				{
					if (getCollisionBox().left == tileMap[cellY][cellX].getTile().getPosition().x) // Si on arrive au bout de la cellule
						_pacman.move(0, -8);
				}
				break;
			case 2:
				if (tileMap[cellY + 1][cellX].getTileType() == 1)
				{
					if (getCollisionBox().top != tileMap[cellY][cellX].getTile().getPosition().y)
					{
						_pacman.move(0, 8);
					}
				}
				else
				{
					if (getCollisionBox().left == tileMap[cellY][cellX].getTile().getPosition().x) // Si on arrive au bout de la cellule
						_pacman.move(0, 8);
				}
				break;

			case 3:
				if (tileMap[cellY][cellX - 1].getTileType() == 1)
				{
					if (getCollisionBox().left != tileMap[cellY][cellX].getTile().getPosition().x)
					{
						_pacman.move(-8, 0);
					}
				}
				else
				{
					if (getCollisionBox().top == tileMap[cellY][cellX].getTile().getPosition().y) // Si on arrive au bout de la cellule
						_pacman.move(-8, 0);
				}
				break;
			case 4:
				if (tileMap[cellY][cellX + 1].getTileType() == 1)
				{
					if (getCollisionBox().left != tileMap[cellY][cellX].getTile().getPosition().x)
					{
						_pacman.move(8, 0);
					}
				}
				else
				{
					if (getCollisionBox().top == tileMap[cellY][cellX].getTile().getPosition().y) // Si on arrive au bout de la cellule
						_pacman.move(8, 0);
				}
				break;
			}

			if (_pacman.getPosition().x != posInit.x || _pacman.getPosition().y != posInit.y)
				_isMoving = 1;
			else
				_isMoving = 0;
        }

		_cpt++;
		if (_cpt == 2)
			moveAnimation();
	}
}

bool PacMan::eat(std::vector<std::vector<Tile>> &tileMap, sf::RenderWindow &window, int &score, int& pacDots)
{
    int cellX = (getCollisionBox().left / 24);
    int cellY = (getCollisionBox().top - 72) / 24;

    if (cellX >= 0 && cellX < 28)
    {
        switch (tileMap[cellY][cellX].getTileType())
        {
        case 2:
            tileMap[cellY][cellX].setTileType(0);
			pacDots++;
            score += 10;
			if (_eatSound.getStatus() == sf::Sound::Stopped)
				_eatSound.play();
            break;
        case 3:
            tileMap[cellY][cellX].setTileType(0);
			score += 50;
			_invincibleSound.play();
			_canKill = 1;
			return 1;
        }
    }
	return 0;
}

void PacMan::setIntRectLeft(const int left)
{
	_rectSprite.left = left;
	_pacman.setTextureRect(_rectSprite);
}

void PacMan::moveAnimation()
{
    if (_isMoving)
    {
        if (_rectSprite.left >= 489)
            _rectSprite.left = 457;
        else
            _rectSprite.left += 16;

        switch (_crntDir)
        {
        case 1:
            _rectSprite.top = 33;
            break;
        case 2:
            _rectSprite.top = 49;
            break;
        case 3:
            _rectSprite.top = 17;
            break;
        }

        if (_rectSprite.left == 489)
            _rectSprite.top = 1;

		_pacman.setTextureRect(_rectSprite);
    }
    _cpt = 0;
}

void PacMan::deathAnimation()
{
    _rectSprite.top = 1;

	if (_rectSprite.left == 489)
		_deathSound.play();

    if (_rectSprite.left < 489)
        _rectSprite.left = 489;
    else
        _rectSprite.left += 16;

	if (_rectSprite.left == 681)
		_crntDir = 3;

    _pacman.setTextureRect(_rectSprite);
}

void PacMan::setCanMove(const bool canMove)
{
    _canMove = canMove;
}

void PacMan::setCanKill(const bool canKill)
{
	_canKill = canKill;
}

void PacMan::draw(sf::RenderWindow& window)
{
	window.draw(_pacman);
}

sf::IntRect PacMan::getRectSprite() const
{
    return _rectSprite;
}