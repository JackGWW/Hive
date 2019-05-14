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
void testMoves();
void printLocations(vector<Location>);


class Game
{
private:
	vector<string> names;
	int curPlayer = 0;
	int turnCount = 0;
	vector<vector<unique_ptr<Piece>>> unusedPieces;
	Board board;
	string white = "white";
	string black = "black";
	vector<string> colors{ white, black };
	int winner = -1;
public:
	Game(string, string);
	~Game();
	void play();
	void nextTurn();
	void playPiece(unique_ptr<Piece> p);
	void movePiece(Location l);
	void playTurn();
	int playerTurn(int player);
	bool mustPlayQueen(int player);
	int countPlayablePieces(int player);
	void printUnusedPieces(int player, int offset=0);
	void addStartingPieces();
};