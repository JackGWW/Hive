#pragma once

#include "board.h"
#include "pch.h"
#include "piece.h"
#include "Grasshopper.h"
#include "ant.h"
#include "spider.h"
#include "beetle.h"
#include "queen.h"
#include "board.h"
#include <vector>
#include <iostream>
#include <stdlib.h>
#include <string>

using std::vector;

void printLocations(vector<Location>);


class Game
{
private:
	unordered_map<string, string> names;
	unordered_map<string, vector<Piece>> unusedPieces;
	Board board;
public:
	Game(string, string);
	~Game();
	void addStartingPieces();
};