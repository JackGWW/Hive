// Hive.cpp : This file contains the 'main' function. Program execution begins and ends there.

#include "Hive.h"

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
	
	Board b;
	Piece wAnt = Piece(white, "A");
	Piece bQueen = Piece(black, "QUEEN");
	Grasshopper wHopper = Grasshopper(white);
	
	Location test = Location(0, 0, 0);
	vector<Location> testv = test.adjecent();
	
	cout << "Welcome to Hive!\n";

	printLocations(testv);

	cout << "DONE" << endl;
}

void printLocations(vector<Location> locations)
{
	Board b;
	Piece testPiece = Piece("test", "TEST");

	//Add pieces to board
	for (Location l : locations) {
		try {
			b.add(l, testPiece);
		}
		catch (std::invalid_argument& e) {
			cout << "Exception when add pieces to board:" << endl;
			cout << e.what() << endl;
		}
		cout << l.to_string() << endl;
	}

	b.print();
}