#pragma once
#include "piece.h"
#include "board.h"
#include <string>
#include <vector>

using std::vector;
using std::string;

class Beetle :
	public Piece
{
public:
	Beetle(string);
	vector<Location> moves(const Board & b, Location);
	~Beetle();
};

