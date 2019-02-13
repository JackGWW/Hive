// Hive.cpp : This file contains the 'main' function. Program execution begins and ends there.

#include "pch.h"
#include "piece.h"
#include "board.h"
#include <iostream>
#include <stdlib.h>
#include <string>

using std::cout;
using std::endl;
using std::string;

int main()
{
	cout << "Welcome to Hive!\n";

	Board b;

	Piece wAnt = Piece("white", "ANT");
	Piece bQueen = Piece("black", "QUEEN");

	try {
		b.add(Location(0, 0, 0), wAnt);
		b.add(Location(0, 6, 0), bQueen);
	}
	catch (std::invalid_argument& e) {
		cout << "Exception when add pieces to board:" << endl;
		cout << e.what() << endl;
	}
	
	bQueen.print();
	b.print();

}