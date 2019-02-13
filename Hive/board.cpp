#include "pch.h"
#include "board.h"
#include "piece.h"
#include <stdlib.h>
#include <iostream>
#include <stdlib.h>
#include <string>
#include <unordered_map>


using std::cout;
using std::endl;
using std::string;
using std::unordered_map;

Location::Location(int _x, int _y, int _z) : x(_x), y(_y), z(_z)
{

}

string Location::to_string()
{
	string str = "";
	str += "x: " + std::to_string(x) + "  ";
	str += "y: " + std::to_string(y) + "  ";
	str += "z: " + std::to_string(z);

	return str;
}

bool operator==(const Location& x, const Location& y)
{
	return x.x == y.x && x.y == y.y;
}



bool keyExists(unordered_map<Location, Piece> m, Location key)
{
	if (m.find(key) == m.end())
		return false;

	return true;
}


Board::Board()
{
	unordered_map<Location, Piece> pieces;
}


Board::~Board()
{
}

void Board::print()
{
	int maxX = INT_MIN, maxY = INT_MIN;
	int minX = INT_MAX, minY = INT_MAX;

	for (auto& it : pieces) {
		Location l = it.first;

		if (l.x > maxX) maxX = l.x;
		if (l.y > maxY) maxY = l.y;
		if (l.x < minX) minX = l.x;
		if (l.y < minY) minY = l.y;
	}
	cout << maxX << ',' << maxY << ' ' << minX << ',' << minY << endl;
}

void Board::add(Location l, Piece p)
{
	if (keyExists(pieces, l))
	{
		throw std::invalid_argument("There is already a piece in location: " + l.to_string());
	}

	pieces.insert(std::make_pair(l, p));
}
