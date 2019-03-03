#include "Grasshopper.h"


Grasshopper::Grasshopper(string _color)
{
	color = _color;
	name = "HOP";
}


Grasshopper::~Grasshopper()
{
}

vector<Location> Grasshopper::moves(Board b, Location cur) {
	vector<Location> destinations;
	vector<Location> adj = cur.adjecent();

	//TODO add adjecent function to board
	//TODO get all adjecent pieces from board passing in location

	Location next;
	int xStep, yStep;
	for (Location l : adj) {
		if (b.exists(l)) {
			next = l;
			xStep = l.x - cur.x;
			yStep = l.y - cur.y;
			
			while (b.exists(next)) {
				next.move(xStep, yStep);
			}
			destinations.push_back(next);
		}
	}
	return destinations;
}

//Get all valid places it can move

//Check if proposed moved is valid

//