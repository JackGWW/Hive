#pragma once
#include <string>
#include <vector>
#include "location.h"
//#include "boost\noncopyable.hpp"

using std::vector;
using std::string;

class Board;

class Piece //: boost::noncopyable
{
private:
public:
	string name;
	string color;
	string paddedColor();
	string paddedName();
	Piece(string _color, string _name);
	Piece();
	~Piece();
	void print();
	virtual bool isQueen();
	virtual vector<Location> moves(const Board &, Location);
};
