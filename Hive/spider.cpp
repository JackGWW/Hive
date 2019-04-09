#include "spider.h"


Spider::Spider(string _color)
{
	color = _color;
	name = "SPIDR";
}


Spider::~Spider()
{
}


vector<Location> Spider::moves(Board b, Location curLoc)
{
	return b.slide(curLoc, 3);
}