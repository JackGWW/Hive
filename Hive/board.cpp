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
							if (exists(Location(x - 1, y + 1))) {
								cout << "/    ";						
							}
							else {
								cout << " /    ";						
							}
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
							if (exists(Location(x - 1, y))) {
								cout << "\\";
							}
							else {
								cout << " \\";
							}
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


void Board::add(Location l, Piece p)
{
	if (exists(l)){
		throw std::invalid_argument("There is already a piece in location: " + l.to_string());
	}

	pieces.insert(std::make_pair(l, p));
}

//TODO implement the pinned function
bool Board::pinned(Location l)
{
	//can't moved because it would break the hive
	return false;
}

bool Board::surrounded(Location l) 
{	
	//Can't physically move
	//If there are two consecutive empty adjecent spaces, it isn't surrounded
	vector<Location> adj = l.adjecent();
	for (int i = 0; i < 6; i++) { 
		if (!exists(adj[i]) && !exists(adj[(i + 1) % 6])) {
			return false;
		}
	}
	return true;
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

vector<Location> Board::slide(Location curLoc, int moves)
{
	if (moves <= 0) {
		throw std::invalid_argument("Number of moves must be greater than 0");
	}
	
	vector<Location> destinations;

	// Find all destinations sliding clockwise
	vector<Location> curLocations;
	vector<Location> nextLocations;
	vector<Location> nextLoc;
	curLocations.push_back(curLoc);
	for (int i = 0; i < moves; i++) {
		//Get next location for every current location
		for (Location l : curLocations) {
			nextLoc = slideCW(l);
			nextLocations.insert(nextLoc.end(), nextLoc.begin(), nextLoc.end());
		}
		curLocations = nextLocations;
		nextLocations.clear();
	}
	destinations.insert(curLocations.end(), curLocations.begin(), curLocations.end());

	// Find all destinations sliding counter-clockwise
	curLocations.clear();
	nextLocations.clear();
	nextLoc.clear();
	curLocations.push_back(curLoc);
	for (int i = 0; i < moves; i++) {
		//Get next location for every current location
		for (Location l : curLocations) {
			nextLoc = slideCCW(l);
			nextLocations.insert(nextLoc.end(), nextLoc.begin(), nextLoc.end());
		}
		curLocations = nextLocations;
		nextLocations.clear();
	}
	destinations.insert(curLocations.end(), curLocations.begin(), curLocations.end());

	return destinations;
}

//TODO refactor slideCCW and slideCW to have less duplicated code
vector<Location> Board::slideCCW(Location curLoc)
{
	if (trapped(curLoc)){
		throw std::invalid_argument("Piece cannot be moved");
	}
	vector<Location> adj = curLoc.adjecent();
	vector<Location> nextLoc;

	//iterate through all six adjecent pieces
	for (int i = 0; i < 6; i++) {
		//to be able to slide, there needs to be a two piece gap after a piece
		if (exists(adj[(i + 5) % 6]) && !exists(adj[i]) && !exists(adj[(i + 1) % 6])) {
			nextLoc.push_back(adj[i]);
		}
	}
	return nextLoc;
}

vector<Location> Board::slideCW(Location curLoc)
{
	if (trapped(curLoc)) {
		throw std::invalid_argument("Piece cannot be moved");
	}
	vector<Location> adj = curLoc.adjecent();
	vector<Location> nextLoc;

	//iterate through all six adjecent pieces
	for (int i = 0; i < 6; i++) {
		//to be able to slide, there needs to be a two piece gap after a piece
		if (exists(adj[(i + 1) % 6]) && !exists(adj[i]) && !exists(adj[(i + 5) % 6])) {
			nextLoc.push_back(adj[i]);
		}
	}
	return nextLoc;
}