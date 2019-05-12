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
using std::move;

// TODO add in proper public/private usage
// TODO add in const usage
// TODO add virtual classes and virtual desctructor to piece


int main()
{
	/*testMoves();
	return 0;*/
	Game game = Game("Player 1 (White)", "Player 2 (Black)");
	game.play();
}

//void testMoves()
//{
//	Board b;
//	b.addPiece(Location(0, 0), unique_ptr<Piece> (new Queen("WHITE")));
//	b.addPiece(Location(1, 3), unique_ptr<Piece> (new Ant("WHITE")));
//	b.addPiece(Location(1, 1), unique_ptr<Piece> (new Beetle("WHITE")));
//	b.addPiece(Location(-1, 1), unique_ptr<Piece> (new Grasshopper("WHITE")));
//	b.addPiece(Location(0, 2), unique_ptr<Piece> (new Spider("WHITE")));
//
//	b.print(b.getMovementLocations(Location(0,0)));
//	b.print(b.getMovementLocations(Location(1, 1)));
//	b.print(b.getMovementLocations(Location(1, 3)));
//	b.print(b.getMovementLocations(Location(0, 2)));
//	b.print(b.getMovementLocations(Location(-1, 1)));
//
//	//printLocations(b.getMovementLocations(Location(1, 1)));
//
//}

void printLocations(vector<Location> locations)
{
	Board b;

	//Add pieces to board
	for (Location l : locations) {
		unique_ptr<Piece> testPiece(new Piece("test", "TEST"));
		try {
			b.addPiece(l, move(testPiece));
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
	//Piece toPlay;
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
	//TODO Check for game over
	cout << endl << names[curPlayer] << ", it's your turn!" << endl;
}

void Game::playPiece(unique_ptr<Piece> p)
{
	if (turnCount == 0) {
		board.addPiece(Location(0, 0), move(p));
		return;
	} 
	if (turnCount == 1) {
		board.addPiece(Location(0, 2), move(p));
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
	board.addPiece(validLocations[chosenLocation], move(p));
}

void Game::movePiece(Location locationToMove)
{
	int chosenLocation = -1;
	vector<Location> validLocations = board.getMovementLocations(locationToMove);

	cout << names[curPlayer] << ", where do you want to move your piece?" << endl;
	board.print(validLocations);

	cin >> chosenLocation;
	int maxLocation = validLocations.size() - 1;
	while (chosenLocation < 0 || chosenLocation > maxLocation) {
		cout << "Please enter a number between 0 and " << maxLocation << endl;
		cin >> chosenLocation;
	}
	board.movePiece(locationToMove, validLocations[chosenLocation]);
}

void Game::playTurn()
{
	int chosenPieceIndex = -1;
	const vector<Location> & moveablePieceLocations = board.getMoveablePieces(colors[curPlayer]);
	int maxPiece = moveablePieceLocations.size() + unusedPieces[curPlayer].size() - 1;

	cout << names[curPlayer] << ", which piece do you want to move/play?" << endl;

	//Pieces already on the board
	board.print(moveablePieceLocations);
	
	//Pieces not yet on the board
	printUnusedPieces(curPlayer, moveablePieceLocations.size());

	cin >> chosenPieceIndex;
	while (chosenPieceIndex < 0 || chosenPieceIndex > maxPiece) {
		cout << "Please enter a number between 0 and " << maxPiece << endl;
		cin >> chosenPieceIndex;
	}

	//Move piece on board
	if (chosenPieceIndex < int(moveablePieceLocations.size())) {
		movePiece(moveablePieceLocations[chosenPieceIndex]);
	}
	//Remove piece from unused pieces and add to board
	else {
		chosenPieceIndex -= moveablePieceLocations.size();
		unique_ptr<Piece> toPlay (move(unusedPieces[curPlayer][chosenPieceIndex]));
		unusedPieces[curPlayer].erase(unusedPieces[curPlayer].begin() + chosenPieceIndex);
		playPiece(move(toPlay));
	}
}


void Game::printUnusedPieces(int player, int offset) 
{
	vector<unique_ptr<Piece>> & pieces = unusedPieces[player];
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

	for (unique_ptr<Piece> & p : pieces) {
		cout << "/" << p->paddedName() << "\\";
	}
	cout << endl;

	for (unique_ptr<Piece> & p : pieces) {
		cout << "\\" << p->paddedColor() << "/";
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
		unusedPieces.push_back(vector<unique_ptr<Piece>>());
		unusedPieces[i].emplace_back(new Queen(colors[i]));

		unusedPieces[i].emplace_back(new Beetle(colors[i]));
		unusedPieces[i].emplace_back(new Beetle(colors[i]));
		
		unusedPieces[i].emplace_back(new Spider(colors[i]));
		unusedPieces[i].emplace_back(new Spider(colors[i]));
		
		unusedPieces[i].emplace_back(new Grasshopper(colors[i]));
		unusedPieces[i].emplace_back(new Grasshopper(colors[i]));
		unusedPieces[i].emplace_back(new Grasshopper(colors[i]));
		
		unusedPieces[i].emplace_back(new Ant(colors[i]));
		unusedPieces[i].emplace_back(new Ant(colors[i]));
		unusedPieces[i].emplace_back(new Ant(colors[i]));
	}
}
