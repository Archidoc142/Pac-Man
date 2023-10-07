#include "Game.h"

Game::Game() : _pacman(_spriteSheet)
{
    for (int i = 0; i < 8; i++)
    {
        _fruit[i].init(i);
        _fruit[i].getFruitShape().setTexture(&_spriteSheet);
    }
    for (int i = 0; i < 4; i++)
    {
        _ghost[i].init(i);
        _ghost[i].getGhostShape().setTexture(&_spriteSheet);
    }

	init();
    while (_window.isOpen())
    {
        switch (_menu.showMenu(_window, importScore()))
        {
        case 1:
            play();
            break;
        case 2:
            exit(0);
            break;
        default:
            continue;
        }
    }
}

void Game::init()
{
    srand(time(NULL));

    importSpriteSheet();
    importTileMap();

    if (!(_ghostMove.loadFromFile("Sound/ghostmove.wav")
        && _ghostMoveFear.loadFromFile("Sound/ghostfear.wav")
        && _eatGhostSound.loadFromFile("Sound/pacman_eatGhost.ogg")
        && _startSound.loadFromFile("Sound/GameStart.wav")
        && _ghostMoveDead.loadFromFile("Sound/moveback.wav")))
    {
        exit(1);
    }

    _ghostSound.setBuffer(_ghostMove);
    _eatGhost.setBuffer(_eatGhostSound);
    _start.setBuffer(_startSound);
    _start.setLoop(false);

	_window.create(sf::VideoMode(672, 888), "Pac-Man.SFML");
    
    _backGround.setSize(sf::Vector2f(672, 744));
    // Do you have a problem here? You need to put sfml manualy
    _backGround.setTexture(&_spriteSheet);
    sf::IntRect rectTemp(228, 0, 224, 248);
    _backGround.setTextureRect(rectTemp);
    _backGround.setPosition(sf::Vector2f(0, 72));

    if (!_font.loadFromFile("Sprite/emulogic.ttf"))
        exit(1);

    _scoreText.setFont(_font);
    _scoreText.setCharacterSize(24);

    _ready.setFont(_font);
    _ready.setCharacterSize(24);
    _ready.setString("READY!");
    _ready.setFillColor(sf::Color::Yellow);

    float xPos = (_window.getSize().x - _ready.getGlobalBounds().width) / 2;
    _ready.setPosition(xPos, 480);

    _vie = 3;
    _score = 0;
}

void Game::importTileMap()
{
    std::ifstream fTileMap("TileMap.txt");
    sf::IntRect rectTemp(0, 0, 8, 8);

    if (fTileMap)
    {
        int estMur;

        for (int y = 0; y < 31; y++) 
        {
            _tileMap.push_back(std::vector<Tile>());

            for (int x = 0; x < 28; x++) 
            {
                fTileMap >> estMur;
                _tileMap[y].push_back(Tile(x, y, estMur));

                switch (_tileMap[y][x].getTileType())
                {
                case 2:
                    rectTemp.left = 168;
                    rectTemp.top = 208;
                    _tileMap[y][x].getTile().setTexture(&_spriteSheet);
                    _tileMap[y][x].getTile().setTextureRect(rectTemp);
                    break;
                case 3:
                    rectTemp.left = 208;
                    rectTemp.top = 184;
                    _tileMap[y][x].getTile().setTexture(&_spriteSheet);
                    _tileMap[y][x].getTile().setTextureRect(rectTemp);
                    break;
                }
            }
        }
    }

    fTileMap.close();
}

void Game::importSpriteSheet()
{
    if (!_spriteSheet.loadFromFile("Sprite/newspritesheet.png"))
        exit(1);
}

void Game::exportScore()
{
    int scoreIn = 0;

    std::ifstream scoreTxtIn("highScore.txt");
    if (scoreTxtIn)
    {
        scoreTxtIn >> scoreIn;
    }
    scoreTxtIn.close();

    std::ofstream scoreTxtOut("highScore.txt");
    if (scoreTxtOut)
    {
        if (scoreIn < _finalScore)
        {
            scoreTxtOut << _finalScore;
        }
        else
            // depuis quand le fait de juste DÉCLARER un ofstream EFFACE TOUT LE CONTENU DU TXT???????????
        {
            scoreTxtOut << scoreIn;
        }
    }
    scoreTxtOut.close();
}

int Game::importScore()
{
    int scoreIn = 0;

    std::ifstream scoreTxtIn;
    scoreTxtIn.open("highScore.txt");
    if (scoreTxtIn)
    {
        scoreTxtIn >> scoreIn;
    }
    scoreTxtIn.close();

    return scoreIn;
}

void Game::play()
{
    // Je pourrais faire une struct, mais... n'en ai pas envie :P
    int killRow = 0;
    int ghostMod = 0;

    int cellX;
    int cellY;

    int cptPacDotEated = 0;
    int cptFruitEated = 0;
    int level = 1;

    bool SPGEated;
    bool isEat = 0;

    _pacman.setPosition(322, 618);
    for (int i = 0; i < 4; i++)
        _ghost[i].setCrntDir(rand() % 2 + 2);

    while (_window.isOpen())
    {
        if (!_gameStarted)
        {
            if (_start.getStatus() == sf::Sound::Stopped)
            {
                _clock.restart();
                _start.play();
            }

            int startSoundTime = _clock.getElapsedTime().asMilliseconds();
            if (startSoundTime >= 4455)
            {
                _gameStarted = true;
                _ghostClock.restart();
            }
        }
        else
        {
            cellX = (_pacman.getCollisionBox().left / 24);
            cellY = (_pacman.getCollisionBox().top - 72) / 24;

            sf::Event event;

            while (_window.pollEvent(event))
            {
                if (event.type == sf::Event::Closed) {
                    _window.close();
                }

                else if (event.type == sf::Event::KeyPressed)
                {
                    switch (event.key.code)
                    {
                    case sf::Keyboard::Escape:
                        _window.close();
                        break;
                    case sf::Keyboard::Up:
                        _dir = UP;
                        break;
                    case sf::Keyboard::Down:
                        _dir = DOWN;
                        break;
                    case sf::Keyboard::Left:
                        _dir = LEFT;
                        break;
                    case sf::Keyboard::Right:
                        _dir = RIGHT;
                        break;
                    }
                }
            }

            _ghost[0].setIsStatic(false);
            _ghostTime = _ghostClock.getElapsedTime();

            for (int i = 1; i < 4; i++)
                if (_ghostTime.asSeconds() >= i * 3 && _ghost[i].getIsStatic()) 
                {
                    _ghost[i].setIsStatic(false);
                    _ghost[i].setPosition(322, 330);
                }

            bool crntPacmanStatus = _pacman.getCanKill();

            _time = _clock.getElapsedTime();
            if (_time.asMilliseconds() >= 38.0f && !_isHit)
            {
                if (_ghostSound.getStatus() == sf::Sound::Stopped && !_isHit)
                    _ghostSound.play();

                _pacman.move(_dir, _tileMap);

                for (int i = 0; i < 4; i++)
                    if (!_ghost[i].getIsStatic())
                    {
                        if (ghostMod <= 3000 || _ghost[i].getKillable())
                        {
                            if (!_ghost[i].getIsDead())
                                _ghost[i].move(i, _tileMap);
                            else
                                _ghost[i].hunt(i, _tileMap, 13, 11);
                        }
                        else if (ghostMod > 3000)
                        {
                            if (!_ghost[i].getIsDead())
                                _ghost[i].hunt(i, _tileMap, cellX, cellY);
                            else
                                _ghost[i].hunt(i, _tileMap, 13, 11);
                        }
                    }

                _cptPoint++;

                SPGEated = _pacman.eat(_tileMap, _window, _score, cptPacDotEated);
                if (SPGEated)
                {
                    _killClk.restart();
                    SPGEated = 0;
                    killRow = 0;
                    for (int i = 0; i < 4; i++)
                        _ghost[i].setKillable(1);
                }

                if (crntPacmanStatus != _pacman.getCanKill() && _pacman.getCanKill())
                {
                    _ghostSound.setBuffer(_ghostMoveFear);
                    //_ghostSound.setPitch(1.0f);
                }

                // Collison
                for (int i = 0; i < 4; i++)
                    if (_ghost[i].getIsDead() && _ghost[i].getGhostShape().getPosition().y == 330 && (_ghost[i].getGhostShape().getPosition().x > 224 || _ghost[i].getGhostShape().getPosition().x < 384))
                    {
                        _ghost[i].setIsDead(0);
                        _ghost[i].setKillable(0);
                        _ghostMovingBack = false;
                        if (_pacman.getCanKill())
                            _ghostSound.setBuffer(_ghostMoveFear);
                        else
                            _ghostSound.setBuffer(_ghostMove);
                    }
                    else if (!_ghost[i].getKillable() && _pacman.getPacManShape().getGlobalBounds().intersects(_ghost[i].getGhostShape().getGlobalBounds()) && !_ghost[i].getIsDead())
                        _isHit = 1;
                    else if (_ghost[i].getKillable() && _pacman.getPacManShape().getGlobalBounds().intersects(_ghost[i].getGhostShape().getGlobalBounds()) && !_ghost[i].getIsDead())
                    {
                        _ghost[i].setIsDead(1);
                        _eatGhost.play();
                        _ghostMovingBack = true;
                        _ghostSound.setBuffer(_ghostMoveDead);
                        killRow++;
                        switch (killRow)
                        {
                        case 1:
                            _score += 200;
                            break;
                        case 2:
                            _score += 400;
                            break;
                        case 3:
                            _score += 800;
                            break;
                        case 4:
                            _score += 1600;
                            break;
                        }
                    }
                if (_pacman.getPacManShape().getGlobalBounds().intersects(_fruit[level - 1].getFruitShape().getGlobalBounds()))
                {
                    _fruit[level - 1].setPosition(636 - (cptFruitEated * 42 + 8), 824);
                    switch (level)
                    {
                    case 1:
                        _score += 100;
                        break;
                    case 2:
                        _score += 300;
                        break;
                    case 3:
                        _score += 500;
                        break;
                    case 4:
                        _score += 700;
                        break;
                    case 5:
                        _score += 1000;
                        break;
                    case 6:
                        _score += 1000;
                        break;
                    case 7:
                        _score += 2000;
                        break;
                    case 8:
                        _score += 5000;
                        break;
                    }
                    cptFruitEated++;
                    isEat = 1;
                    _vie++;
                }

                _clock.restart();
            }

            if (_isHit)
            {
                _ghostSound.stop();

                if (_time.asMilliseconds() >= 150.0f)
                {
                    _pacman.deathAnimation();
                    draw();

                    if (_pacman.getRectSprite().left == 681)
                    {
                        resetPG();
                        _vie--;
                        _isHit = 0;
                    }
                    _clock.restart();
                }
            }

            // Timer Super Pac-Gomme
            if (_pacman.getCanKill() && _killClk.getElapsedTime().asSeconds() >= 10)
            {
                _pacman.setCanKill(0);

                for (int i = 0; i < 4; i++)
                    _ghost[i].setKillable(0);

                if(!_ghostMovingBack)
                    _ghostSound.setBuffer(_ghostMove);

                killRow = 0;
            }

            // Timer Hunt | move
            if (ghostMod == 5000)
                ghostMod = 0;
            else
                ghostMod++;

            switch (cptPacDotEated)
            {
            case 70:
                _fruit[level - 1].setPosition(322, 474);
                _fruit[level - 1].setIsShowed(1);
                break;
            case 120:
                _ghostPitch = 1.25f;
                break;
            case 180:
                _ghostPitch = 1.5f;
                break;
            case 240:
                resetMap();
                _ghostPitch = 1.0f;
                cptPacDotEated = 0;
                if (!isEat)
                {
                    _fruit[level - 1].setIsShowed(0);
                    _fruit[level - 1].setPosition(0, 0);
                }
                isEat = 0;
                level++;
                break;
            }

            if (_pacman.getCanKill() || _ghostMovingBack)
                _ghostSound.setPitch(1.0f);
            else
                _ghostSound.setPitch(_ghostPitch);
            

            if (level == 9 || _vie == 0)
            {
                _finalScore = _score;
                resetGame();
                _gameStarted = 0;
                for (int i = 0; i < 8; i++)
                {
                    _fruit[i].setIsShowed(0);
                    _fruit[i].setPosition(0, 0);
                }
                break;
            }
        }
        draw();
    }
    exportScore();
}

void Game::draw()
{
    _window.clear(sf::Color::Black);

    drawTileMap();
    _window.draw(_backGround);
    for (int i = 0; i < 8; i++)
        if (_fruit[i].getIsShowed())
            _fruit[i].draw(_window);
    for (int i = 0; i < 4; i++)
        if (!_isHit)
            _ghost[i].draw(_window);
    _pacman.draw(_window);
    drawLife();
    drawScore();

    if (!_gameStarted)
        _window.draw(_ready);

    _window.display();
}

void Game::drawTileMap()
{
    for (int y = 0; y < 31; y++)
    {
        for (int x = 0; x < 28; x++)
        {
            if (!(_cptPoint > 6 && _tileMap[y][x].getTileType() == 3))
                _tileMap[y][x].draw(_window);
            else if (_cptPoint == 10)
                _cptPoint = 0;
        }
    }
}

void Game::resetPG()
{
    _pacman.setPosition(322, 618);
    for (int i = 0; i < 4; i++)
    {
        _ghost[i].setCrntDir(rand() % 2 + 2);
        _ghost[i].setKillable(0);
        _ghost[i].init(i);
    }
    _pacman.setIntRectLeft(489);
    _dir = NONE;
}

void Game::resetMap()
{
    std::ifstream fTileMap("TileMap.txt");
    sf::IntRect rectTemp(0, 0, 8, 8);

    _tileMap.clear();

    if (fTileMap)
    {
        int estMur;

        for (int y = 0; y < 31; y++)
        {
            _tileMap.push_back(std::vector<Tile>());

            for (int x = 0; x < 28; x++)
            {
                fTileMap >> estMur;
                _tileMap[y].push_back(Tile(x, y, estMur));

                switch (_tileMap[y][x].getTileType())
                {
                case 2:
                    rectTemp.left = 168;
                    rectTemp.top = 208;
                    _tileMap[y][x].getTile().setTexture(&_spriteSheet);
                    _tileMap[y][x].getTile().setTextureRect(rectTemp);
                    break;
                case 3:
                    rectTemp.left = 208;
                    rectTemp.top = 184;
                    _tileMap[y][x].getTile().setTexture(&_spriteSheet);
                    _tileMap[y][x].getTile().setTextureRect(rectTemp);
                    break;
                }
            }
        }
    }

    fTileMap.close();
    resetPG();
    _cptPoint = 0;
}

void Game::resetGame()
{
    resetMap();

    _vie = 3;
    _score = 0;
    _ghostPitch = 1.0f;
    _ghostClock.restart();
}

void Game::drawLife()
{
    sf::RectangleShape pacVie(sf::Vector2f(32, 32));
    pacVie.setTexture(&_spriteSheet);

    sf::IntRect rectTemp(474, 17, 13, 13);
    pacVie.setTextureRect(rectTemp);

    for (int i = 0; i < _vie; i++)
    {
        pacVie.setPosition(sf::Vector2f(22 + i * 45, 830));
        _window.draw(pacVie);
    }
}

void Game::drawScore()
{
    std::string scoreString = std::to_string(_score);
    _scoreText.setString(scoreString);

    sf::FloatRect TextW = _scoreText.getLocalBounds();
    float xPos = (_window.getSize().x - TextW.width) / 2;
    _scoreText.setPosition(xPos, 40);

    sf::Text highScore("High Score", _font, 24);
    TextW = highScore.getLocalBounds();
    xPos = (_window.getSize().x - TextW.width) / 2;
    highScore.setPosition(xPos, 10);

    _window.draw(highScore);
    _window.draw(_scoreText);
}