#pragma once
#include "piece.h"
#include "board.h"
#include <string>
#include <vector>

using std::vector;
using std::string;

class Spider :
	public Piece
{
public:
	Spider(string);
	vector<Location> moves(Board, Location);
	~Spider();
};
