#include "ant.h"



Ant::Ant(string _color)
{
	color = _color;
	name = "ANT";
	nameColor = 1; //Blue
}

vector<Location> Ant::moves(const Board & b, Location curLoc)
{
	return b.slide(curLoc);
}

Ant::~Ant()
{
}
