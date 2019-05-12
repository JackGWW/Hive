#include "location.h"
#include "location.h"
#include "pch.h"
#include "location.h"
#include <stdlib.h>
#include <iostream>
#include <stdlib.h>
#include <string>
#include <unordered_map>
#include <queue>
#include <set>


using std::cout;
using std::endl;
using std::string;


//TODO implement iterators for adjecent pieces

Location::Location(int _x, int _y, int _z)
{
	x = _x;
	y = _y;
	z = _z;

	if ((x + y) % 2 != 0) {
		throw std::invalid_argument("X & Y must be on the same offset (both odd or even)");
	}
	if (z < 0) {
		throw std::invalid_argument("Z cannot be negative: " + std::to_string(z));
	}

}
Location::Location(int _x, int _y)
{
	x = _x;
	y = _y;
	z = 0;

	if ((x + y) % 2 != 0) {
		throw std::invalid_argument("X & Y must be on the same offset (both odd or even)");
	}
}

Location::Location()
{
	x = 0;
	y = 0;
	z = 0;
}


string Location::to_string() const
{
	string str = "";
	str += "x: " + std::to_string(x) + "  ";
	str += "y: " + std::to_string(y) + "  ";
	str += "z: " + std::to_string(z);

	return str;
}

vector<Location> Location::adjecentLocations() const
{
	vector<Location> adj;
	adj.push_back(Location(x, y + 2, z));
	adj.push_back(Location(x + 1, y + 1, z));
	adj.push_back(Location(x + 1, y - 1, z));
	adj.push_back(Location(x, y - 2, z));
	adj.push_back(Location(x - 1, y - 1, z));
	adj.push_back(Location(x - 1, y + 1, z));

	return adj;
}

void Location::move(int _x, int _y, int _z)
{
	x += _x;
	y += _y;
	z += _z;
}

void Location::move(int _x, int _y)
{
	x += _x;
	y += _y;
}

Location Location::above() const
{
	return Location(x, y, z+1);
}
