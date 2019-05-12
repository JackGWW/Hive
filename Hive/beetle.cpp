#include "beetle.h"



Beetle::Beetle(string _color)
{
	color = _color;
	name = "BEETLE";
}

vector<Location> Beetle::moves(const Board & b, Location curLoc)
{
	vector<Location> potentialMoves;
	vector<Location> toAdd;

	toAdd = b.slide(curLoc, 1);
	for (Location l : toAdd) {
		potentialMoves.push_back(l);
	}

	toAdd = b.climb(curLoc);
	for (Location l : toAdd) {
		potentialMoves.push_back(l);
	}

	toAdd = b.descend(curLoc);
	for (Location l : toAdd) {
		potentialMoves.push_back(l);
	}
	return potentialMoves;
}


Beetle::~Beetle()
{
}
