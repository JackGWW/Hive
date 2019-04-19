#include "queen.h"

Queen::Queen(string _color)
{
	color = _color;
	name = "QUEEN";
}

vector<Location> Queen::moves(Board b, Location curLoc)
{
	return b.slide(curLoc, 1);
}

Queen::~Queen()
{
}
