#pragma once
#include "piece.h"
#include "board.h"
#include <string>
#include <vector>

using std::vector;
using std::string;

class Ant :
	public Piece
{
public:
	Ant(string);
	vector<Location> moves(const Board &, Location);
	~Ant();
};

