#include <SFML/Graphics.hpp>

#include "Tile.h"

Tile::Tile(int x, int y, int mur)
{
	_tile.setSize(sf::Vector2f(24, 24));
	_tile.setPosition(sf::Vector2f(x*24, y*24 + 72));
	_type = mur;

	switch (mur)
	{
	case 0:
	case 4:
	case 5:
		_tile.setFillColor(sf::Color::Black);
		break;
	case 1:
		_tile.setFillColor(sf::Color::Transparent);
		break;
	}
}

int Tile::getTileType() const
{
	return _type;
}

int Tile::getParent(int i) const
{
	return _parent[i];
}

bool Tile::getVisited() const
{
	return _visited;
}

bool Tile::getPath() const
{
	return _path;
}

sf::RectangleShape& Tile::getTile()
{
	return _tile;
}

void Tile::setTileType(int type)
{
	_type = type;

	if (_type == 0)
		_tile.setFillColor(sf::Color::Black);
}

void Tile::setVisited(const bool visited)
{
	_visited = visited;
}

void Tile::setParent(const int x, const int y)
{
	_parent[0] = x;
	_parent[1] = y;
}

void Tile::setPath(const bool path)
{
	_path = path;
}

void Tile::draw(sf::RenderWindow& window)
{
	window.draw(_tile);
}