#pragma once
#include <unordered_map>
#include "piece.h"
#include <vector>

using std::unordered_map;
using std::vector;


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

bool operator==(const Location& x, const Location& y);


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
			return l.x + l.z * 10 + l.y * 100;
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
	vector<Location> adjecent(Location l);
	vector<Location> slideCW(Location l);
	vector<Location> slideCCW(Location l);
	void add(Location, Piece);
	bool pinned(Location l);
	bool surrounded(Location l);
	bool trapped(Location);
};

