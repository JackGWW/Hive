#pragma once
#include "piece.h"
#include "board.h"
#include <string>
#include <vector>

using std::vector;
using std::string;

class Grasshopper :
	public Piece
{
public:
	Grasshopper(string);
	vector<Location> moves(Board, Location);
	~Grasshopper();
};

