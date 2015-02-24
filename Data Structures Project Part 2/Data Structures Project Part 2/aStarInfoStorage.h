#ifndef ASTARINFOSTORAGE_H
#define ASTARINFOSTORAGE_H

#include <string>
#include "stdafx.h"

using namespace std;

class aStarInfoStorage
{
public:
	aStarInfoStorage();

	//nodeName, costSoFar, heuristic, position to be drawn at
	aStarInfoStorage(string n, float c, float h, sf::Vector2f pos);

	~aStarInfoStorage();

	string getNodeName();

	float getHeuristicValue();

	float getCostSoFar();

	sf::Vector2f getPosition();

	void setNodeName(string na);

	void setHeuristicValue(float he);

	void setCostSoFar(float co);

	void setPosition(sf::Vector2f pos);
private:
	string nodeName;
	float heuristicValue;
	float costSoFar;
	sf::Vector2f position;
};

#endif