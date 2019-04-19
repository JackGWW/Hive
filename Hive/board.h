#pragma once
#include <unordered_map>
#include "piece.h"
#include <vector>
#include <set>

using std::unordered_map;
using std::vector;
using std::set;


struct Location
{
	int x, y, z;
	Location(int, int, int);
	Location(int, int);
	Location();
	string to_string();
	vector<Location> adjecent();
	void move(int, int, int);
	void move(int, int);
};

inline bool operator==(const Location& lhs, const Location& rhs) { return lhs.x == rhs.x && lhs.y == rhs.y && lhs.z == rhs.z; }
inline bool operator!=(const Location& lhs, const Location& rhs) { return !operator==(lhs, rhs); }
inline bool operator< (const Location& lhs, const Location& rhs) { return (lhs.x + lhs.y * 10 + lhs.z * 100) < (rhs.x + rhs.y * 10 + rhs.z * 100); }
inline bool operator> (const Location& lhs, const Location& rhs) { return  operator< (rhs, lhs); }
inline bool operator<=(const Location& lhs, const Location& rhs) { return !operator> (lhs, rhs); }
inline bool operator>=(const Location& lhs, const Location& rhs) { return !operator< (lhs, rhs); }

namespace std {

	template <>
	struct hash<Location>
	{
		std::size_t operator()(const Location& l) const
		{
			using std::size_t;
			using std::hash;
			using std::string;

			//x & y range from -50 to 50, z ranges from 0-10
			return l.x + l.y * 10 + l.z * 100;
		}
	};
}


class Board
{
private:
	unordered_map<Location, Piece> pieces;
public:
	Board();
	~Board();
	void print();
	bool exists(Location);
	vector<Location> adjecent(Location);
	vector<Location> slide(Location curLoc, int moves);
	vector<Location> slide(Location curLoc);
	vector<Location> slideOnTop(Location curLoc);
	vector<Location> slideCW(Location);
	vector<Location> slideCCW(Location);
	bool isAdjecent2D(Location start, Location end);
	bool canSlide(Location start, Location end);
	vector<Location> descend(Location curLoc);
	Location top(Location curLoc);
	vector<Location> climb(Location curLoc);
	void add(Location, Piece);
	bool pinned(Location);
	bool surrounded(Location);
	bool trapped(Location);
};

