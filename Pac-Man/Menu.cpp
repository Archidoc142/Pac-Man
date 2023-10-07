#include "Menu.h"

Menu::Menu()
{
    if (!(_bgTexture.loadFromFile("Sprite/AccueilPacman.png")
        && _font.loadFromFile("Sprite/Minecraft.ttf")
        && _tutoTexture.loadFromFile("Sprite/tuto.png")
        && _clickBuffer.loadFromFile("Sound/clicSound.ogg")
        ))
    {
        exit(-1);
    }

    _bg.setTexture(&_bgTexture);
    _tuto.setTexture(&_tutoTexture);

    _bg.setSize(sf::Vector2f(672, 888));
    _tuto.setSize(sf::Vector2f(672, 888));

    _click.setBuffer(_clickBuffer);

    for (int i = 0; i < 3; i++)
    {
        _button[i].setFillColor(sf::Color::White);
        _button[i].setSize(sf::Vector2f(200, 60));
        _button[i].setPosition(15 + (i * 220), 450);
        _button[i].setOutlineColor(sf::Color::Red);
        _button[i].setOutlineThickness(5);

        _text[i].setString(_textStr[i]);
        _text[i].setFont(_font);
        _text[i].setCharacterSize(24);
        _text[i].setStyle(sf::Text::Bold);
        _text[i].setFillColor(sf::Color::Black);
        sf::FloatRect text_bounds = _text[i].getLocalBounds();
        _text[i].setOrigin(text_bounds.left + text_bounds.width / 2.0f, text_bounds.top + text_bounds.height / 2.0f);
        _text[i].setPosition(sf::Vector2f(_button[i].getPosition().x + _button[i].getSize().x / 2.0f, (_button[i].getPosition().y + _button[i].getSize().y / 2.0)));
    }

    _button[3].setFillColor(sf::Color::White);
    _button[3].setSize(sf::Vector2f(200, 60));
    _button[3].setPosition(240, 800);
    _button[3].setOutlineColor(sf::Color::Red);
    _button[3].setOutlineThickness(5);

    _text[3].setString(_textStr[3]);
    _text[3].setFont(_font);
    _text[3].setCharacterSize(24);
    _text[3].setStyle(sf::Text::Bold);
    _text[3].setFillColor(sf::Color::Black);
    sf::FloatRect text_bounds = _text[3].getLocalBounds();
    _text[3].setOrigin(text_bounds.left + text_bounds.width / 2.0f, text_bounds.top + text_bounds.height / 2.0f);
    _text[3].setPosition(sf::Vector2f(_button[3].getPosition().x + _button[3].getSize().x / 2.0f, (_button[3].getPosition().y + _button[3].getSize().y / 2.0)));

    _highScoreTxt.setFont(_font);
    _highScoreTxt.setStyle(sf::Text::Bold);
    _highScoreTxt.setCharacterSize(36);
    _highScoreTxt.setFillColor(sf::Color::White);
}

int Menu::showMenu(sf::RenderWindow& win, const int score)
{
    if (score != 0)
    {
        std::string highScoreStr = "HIGH SCORE: " + std::to_string(score);
        _highScoreTxt.setString("HIGH SCORE: " + std::to_string(score));
        _highScoreTxt.setPosition(336 - _highScoreTxt.getGlobalBounds().width / 2, 270);
    }

    sf::Event ev;
    while (win.pollEvent(ev))
    {
        if (ev.type == sf::Event::MouseButtonPressed)
        {
            for (int i = 0; i < 4; i++)
            {
                if (_button[i].getGlobalBounds().contains(ev.mouseButton.x, ev.mouseButton.y))
                {
                    if (i < 3 && !_isTuto || i == 3 && _isTuto)
                    {
                        _click.play();

                        switch (i)
                        {
                        case 0:
                            return 1;
                            break;
                        case 1:
                            _isTuto = true;
                            break;
                        case 2:
                            return 2;
                            break;
                        case 3:
                            _isTuto = false;
                        }
                    }
                }
                else
                {
                    _button[i].setFillColor(sf::Color::White);
                }

            }
        }
    }

    draw(win, score);
    return 0;
}

void Menu::draw(sf::RenderWindow& win, const int score)
{
    win.clear(sf::Color::Black);
    if (!_isTuto)
    {
        win.draw(_bg);
        for (int i = 0; i < 3; i++)
        {
            win.draw(_button[i]);
            win.draw(_text[i]);
        }
        if (score != 0)
        {
            win.draw(_highScoreTxt);
        }
    }
    else
    {
        win.draw(_tuto);
        win.draw(_button[3]);
        win.draw(_text[3]);
    }


    win.display();
}