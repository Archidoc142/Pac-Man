#include "Ghost.h"

Ghost::Ghost()
{
    //Hi, there's nothing to watch here. so... hum... Hello World! I guess????
    if (!_moveBackBuffer.loadFromFile("Sound/moveback.wav"))
        exit(-1);

    _ghost.setSize(sf::Vector2f(36, 36));
}

sf::RectangleShape& Ghost::getGhostShape()
{
    return _ghost;
}

void Ghost::init(int id)
{

    switch (id) {
    case 0:
        _ghost.setPosition(322, 330);
        break;
    case 1:
        _ghost.setPosition(322, 402);
        break;
    case 2:
        _ghost.setPosition(274, 402);
        break;
    case 3:
        _ghost.setPosition(370, 402);
        break;
    }

    //_ghost.setPosition(322 /*+ id * 40*/, 330);

    sf::IntRect rectTemp(457, 65 + (id * 16), 14, 14);
    _rectSprite = rectTemp;
    _ghost.setTextureRect(_rectSprite);
    _killable = false;
    _isStatic = true;
}

sf::FloatRect Ghost::getCollisionBox() const
{
    return sf::FloatRect::Rect(_ghost.getGlobalBounds().left + 6, _ghost.getGlobalBounds().top + 6,
        _ghost.getGlobalBounds().width - 6, _ghost.getGlobalBounds().height - 6);
}

void Ghost::move(int id, std::vector< std::vector<Tile> > tileMap)
{
    int cellX = (getCollisionBox().left / 24);
    int cellY = (getCollisionBox().top - 72) / 24;
    int cptPos = 0;


    if (cellX <= 0 || cellX >= 27)
    {
        if (_crntDir == 3)
        {
            _ghost.move(-8, 0);
            if (_ghost.getPosition().x <= -38)
                _ghost.setPosition(sf::Vector2f(674, 402));
        }
        else if (_crntDir == 4)
        {
            _ghost.move(8, 0);
            if (_ghost.getPosition().x >= 674)
                _ghost.setPosition(sf::Vector2f(-38, 402));
        }
    }
    else
    {
        switch (_crntDir)
        {
        case 1:
            if ((tileMap[cellY - 1][cellX].getTileType() != 1 && getCollisionBox().left == tileMap[cellY][cellX].getTile().getPosition().x) || getCollisionBox().top != tileMap[cellY][cellX].getTile().getPosition().y)
            {
                _pos[cptPos] = 1;
                cptPos++;
            }
            if ((tileMap[cellY][cellX - 1].getTileType() != 1 && getCollisionBox().top == tileMap[cellY][cellX].getTile().getPosition().y) || getCollisionBox().left != tileMap[cellY][cellX].getTile().getPosition().x)
            {
                _pos[cptPos] = 3;
                cptPos++;
            }
            if ((tileMap[cellY][cellX + 1].getTileType() != 1 && getCollisionBox().top == tileMap[cellY][cellX].getTile().getPosition().y) || getCollisionBox().left != tileMap[cellY][cellX].getTile().getPosition().x)
            {
                _pos[cptPos] = 4;
                cptPos++;
            }
            break;
        case 2:
            if ((tileMap[cellY + 1][cellX].getTileType() != 1 && getCollisionBox().left == tileMap[cellY][cellX].getTile().getPosition().x) || getCollisionBox().top != tileMap[cellY][cellX].getTile().getPosition().y)
            {
                _pos[cptPos] = 2;
                cptPos++;
            }
            if ((tileMap[cellY][cellX - 1].getTileType() != 1 && getCollisionBox().top == tileMap[cellY][cellX].getTile().getPosition().y) || getCollisionBox().left != tileMap[cellY][cellX].getTile().getPosition().x)
            {
                _pos[cptPos] = 3;
                cptPos++;
            }
            if ((tileMap[cellY][cellX + 1].getTileType() != 1 && getCollisionBox().top == tileMap[cellY][cellX].getTile().getPosition().y) || getCollisionBox().left != tileMap[cellY][cellX].getTile().getPosition().x)
            {
                _pos[cptPos] = 4;
                cptPos++;
            }
            break;
        case 3:
            if ((tileMap[cellY][cellX - 1].getTileType() != 1 && getCollisionBox().top == tileMap[cellY][cellX].getTile().getPosition().y) || getCollisionBox().left != tileMap[cellY][cellX].getTile().getPosition().x)
            {
                _pos[cptPos] = 3;
                cptPos++;
            }
            if ((tileMap[cellY + 1][cellX].getTileType() != 1 && getCollisionBox().left == tileMap[cellY][cellX].getTile().getPosition().x) || getCollisionBox().top != tileMap[cellY][cellX].getTile().getPosition().y)
            {
                _pos[cptPos] = 2;
                cptPos++;
            }
            if ((tileMap[cellY - 1][cellX].getTileType() != 1 && getCollisionBox().left == tileMap[cellY][cellX].getTile().getPosition().x) || getCollisionBox().top != tileMap[cellY][cellX].getTile().getPosition().y)
            {
                _pos[cptPos] = 1;
                cptPos++;
            }
            break;
        case 4:
            if ((tileMap[cellY][cellX + 1].getTileType() != 1 && getCollisionBox().top == tileMap[cellY][cellX].getTile().getPosition().y) || getCollisionBox().left != tileMap[cellY][cellX].getTile().getPosition().x)
            {
                _pos[cptPos] = 4;
                cptPos++;
            }
            if ((tileMap[cellY - 1][cellX].getTileType() != 1 && getCollisionBox().left == tileMap[cellY][cellX].getTile().getPosition().x) || getCollisionBox().top != tileMap[cellY][cellX].getTile().getPosition().y)
            {
                _pos[cptPos] = 1;
                cptPos++;
            }
            if ((tileMap[cellY + 1][cellX].getTileType() != 1 && getCollisionBox().left == tileMap[cellY][cellX].getTile().getPosition().x) || getCollisionBox().top != tileMap[cellY][cellX].getTile().getPosition().y)
            {
                _pos[cptPos] = 2;
                cptPos++;
            }
            break;
        }

        int newDir = rand() % cptPos;
        _crntDir = _pos[newDir];

        switch (_crntDir)
        {
        case 1:
            _ghost.move(0, -8);
            break;
        case 2:
            _ghost.move(0, 8);
            break;
        case 3:
            _ghost.move(-8, 0);
            break;
        case 4:
            _ghost.move(8, 0);
            break;
        }
    }
    if (_cpt == 2)
        animation(id);
    _cpt++;

    _ghost.setTextureRect(_rectSprite);
}

void Ghost::hunt(int id, std::vector<std::vector<Tile>> tileMap, int cellXP, int cellYP)
{
    int cellX = floor(getCollisionBox().left / 24);
    int cellY = floor(getCollisionBox().top - 72) / 24;
    int moveX = 0;

    if (cellX <= 0 || cellX >= 27)
    {
        if (_crntDir == 3)
        {
            _ghost.move(-8, 0);
            if (_ghost.getPosition().x <= -38)
                _ghost.setPosition(sf::Vector2f(674, 402));
        }
        else if (_crntDir == 4)
        {
            _ghost.move(8, 0);
            if (_ghost.getPosition().x >= 674)
                _ghost.setPosition(sf::Vector2f(-38, 402));
        }
    }
    else
    {
        // J'ai pas trouvé mieux : il y a un out of range si le pac man passe dans le tunnel (pas sur une tuile)
        if (!(cellXP <= 0 || cellXP >= 27))
        {
            tileMap[cellY][cellX].setVisited(1);

            // Trouve pacman
            while (!tileMap[cellYP][cellXP].getVisited())
            {
                for (int y = 0; y < 31; y++)
                {
                    for (int x = 0; x < 28; x++)
                    {
                        if (tileMap[y][x].getVisited())
                        {
                            if (y + 1 < 31 && x < 28 && tileMap[y + 1][x].getTileType() != 1 && !tileMap[y + 1][x].getVisited())
                            {
                                tileMap[y + 1][x].setVisited(1);
                                tileMap[y + 1][x].setParent(y, x);
                            }
                            if (y - 1 >= 0 && x < 28 && tileMap[y - 1][x].getTileType() != 1 && !tileMap[y - 1][x].getVisited())
                            {
                                tileMap[y - 1][x].setVisited(1);
                                tileMap[y - 1][x].setParent(y, x);
                            }
                            if (x + 1 < 28 && y < 31 && tileMap[y][x + 1].getTileType() != 1 && !tileMap[y][x + 1].getVisited())
                            {
                                tileMap[y][x + 1].setVisited(1);
                                tileMap[y][x + 1].setParent(y, x);
                            }
                            if (x - 1 >= 0 && y < 31 && tileMap[y][x - 1].getTileType() != 1 && !tileMap[y][x - 1].getVisited())
                            {
                                tileMap[y][x - 1].setVisited(1);
                                tileMap[y][x - 1].setParent(y, x);
                            }
                        }
                    }
                }
            }

            int checkCellY = tileMap[cellYP][cellXP].getParent(0);
            int checkCellX = tileMap[cellYP][cellXP].getParent(1);
            int temp;

            // Trouve le chemin
            while (!tileMap[cellY][cellX].getPath())
            {
                tileMap[checkCellY][checkCellX].setPath(1);

                if (tileMap[cellY][cellX].getPath()) // Le point d'origine (ghost) n'a pas de parent
                    break;
                temp = checkCellY;
                checkCellY = tileMap[checkCellY][checkCellX].getParent(0);
                checkCellX = tileMap[temp][checkCellX].getParent(1);
            }

            // Suis le chemin
            if ((tileMap[cellY - 1][cellX].getPath() && getCollisionBox().left == tileMap[cellY][cellX].getTile().getPosition().x))
                _crntDir = 1;
            else if ((tileMap[cellY + 1][cellX].getPath() && getCollisionBox().left == tileMap[cellY][cellX].getTile().getPosition().x))
                _crntDir = 2;
            else if ((tileMap[cellY][cellX - 1].getPath() && getCollisionBox().top == tileMap[cellY][cellX].getTile().getPosition().y))
                _crntDir = 3;
            else if ((tileMap[cellY][cellX + 1].getPath() && getCollisionBox().top == tileMap[cellY][cellX].getTile().getPosition().y))
                _crntDir = 4;

            switch (_crntDir)
            {
            case 1:
                _ghost.move(0, -8);
                break;
            case 2:
                _ghost.move(0, 8);
                break;
            case 3:
                _ghost.move(-8, 0);
                break;
            case 4:
                _ghost.move(8, 0);
                break;
            }
        }
    }
    if (_cpt == 2)
        animation(id);
    _cpt++;

    _ghost.setTextureRect(_rectSprite);
}

void Ghost::setIsStatic(const bool isStatic)
{
    _isStatic = isStatic;
}

void Ghost::setCrntDir(const int dir)
{
    _crntDir = dir;
}

void Ghost::animation(int id)
{
    if (_isDead)
    {
        _rectSprite.top = 81;

        switch (_crntDir)
        {
        case 1:
                _rectSprite.left = 617;
            break;
        case 2:
                _rectSprite.left = 633;
            break;
        case 3:
                _rectSprite.left = 601;
            break;
        case 4:
                _rectSprite.left = 585;
            break;
        }
    }
    else if (!_killable)
    {
        _rectSprite.top = 65 + (id * 16);

        switch (_crntDir)
        {
        case 1:
            if (_frameAnim == 1)
                _rectSprite.left = 521;
            else
                _rectSprite.left = 537;
            break;
        case 2:
            if (_frameAnim == 1)
                _rectSprite.left = 553;
            else
                _rectSprite.left = 569;
            break;
        case 3:
            if (_frameAnim == 1)
                _rectSprite.left = 489;
            else
                _rectSprite.left = 505;
            break;
        case 4:
            if (_frameAnim == 1)
                _rectSprite.left = 457;
            else
                _rectSprite.left = 473;
            break;
        }
        if (_frameAnim == 2)
            _frameAnim--;
        else
            _frameAnim++;
    }
    else
    {
        if (_rectSprite.left < 585)
            _rectSprite.left = 585;

        _rectSprite.top = 65;
        if (_rectSprite.left >= 633)
            _rectSprite.left = 585;
        _rectSprite.left += 16;
    }
    _cpt = 0;
}

void Ghost::draw(sf::RenderWindow& window)
{
	window.draw(_ghost);
}

bool Ghost::getKillable() const
{
    return _killable;
}

bool Ghost::getIsStatic() const
{
    return _isStatic;
}

bool Ghost::getIsDead()
{
    return _isDead;
}

void Ghost::setKillable(const bool killable)
{
    _killable = killable;
}

void Ghost::setIsDead(const bool isDead)
{
    _isDead = isDead;
}

void Ghost::setPosition(const int x, const int y)
{
    _ghost.setPosition(x, y);
}

void Ghost::resetPosition(int id)
{
    _isStatic = true;
    _killable = false;

    switch (id) {
    case 0:
        _ghost.setPosition(322, 330);
        break;
    case 1:
        _ghost.setPosition(322, 402);
        break;
    case 2:
        _ghost.setPosition(274, 402);
        break;
    case 3:
        _ghost.setPosition(370, 402);
        break;
    }

}
