#pragma once
#include <unordered_map>
#include "piece.h"

using std::unordered_map;



struct Location
{
	int x, y, z;
	Location(int, int, int);
	string to_string();
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
	void add(Location, Piece);
};

