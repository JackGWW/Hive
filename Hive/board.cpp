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


string Location::to_string()
{
	string str = "";
	str += "x: " + std::to_string(x) + "  ";
	str += "y: " + std::to_string(y) + "  ";
	str += "z: " + std::to_string(z);

	return str;
}

vector<Location> Location::adjecent()
{
	vector<Location> adj;
    adj.push_back(Location(x, y + 2, z));
    adj.push_back(Location(x + 1, y + 1, z));
    adj.push_back(Location(x + 1, y - 1, z));
    adj.push_back(Location(x, y - 2, z));
    adj.push_back(Location(x -1, y - 1, z));
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

bool operator==(const Location& x, const Location& y)
{
	return x.x == y.x && x.y == y.y;
}


//void printBoardLine(unordered_map<Location, Piece> m, Location l, int line) 
//{
//	
//}

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

	cout << "PRINTING BOARD: MIN(" << minX << "," << minY << ") MAX(" << maxX << "," << maxY << ")" << endl;

	////print top border for first line of hexagons
	//for (int x = minX; x <= maxX; x ++) {
	//	if ((x + maxY) % 2 == 0) {
	//		if (keyExists(pieces, Location(x, maxY, 0))) {
	//			cout << "  ____";
	//		}
	//		else {
	//			cout << "      ";
	//		}
	//	}
	//	else {
	//		cout << "      ";
	//	}
	//	
	//}
	//cout << endl;

	//Iterate through every hexagonal spot on the board
	string color;
	string name;
	Location curLocation;
	for (int y = maxY + 1; y >= minY - 1; y--) {
		//Iterate through top 2 lines of the hexagon
		for (int line = 0; line < 2; line++) {
			for (int x = minX; x <= maxX + 1; x++) {
				if ((x + y) % 2 == 0) {
					curLocation = Location(x, y);
				}
				switch (line) {
				case 0:
					if ((y + x) % 2 == 0) {
						if (exists(curLocation) || exists(Location(x - 1, y + 1))) {
							cout << " /    ";
						}
						else {
							cout << "      ";
						}
					}
					else {
						if (exists(Location(x, y + 1))) {
							color = pieces[Location(x, y + 1)].paddedColor();
							cout << "\\" << color;
						}
						else if(exists(Location(x - 1, y ))) {
						    cout << "\\     ";
						} else {
							cout << "      ";
						}
					}
					break;
				case 1:
					if ((y + x) % 2 == 0) {
						if (exists(curLocation)) {
							name = pieces[curLocation].paddedName();
							cout << "/" << name;
						}
						else if (exists(Location(x - 1, y + 1))) {
							cout << "/     ";
						}
						else {
							cout << "      ";
						}
					}
					else {
						if (exists(Location(x, y + 1)) || exists(Location(x - 1, y))) {
							cout << " \\";
						}
						else {
							cout << "  ";
						}
						if (exists(Location(x, y + 1)) || exists(Location(x, y -1))) {
							cout << "____";
						}
						else {
							cout << "    ";
						}

					}
					break;
				}
			}
			cout << endl;
		}
	}
}
//-1:"  "____"      "
// 0:" "/"    "\"     "
// 1:"/"      "\"____"
//
//        /    \      /    \      /    \  //if y & maxY == 0 
//	   /      \____/      \____/      \ 
////6 spa\      /    \      /    \      /   //if y & maxY == 1 
////6 spac\____/      \____/      \____/      
//	
//
//Line 0:  ' ' 
//	check if (leftdown or current) "\" 
//	check if (bottom or current) "____"
//	check if (rightdown or current) "/ "
//Line 1:
//	check "  '"
//	check if (leftdown or current) "\"
//	"    "
//	check if rightdown or current "/"
//
//	line printer is same,
//	only question of where it starts and whether or not to include the botoom
//
//
//
//  ____
// /    \
///-1, 1 \____	always start printing close
//\	     /	  \then print far (add the oppose from inside to the other 
// \____/	0,0\____alternating going up and down the sides)
// /	\      /    \
///-1,-1 \____/ 1,-1 \
//\	     /    \      /
// \____/	   \____/
//	if row is different even/odd of highest, 
//		print four spaces to get offset required
//	for each one, print lside rside down
//	/*cout << "  ____" << endl;
//	cout << " /    \\" << endl;
//	cout << "/" << name << "\\" << endl;
//	cout << "\\" << color << "/" << endl;
//	cout << " \\____/" << endl;
//
//	cout << maxX << ',' << maxY << ' ' << minX << ',' << minY << endl; */
//}

void Board::add(Location l, Piece p)
{
	if (exists(l)){
		throw std::invalid_argument("There is already a piece in location: " + l.to_string());
	}

	pieces.insert(std::make_pair(l, p));
}

bool Board::pinned(Location l)
{
	//can't moved because it would break the hive
	return false;
}

bool Board::surrounded(Location l) 
{
	//can't physically move
	return false;
}

bool Board::trapped(Location l)
{
	return pinned(l) && surrounded(l);
}

bool Board::exists(Location l)
{
	if (pieces.find(l) == pieces.end())
		return false;
	
	return true;
}

vector<Location> Board::adjecent(Location l)
{	
	vector<Location> adjPieces;
	vector<Location> adjSpots = l.adjecent();

	for (Location spot : adjSpots)
	{
		if (exists(spot))
		{
			adjPieces.push_back(spot);
		}
	}

	return adjPieces;
}

Location Board::slideCW(Location l)
{
	if (trapped(l))
	{
		throw std::invalid_argument("Piece cannot be moved");
	}

	//where is it fre to slide 

}

Location Board::slideCCW(Location l)
{

}