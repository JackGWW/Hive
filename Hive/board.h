#pragma once
#include <unordered_map>
#include "location.h"
#include <vector>
#include <set>
#include <utility>
#include <memory>

using std::unordered_map;
using std::vector;
using std::unique_ptr;
using std::set;
using std::string;

class Piece;

class Board
{
private:
	unordered_map<Location, unique_ptr<Piece>> pieces;
	unordered_map<string, bool> queenPlayed;
public:
	Board();
	~Board();
	bool getQueenPlayed(string color);
	bool onTop(const Location & l);
	bool onlyTouches(const Location & l, const string & color);
	vector<Location> getColoredPieces(const string & color);
	vector<Location> getPlacementLocations(const string & color);
	vector<Location> getMovementLocations(const Location &);
	void movePiece(Location origin, Location destination);
	unique_ptr<Piece> remove(Location l);
	vector<Location> getMoveablePieces(string color);
	void print();
	void print(vector<Location> numberedLocations);
	void addPiece(const Location & l, unique_ptr<Piece> p);
	bool exists(const Location & l) const;
	bool exists2D(const Location & l) const;
	vector<Location> adjecentPieces(const Location & l) const;
	vector<Location> slide(const Location & curLoc, int moves) const;
	vector<Location> slide(const Location & curLoc) const;
	vector<Location> groundedAdjecentLocations(const Location & l) const;
	vector<Location> slideOnTop(const Location & curLoc) const;
	vector<Location> slideCW(const Location & curLoc) const;
	vector<Location> slideCCW(const Location & curLoc) const;
	bool isAdjecent2D(const Location & start, const Location & end) const;
	bool canSlide(const Location & end, const Location & start) const;
	vector<Location> descend(const Location & curLoc) const;
	Location locationOnTop(Location curLoc) const;
	Location topPieceLocation(Location curLoc) const;
	vector<Location> climb(const Location & curLoc) const;
	bool pinned(const Location & l) const;
	bool covered(Location l) const;
	bool surrounded(const Location & l) const;
	bool trapped(const Location & l) const;
};

std::pair<bool, int> findInVector2D(const std::vector<Location>& vecOfElements, const Location & element);

string locationIndex(const vector<Location> & allLocations, const Location & specificLocation);

string locationIndex2D(const vector<Location>& allLocations, const Location & specificLocation);

string intToString(int number);
