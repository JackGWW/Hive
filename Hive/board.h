#pragma once
#include <unordered_map>
#include "piece.h"
#include "location.h"
#include <vector>
#include <set>

using std::unordered_map;
using std::vector;
using std::set;
using std::string;


class Board
{
private:
	unordered_map<Location, Piece> pieces;
	unordered_map<string, bool> queenPlayed;
public:
	Board();
	~Board();
	Piece getPiece(const Location & l);
	bool onTop(const Location & l);
	bool onlyTouches(const Location & l, const string & color);
	vector<Location> getColoredPieces(const string & color);
	vector<Location> getPlacementLocations(string color);
	void move(Location origin, Location destination);
	Piece remove(Location l);
	vector<Location> getMoveablePieces(string color);
	void print();
	void print(vector<Location> numberedLocations);
	void add(const Location & l, const Piece & p);
	bool exists(Location);
	vector<Location> adjecent(Location);
	vector<Location> slide(Location curLoc, int moves);
	vector<Location> slide(Location curLoc);
	vector<Location> groundedAdjecent(Location l);
	vector<Location> slideOnTop(Location curLoc);
	vector<Location> slideCW(Location);
	vector<Location> slideCCW(Location);
	bool isAdjecent2D(Location start, Location end);
	bool canSlide(Location start, Location end);
	vector<Location> descend(Location curLoc);
	Location top(Location curLoc);
	vector<Location> climb(Location curLoc);
	bool pinned(Location);
	bool covered(Location l);
	bool surrounded(Location);
	bool trapped(Location);
};

string locationIndex(vector<Location> vecOfElements, Location element);
