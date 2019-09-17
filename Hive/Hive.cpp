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

#include <iostream>
#include <windows.h> // WinApi header

int main()
{
	Game game = Game("Player 1 (White)", "Player 2 (Black)");
	game.play();
}


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

	while (winner == -1) {
		
		
		playTurn();
		nextTurn();
	}
	if (winner == 2) {
		cout << "TIE GAME" << endl;
	}
	else {
		cout << names[winner] << " WON, CONGRATS" << endl;
	}
	cout << "THANKS FOR PLAYING!" << endl;
}

void Game::nextTurn()
{
	cout << "Current Board:" << endl;
	board.print();

	curPlayer = (curPlayer + 1) % 2;
	turnCount++;
	winner = board.checkForWinner(colors);
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
	//TODO handle logic where piece cannot legally be added to the board
	int chosenPieceIndex = -1;
	const vector<Location> & moveablePieceLocations = board.getMoveablePieces(colors[curPlayer]);
	int maxPiece = moveablePieceLocations.size() + countPlayablePieces(curPlayer) - 1;

	cout << endl << names[curPlayer] << ", it's your turn!" << endl;

	//No legal moves
	if (maxPiece < 0) {
		cout << "Your turn is skipped as you have no legal moves." << endl;
		return;
	}
	cout << "Which piece do you want to move/play?" << endl;

	//Pieces already on the board
	board.print(moveablePieceLocations);
	
	//Pieces not yet on the board
	printUnusedPieces(curPlayer, moveablePieceLocations.size());

	cin >> chosenPieceIndex;
	while (chosenPieceIndex < 0 || chosenPieceIndex > maxPiece) {
		cout << "Please enter a number between 0 and " << maxPiece << endl;
		cin >> chosenPieceIndex;
	}

	//Check if piece is being moved or added to the board
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

int Game::playerTurn(int player)
{
	return ((turnCount - player) / 2) + 1;
}

bool Game::mustPlayQueen(int player) {
	if (!board.getQueenPlayed(colors[player]) && playerTurn(player) >= 4) {
		return true;
	}
	return false;
}

int Game::countPlayablePieces(int player)
{
	if (mustPlayQueen(player))
		return 1;
	return unusedPieces[player].size();
}

void Game::printUnusedPieces(int player, int offset) 
{
	vector<unique_ptr<Piece>> & pieces = unusedPieces[player];
	int number;
	int numPieces;
	if (mustPlayQueen(player)) {
		numPieces = 1;
	}
	else {
		numPieces = pieces.size();
	}

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

	for (int i = 0; i < numPieces; i++) {
		cout << "/" << pieces[i]->paddedName() << "\\";
	}
	cout << endl; 

	for (int i = 0; i < numPieces; i++) {
		cout << "\\" << pieces[i]->paddedColor() << "/";
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
