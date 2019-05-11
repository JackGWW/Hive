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
	vector<Location> getPlacementLocations(const string & color);
	void move(Location origin, Location destination);
	Piece remove(Location l);
	vector<Location> getMoveablePieces(string color);
	void print();
	void print(vector<Location> numberedLocations);
	void add(const Location & l, const Piece & p);
	bool exists(const Location & l);
	vector<Location> adjecent(const Location & l);
	vector<Location> slide(const Location & curLoc, int moves);
	vector<Location> slide(const Location & curLoc);
	vector<Location> groundedAdjecent(const Location & l);
	vector<Location> slideOnTop(const Location & curLoc);
	vector<Location> slideCW(const Location & curLoc);
	vector<Location> slideCCW(const Location & curLoc);
	bool isAdjecent2D(const Location & start, const Location & end);
	bool canSlide(const Location & end, const Location & start);
	vector<Location> descend(const Location & curLoc);
	Location top(Location curLoc);
	vector<Location> climb(const Location & curLoc);
	bool pinned(const Location & l);
	bool covered(Location l);
	bool surrounded(const Location & l);
	bool trapped(const Location & l);
};

string locationIndex(const vector<Location> & allLocations, const Location & specificLocation);
