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
using std::string;
void printLocations(vector<Location>);


class Game
{
private:
	vector<string> names;
	int curPlayer = 0;
	int turnCount = 0;
	vector<vector<Piece>> unusedPieces;
	Board board;
	string white = "white";
	string black = "black";
	vector<string> colors{ white, black };
	bool gameOver = false;
public:
	Game(string, string);
	~Game();
	void play();
	void nextTurn();
	void playPiece(Piece p);
	void movePiece(Location l);
	void playTurn();
	void printUnusedPieces(int player, int offset=0);
	void addStartingPieces();
};