// Hive.cpp : This file contains the 'main' function. Program execution begins and ends there.

#include "pch.h"
#include "piece.h"
#include "Grasshopper.h"
#include "board.h"
#include <iostream>
#include <stdlib.h>
#include <string>

using std::cout;
using std::endl;
using std::string;

// TODO add in proper public/private usage
// TODO add in const usage
// TODO add virtual classes and virtual desctructor to piece

int main()
{
	string white = "white";
	string black = "black";
	
	cout << "Welcome to Hive!\n";

	Board b;

	Piece wAnt = Piece(white, "A");
	Piece bQueen = Piece(black, "QUEEN");
	Grasshopper wHopper = Grasshopper(white);

	Location test = Location(0, 0, 0);
	
	vector<Location> testv = test.adjecent();
	for (Location l : testv) {
		try {
			b.add(l, wHopper);
		}
		catch (std::invalid_argument& e) {
			cout << "Exception when add pieces to board:" << endl;
			cout << e.what() << endl;
		}
		cout << l.to_string() << endl;
	}

	b.print();

	cout << "DONE" << endl;
}