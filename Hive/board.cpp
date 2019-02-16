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

void printBoardLine(unordered_map<Location, Piece> m, Location l, int line) 
{
	
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

	//Find bounding coordinates of board
	for (auto& it : pieces) {
		Location l = it.first;

		if (l.x > maxX) maxX = l.x;
		if (l.y > maxY) maxY = l.y;
		if (l.x < minX) minX = l.x;
		if (l.y < minY) minY = l.y;
	}
	//Iterate through every hexagonal spot on the board
	for (int i = maxY; i >= minY; i--){
		//Iterate through all 4 lines of the hexagon
		for (int line = 0; line < 4; line++) {
			for (int j = minX; j <= maxX; j++) {
				
				switch (line) {
				case 0: cout << '0';
					break;
				case 1: cout << '1';
					break;
				case 2: cout << '2';
					break;
				case 3: cout << '3';
					break;
				}
				
				if (keyExists(pieces, Location(i, j, 0))) {
					cout << i << j << "- TRUE" << endl;
				}
				else {
					cout << "FALSE";
				}
			}
			cout << endl;
		}
	}


	/*cout << "  ____" << endl;
	cout << " /    \\" << endl;
	cout << "/" << name << "\\" << endl;
	cout << "\\" << color << "/" << endl;
	cout << " \\____/" << endl;

	cout << maxX << ',' << maxY << ' ' << minX << ',' << minY << endl; */
}

void Board::add(Location l, Piece p)
{
	if (keyExists(pieces, l)){
		throw std::invalid_argument("There is already a piece in location: " + l.to_string());
	}

	pieces.insert(std::make_pair(l, p));
}
