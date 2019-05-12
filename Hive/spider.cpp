#include "spider.h"


Spider::Spider(string _color)
{
	color = _color;
	name = "SPIDER";
}


Spider::~Spider()
{
}


vector<Location> Spider::moves(const Board & b, Location curLoc)
{
	return b.slide(curLoc, 3);
}