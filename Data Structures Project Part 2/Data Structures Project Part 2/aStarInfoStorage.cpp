#include"aStarInfoStorage.h"
#include "stdafx.h"

aStarInfoStorage::aStarInfoStorage()
{//blank default constructor
}

//nodeName, costSoFar, heuristic and position to be drawn at are passed in and stored
aStarInfoStorage::aStarInfoStorage(string n, float c, float h, sf::Vector2f pos) : nodeName(n), costSoFar(c), heuristicValue(h), position(pos)
{
}

aStarInfoStorage::~aStarInfoStorage()
{//blank destructor
}

//gets for nodeName, costSoFar, heuristic, position to be drawn at
#pragma region Gets
string aStarInfoStorage::getNodeName(){ return nodeName; }

float aStarInfoStorage::getHeuristicValue(){ return heuristicValue; }

float aStarInfoStorage::getCostSoFar(){ return costSoFar; }

sf::Vector2f aStarInfoStorage::getPosition(){ return position; }
#pragma endregion

//sets for nodeName, costSoFar, heuristic, position to be drawn at
#pragma region Sets
void aStarInfoStorage::setNodeName(string na){ nodeName = na; }

void aStarInfoStorage::setHeuristicValue(float he){ heuristicValue = he; }

void aStarInfoStorage::setCostSoFar(float co) { costSoFar = co; }

void aStarInfoStorage::setPosition(sf::Vector2f pos){ position = pos; }
#pragma endregion