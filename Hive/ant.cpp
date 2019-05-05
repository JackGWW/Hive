#include "ant.h"



Ant::Ant(string _color)
{
	color = _color;
	name = "ANT";
}

vector<Location> Ant::moves(Board b, Location curLoc)
{
	return b.slide(curLoc);
}

Ant::~Ant()
{
}
