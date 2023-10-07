#pragma once
class Tile
{
private:
	sf::RectangleShape _tile;
	int _type;
	int _parent[2];
	bool _visited = 0;
	bool _path = 0;

public:

	Tile(int x, int y, int mur);

	int getTileType() const;
	int getParent(int i) const;
	bool getVisited()const;
	bool getPath()const;
	sf::RectangleShape& getTile();

	void setTileType(int type);
	void setVisited(const bool visited);
	void setParent(const int x, const int y);
	void setPath(const bool path);

	void draw(sf::RenderWindow& window);
};