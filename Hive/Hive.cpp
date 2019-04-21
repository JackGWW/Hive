// Hive.cpp : This file contains the 'main' function. Program execution begins and ends there.

#include "Hive.h"
#include <unordered_map>
#include <vector>

using std::unordered_map;
using std::cout;
using std::endl;
using std::vector;
using std::string;

// TODO add in proper public/private usage
// TODO add in const usage
// TODO add virtual classes and virtual desctructor to piece

string white = "white";
string black = "black";
vector<string> colors{ white, black };


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

	Game game = Game("Jack", "Thomas");

	cout << "DONE" << endl;
}

void printLocations(vector<Location> locations)
{
	Board b;
	Piece testPiece = Piece("test", "SPIDER");

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

Game::Game(string p1, string p2)
{
	names.insert({ white, p1 });
	names.insert({ black, p2 });

	addStartingPieces();

	board.add(Location(1, 1), Ant(white));
	board.print();
}

Game::~Game()
{
}


void Game::addStartingPieces()
{
	for (string color : colors) {
		unusedPieces[color].push_back(Queen(color));

		unusedPieces[color].push_back(Beetle(color));
		unusedPieces[color].push_back(Beetle(color));
		
		unusedPieces[color].push_back(Spider(color));
		unusedPieces[color].push_back(Spider(color));
		
		unusedPieces[color].push_back(Grasshopper(color));
		unusedPieces[color].push_back(Grasshopper(color));
		unusedPieces[color].push_back(Grasshopper(color));
		
		unusedPieces[color].push_back(Ant(color));
		unusedPieces[color].push_back(Ant(color));
		unusedPieces[color].push_back(Ant(color));
	}
}
