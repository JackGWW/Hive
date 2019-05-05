// Hive.cpp : This file contains the 'main' function. Program execution begins and ends there.

#include "Hive.h"
#include <unordered_map>
#include <vector>

using std::unordered_map;
using std::cout;
using std::cin;
using std::endl;
using std::vector;
using std::string;

// TODO add in proper public/private usage
// TODO add in const usage
// TODO add virtual classes and virtual desctructor to piece


int main()
{
	//string white = "white";
	//string black = "black";
	//
	//Board b;
	//Piece wAnt = Piece(white, "A");
	//Piece bQueen = Piece(black, "QUEEN");
	//Grasshopper wHopper = Grasshopper(white);
	//
	//Location test = Location(0, 0, 0);
	//vector<Location> testv = test.adjecent();
	//
	//cout << "Welcome to Hive!\n";

	//printLocations(testv);
	//cout << "DONE" << endl;

	Game game = Game("Player 1 (W)", "Player 2 (B)");
	game.play();
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

Game::Game(string p1, string p2)
{
	names.push_back(p1);
	names.push_back(p2);

	addStartingPieces();
}

Game::~Game()
{
}

void Game::play()
{
	cout << "WELCOME TO HIVE - CONSOLE EDITION" << endl;
	Piece toPlay;
	Location placementLocation;

	while (!gameOver) {
		//get moveable pieces (unplayed and on board)
		
		playTurn();
		nextTurn();
	}
	//TODO add checks that pieces can be added or moved
	for (int i = 0; i < 50; i++) {
		// TODO add check that you have a piece to play
		
	}
	
	cout << "THANKS FOR PLAYING!" << endl;
}

void Game::nextTurn()
{
	cout << "Current Board:" << endl;
	board.print();

	curPlayer = (curPlayer + 1) % 2;
	turnCount++;
	//Check for game over
	cout << endl << names[curPlayer] << ", it's your turn!" << endl;
}

void Game::playPiece(Piece p)
{
	if (turnCount == 0) {
		board.add(Location(0, 0), p);
		return;
	} 
	if (turnCount == 1) {
		board.add(Location(0, 2), p);
		return;
	}

	vector<Location> validLocations = board.getPlacementLocations(colors[curPlayer]);
	int chosenLocation = -1;

	cout << names[curPlayer] << ", where do you want to put your piece?" << endl;
	board.print(validLocations);

	cin >> chosenLocation;
	int maxLocation = validLocations.size() - 1;
	while (chosenLocation < 0 || chosenLocation > maxLocation) {
		cout << "Please enter a number between 0 and " << maxLocation << endl;
		cin >> chosenLocation;
	}
	board.add(validLocations[chosenLocation], p);
}

void Game::movePiece(Location l)
{
	//vector<Location> validLocations = board.getPiece(l).moves();
	vector<Location> validLocations = board.getPlacementLocations(colors[curPlayer]);
	int chosenLocation = -1;

	cout << names[curPlayer] << ", where do you want to move your piece?" << endl;
	board.print(validLocations);

	cin >> chosenLocation;
	int maxLocation = validLocations.size() - 1;
	while (chosenLocation < 0 || chosenLocation > maxLocation) {
		cout << "Please enter a number between 0 and " << maxLocation << endl;
		cin >> chosenLocation;
	}
	board.move(l, validLocations[chosenLocation]);
}

void Game::playTurn()
{
	int chosenPiece = -1;
	
	cout << names[curPlayer] << ", which piece do you want to move/play?" << endl;
	
	//Pieces already on the board
	vector<Location> moveablePieces = board.getMoveablePieces(colors[curPlayer]);
	board.print(moveablePieces);
	
	//Pieces not yet on the board
	printUnusedPieces(curPlayer, moveablePieces.size());

	cin >> chosenPiece;
	int maxPiece = moveablePieces.size() + unusedPieces[curPlayer].size() - 1;
	while (chosenPiece < 0 || chosenPiece > maxPiece) {
		cout << "Please enter a number between 0 and " << maxPiece << endl;
		cin >> chosenPiece;
	}

	if (chosenPiece < int(moveablePieces.size())) {
		movePiece(moveablePieces[chosenPiece]);
	}
	else {
		Piece toPlay = unusedPieces[curPlayer][chosenPiece];
		unusedPieces[curPlayer].erase(unusedPieces[curPlayer].begin() + chosenPiece);
		playPiece(toPlay);
	}
}


void Game::printUnusedPieces(int player, int offset) 
{
	vector<Piece> pieces = unusedPieces[player];
	int numPieces = pieces.size();
	int number;

	for (int i = 0; i < numPieces; i++) {
		cout << "  ____  ";
	}
	cout << endl;
	
	for (int i = 0; i < numPieces; i++) {
		number = i + offset;
		if (number < 10) {
			cout << " /  " << number << " \\ ";
		}
		else {
			cout << " / " << number << " \\ ";
		}
	}
	cout << endl;

	for (Piece p : pieces) {
		cout << "/" << p.paddedName() << "\\";
	}
	cout << endl;

	for (Piece p : pieces) {
		cout << "\\" << p.paddedColor() << "/";
	}
	cout << endl;

	for (int i = 0; i < numPieces; i++) {
		cout << " \\____/ ";
	}
	cout << endl;

	cout << endl;
}

void Game::addStartingPieces()
{
	for (int i = 0; i < 2; i ++) {
		unusedPieces.push_back(vector<Piece>());
		unusedPieces[i].push_back(Queen(colors[i]));

		unusedPieces[i].push_back(Beetle(colors[i]));
		unusedPieces[i].push_back(Beetle(colors[i]));
		
		unusedPieces[i].push_back(Spider(colors[i]));
		unusedPieces[i].push_back(Spider(colors[i]));
		
		unusedPieces[i].push_back(Grasshopper(colors[i]));
		unusedPieces[i].push_back(Grasshopper(colors[i]));
		unusedPieces[i].push_back(Grasshopper(colors[i]));
		
		unusedPieces[i].push_back(Ant(colors[i]));
		unusedPieces[i].push_back(Ant(colors[i]));
		unusedPieces[i].push_back(Ant(colors[i]));
	}
}
