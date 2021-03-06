#include "pch.h"
#include "board.h"
#include "piece.h"
#include <stdlib.h>
#include <iostream>
#include <stdlib.h>
#include <string>
#include <unordered_map>
#include <unordered_set>
#include <queue>
#include <utility> //unique_ptr
#include <set>
#include <memory>

//Needed for color printing
#include <iostream>
#include <windows.h> // WinApi header


using std::cout;
using std::endl;
using std::string;
using std::unordered_map;
using std::unordered_set;
using std::queue;
using std::unique_ptr;
using std::max;


//TODO remove current piece when sliding to avoid it sliding onto itself.

Board::Board()
{
}


Board::~Board()
{
}

bool Board::getQueenPlayed(string color)
{
	return queenPlayed[color];
}


bool Board::onTop(const Location & l) {
	if (!(exists(l))) {
		throw std::invalid_argument("Piece must exist in the board");
	}
	return (!exists(l.above()));
}


bool Board::onlyTouches(const Location & l, const string & color) {
	vector<Location> adj = adjecentPieces(l);
	for (Location a : adj) {
		if (pieces.at(topPieceLocation(a))->color != color) {
			return false;
		}
	}
	return true;
}

int Board::checkForWinner(vector<string> colors) {
	int countQueensSurrounded = 0;
	string losingColor = "";

	for (auto& it : pieces) {
		if (it.second->isQueen()) {
			if (fullySurrounded(it.first)) {
				losingColor = it.second->color;
				countQueensSurrounded++;
			}
		}
	}

	//Nobody has won
	if (countQueensSurrounded == 0) {
		return -1;
	}

	//Tie Game
	if (countQueensSurrounded == 2) {
		return 2;
	}
	
	std::pair<bool, int> result = findInVector<string>(colors, losingColor);

	//Return opposite of whichever queen got surrounded
	if (result.first) {
		return (result.second + 1) % 2;
	}
	else {
		throw std::invalid_argument("Queen color not in vector of colors");
	}
}


vector<Location> Board::getColoredPieces(const string & color) {
	vector<Location> allLocations;

	//Find all locations in the board that are the same color
	for (auto& it : pieces) {
		Location l = it.first;

		if (it.second->color == color && onTop(l)) {
			allLocations.push_back(l);
		}
	}
	return allLocations;
}

vector<Location> Board::getPlacementLocations(const string & color)
{
	unordered_set<Location> validLocations;
	vector<Location> allLocations = getColoredPieces(color);

	for (Location l : allLocations) {
		vector<Location> adj = groundedAdjecentLocations(l);
		for (Location a : adj) {
			if (!exists(a) && onlyTouches(a, color)) {
				validLocations.emplace(a);
			}
		}
	}

	return vector<Location> (validLocations.begin(), validLocations.end());
}

vector<Location> Board::getMovementLocations(const Location & l)
{
	return pieces.at(l)->moves(*this, l);
}

void Board::movePiece(Location origin, Location destination)
{
	if (trapped(origin)) {
		throw std::invalid_argument("Piece is trapped and cannot be moved");
	}

	unique_ptr<Piece> toAdd = remove(origin);
	addPiece(destination, std::move(toAdd));
}

unique_ptr<Piece> Board::remove(Location l) {
	unique_ptr<Piece> p (std::move(pieces.at(l)));
	pieces.erase(l);
	return p;
}

vector<Location> Board::getMoveablePieces(string color) 
{
	vector<Location> moveablePieces;
	vector<Location> allLocations = getColoredPieces(color);

	if (!queenPlayed[color]) {
		return vector<Location>();
	}

	for (Location l : allLocations) {
		if (!trapped(l)) {
			moveablePieces.push_back(l);
		}
	}
	return moveablePieces;
}

template < typename T>
std::pair<bool, int > findInVector(const std::vector<T>  & vecOfElements, const T  & element)
{
	std::pair<bool, int > result;

	// Find given element in vector
	auto it = std::find(vecOfElements.begin(), vecOfElements.end(), element);

	if (it != vecOfElements.end())
	{
		result.second = distance(vecOfElements.begin(), it);
		result.first = true;
	}
	else
	{
		result.first = false;
		result.second = -1;
	}

	return result;
}

std::pair<bool, int > findInVector2D(const std::vector<Location>  & vecOfElements, const Location  & element)
{
	std::pair<bool, int > result;

	// Find given element in vector
	for (std::size_t i = 0; i < vecOfElements.size(); i++) {
		if (vecOfElements[i].x == element.x && vecOfElements[i].y == element.y) {
			result.first = true;
			result.second = i;
			return result;
		}
	}
	result.first = false;
	result.second = -1;
	return result;
}

string locationIndex(const vector<Location> & allLocations, const Location & specificLocation)
{
	std::pair<bool, int> result = findInVector<Location>(allLocations, specificLocation);

	if (result.first) {
		return intToString(result.second);
	}
	else {
		return "  ";
	}
}

string locationIndex2D(const vector<Location> & allLocations, const Location & specificLocation)
{
	std::pair<bool, int> result = findInVector2D(allLocations, specificLocation);

	if (result.first) {
		return intToString(result.second);
	}
	else {
		return "  ";
	}
}

string intToString(int number) {
	if (number < 10) {
		return ' ' + std::to_string(number);
	}
	else {
		return std::to_string(number);
	}

}

void Board::print()
{
	print(vector<Location>());
}

//TODO Move into a different file
//TODO rearchitect so that it doesn't rely on string matching
void setColor(string teamColor, int textColor) {
	unordered_map<string, int> colorMapping = {
		{"white", 15},
		{"black", 0}
	};

	//Set text color to opposite of background
	if (textColor == -1) {
		if (teamColor == "white") {
			textColor = colorMapping["black"];
		}
		else {
			textColor = colorMapping["white"];
		}
	}

	int consoleColor = textColor + colorMapping[teamColor] * 16;
	
	HANDLE hConsole;
	hConsole = GetStdHandle(STD_OUTPUT_HANDLE);

	SetConsoleTextAttribute(hConsole, consoleColor);

}

void resetColor() {
	int defaultColor = 128;
	
	HANDLE hConsole;
	hConsole = GetStdHandle(STD_OUTPUT_HANDLE);

	SetConsoleTextAttribute(hConsole, defaultColor);
}

void clearColor() {
	int defaultColor = 15;

	HANDLE hConsole;
	hConsole = GetStdHandle(STD_OUTPUT_HANDLE);

	SetConsoleTextAttribute(hConsole, defaultColor);
}

void Board::print(vector<Location> numberedLocations)
{
	int maxX = INT_MIN, maxY = INT_MIN;
	int minX = INT_MAX, minY = INT_MAX;

	//Find bounding coordinates of board
	for (auto& it : pieces) {
		Location l = it.first;

		if (l.x > maxX) maxX = l.x;
		if (l.y > maxY) maxY = l.y;
		if (l.x < minX) minX = l.x;
		if (l.y < minY) minY = l.y;
	}
	minX--; //For more consistent display of the board
	
	for (Location l : numberedLocations) {
		if (l.x > maxX) maxX = l.x;
		if (l.y > maxY) maxY = l.y;
		if (l.x < minX) minX = l.x;
		if (l.y < minY) minY = l.y;
	}

	//cout << "PRINTING BOARD: MIN(" << minX << "," << minY << ") MAX(" << maxX << "," << maxY << ")" << endl;

	//Iterate through every hexagonal spot on the board
	string color;
	string name;
	string number; 
	string prefix;
	Location curLocation;
	string pieceTeamColor;
	int pieceNameColor;

	resetColor();

	for (int y = maxY + 1; y >= minY - 1; y--) {
		//Iterate through top 2 lines of the hexagon
		for (int line = 0; line < 2; line++) {
			for (int x = minX; x <= maxX + 1; x++) {
				if ((x + y) % 2 == 0) {
					curLocation = Location(x, y);
				}
				switch (line) {
				case 0:
					//Line 1 of hexagon
					if ((y + x) % 2 == 0) {
						number = locationIndex2D(numberedLocations, curLocation);
						if (exists2D(Location(x - 1, y + 1))) {
							prefix = "";
						}
						else {
							prefix = " ";
						}
						if (exists2D(curLocation) || exists2D(Location(x - 1, y + 1))) {
							//Current location has a piece
							if (exists2D(curLocation)) {
								cout << prefix << "/";

								pieceTeamColor = pieces[topPieceLocation(curLocation)]->color;
								setColor(pieceTeamColor, -1);
								cout << " " << number << " ";
								resetColor();
							}
							//Location to the upper left has a piece
							else {
								cout << prefix << "/    ";
							}
						}
						//No piece here
						else {
							cout << "      ";
						}
					}
					//Line 3 of hexagon
					else {
						if (exists2D(Location(x, y + 1))) {
							color = pieces[topPieceLocation(Location(x, y + 1))]->paddedColor();
							cout << "\\";
							
							pieceTeamColor = pieces[topPieceLocation(Location(x, y + 1))]->color;
							pieceNameColor = pieces[topPieceLocation(Location(x, y + 1))]->nameColor;
							setColor(pieceTeamColor, pieceNameColor);
							cout << color;
							resetColor();
						}
						else if(exists2D(Location(x - 1, y ))) {
						    cout << "\\     ";
						} else {
							cout << "      ";
						}
					}
					break;
				case 1:
					//Line 2 of hexagon
					if ((y + x) % 2 == 0) {
						number = locationIndex2D(numberedLocations, curLocation);
						if (exists2D(curLocation)) {
							cout << "/";
							
							name = pieces[topPieceLocation(curLocation)]->paddedName();
							pieceTeamColor = pieces[topPieceLocation(Location(curLocation))]->color;
							pieceNameColor = pieces[topPieceLocation(Location(curLocation))]->nameColor;
							setColor(pieceTeamColor, pieceNameColor);
							cout << name;
							resetColor();
						}
						else if (exists2D(Location(x - 1, y + 1))) {
							cout << "/ " << number << "  ";
						}
						else {
							cout << "  " << number << "  ";
						}
					}
					//Line 4 of hexagon
					else {
						if (exists2D(Location(x, y + 1)) || exists2D(Location(x - 1, y))) {
							if (exists2D(Location(x - 1, y))) {
								cout << "\\";
							}
							else {
								cout << " \\";
							}
						}
						else {
							cout << "  ";
						}
						if (exists2D(Location(x, y + 1)) || exists2D(Location(x, y -1))) {
							if (exists2D(Location(x, y + 1))) {
								pieceTeamColor = pieces[topPieceLocation(Location(x, y + 1))]->color;
								setColor(pieceTeamColor, 0);
							}
							cout << "____";
							resetColor();
						}
						else {
							cout << "    ";
						}

					}
					break;
				}
			}
			cout << endl;
		}
	}
	clearColor();
}

void Board::addPiece(const Location& l, unique_ptr<Piece> p)
{
	if (exists(l)){
		throw std::invalid_argument("There is already a piece in location: " + l.to_string());
	}

	if (p->isQueen()) {
		queenPlayed[p->color]= true;
	}

	pieces.emplace(l, move(p));
	//TODO use return value to check if it was successfully inserted instead of check in advance
}

bool Board::pinned(const Location & curLoc) const
{
	if (curLoc.z > 0) {
		return false;
	}

	unordered_set<Location> visited;
	queue<Location> toVisit;
	vector<Location> addToVisit;
	Location visiting;
	vector<Location> adj = adjecentPieces(curLoc);

	//Can't be pinned if leaf node
	if (adj.size() == 1) {
		return false;
	}

	//Don't explore the current location
	visited.emplace(curLoc);

	//Traverse all connected pieces from one adjecent piece
	toVisit.emplace(adj[0]);
	while (!toVisit.empty()) {
		visiting = toVisit.front();
		visited.emplace(visiting);

		addToVisit = adjecentPieces(visiting);
		for (Location l : addToVisit) {
			//Don't add places we've already visisted
			if (!visited.count(l)) {
				toVisit.push(l);
			}
		}
		toVisit.pop();
	}

	//If the adjecent pieces aren't connected, it is pinned
	for (Location l : adj) {
		if (!visited.count(l)) {
			return true;
		}
	}
	return false;
}

//Check if there is a piece on top
bool Board::covered(Location l) const
{
	l.move(0, 0, 1);
	return exists(l);
}

bool Board::surrounded(const Location & l) const 
{	
	//Check if can't physically slide
	//If there are two consecutive empty adjecent spaces, it isn't surrounded
	vector<Location> adj = l.adjecentLocations();
	for (int i = 0; i < 6; i++) { 
		if (!exists(adj[i]) && !exists(adj[(i + 1) % 6])) {
			return false;
		}
	}
	return true;
}

bool Board::fullySurrounded(const Location & curLoc) const
{
	vector<Location> adj = curLoc.adjecentLocations();
	for (Location l : adj) {
		if (!exists(l))
			return false;
	}
	return true;
}

bool Board::trapped(const Location & l) const
{
	return covered(l) || pinned(l) || surrounded(l);
}

bool Board::exists(const Location & l) const
{
	if (pieces.find(l) == pieces.end()) 
		return false;
	
	return true;
}

bool Board::exists2D(const Location & l) const
{
	for (const auto& any : pieces) {
		if (any.first.x == l.x && any.first.y == l.y)
			return true;
	}
	return false;
}

vector<Location> Board::adjecentPieces(const Location & l) const
{	
	vector<Location> adjPieces;
	vector<Location> adjSpots = l.adjecentLocations();

	for (Location spot : adjSpots)
	{
		if (exists(spot))
		{
			adjPieces.push_back(spot);
		}
	}

	return adjPieces;
}

vector<Location> Board::slide(const Location & curLoc) const
{
	//Function will keep sliding clockwise until it reaches locations it's already visited
	unordered_set<Location> visited;
	queue<Location> toVisit;
	vector<Location> addToVisit;
	Location visiting;

	if (trapped(curLoc)){
		return vector<Location>();
	}
	
	//Add CCW neighbouring location(s) to visited
	//This will stop the function from 
	addToVisit = slideCCW(curLoc);
	for (Location l : addToVisit) {
		visited.emplace(visiting);
	}

	//Set CW neighbouring location(s) as the starting location
	addToVisit = slideCW(curLoc);
	for (Location l : addToVisit) {
		toVisit.push(l);
	}

	//Stop sliding once the CCW side of the current piece is reached
	for (Location l : slideCCW(curLoc)) {
		visited.emplace(l);
	}

	while (!toVisit.empty()) {
		visiting = toVisit.front();
		visited.emplace(visiting);

		addToVisit = slideCW(visiting);
		for (Location l : addToVisit) {
			//Don't revist past places
			if (!visited.count(l)) {
				toVisit.push(l);
			}
		}
		toVisit.pop();
	}
	return vector<Location>(visited.begin(), visited.end());
}

vector<Location> Board::groundedAdjecentLocations(const Location & l) const
{
	return Location(l.x, l.y).adjecentLocations();
}

vector<Location> Board::groundedAdjecentPieces(const Location & curLoc) const
{
	vector<Location> pieceLocations;
	for (Location l : groundedAdjecentLocations(curLoc)) {
		if (exists(l))
			pieceLocations.push_back(l);
	}
	return pieceLocations;
}

vector<Location> Board::slideOnTop(const Location & curLoc) const
{
	if (curLoc.z <= 0) {
		throw std::invalid_argument("Current location must be above ground level");
	}

	vector<Location> destinations;
	if (covered(curLoc)) {
		return destinations;
	}

	Location dest;
	vector<Location> groundAdj = groundedAdjecentPieces(curLoc);
	for (Location l: groundAdj) {
		dest = locationOnTop(l);
		if (canSlide(dest, curLoc)) {
			destinations.push_back(dest);
		}
	}
	return destinations;
}

vector<Location> Board::slide(const Location & curLoc, int moves) const
{
	if (moves <= 0) {
		throw std::invalid_argument("Number of moves must be greater than 0");
	}

	if (curLoc.z > 0) {
		if (moves != 1) {
			throw std::invalid_argument("Can only slide 1 at a time on top of the hive");
		}
		return slideOnTop(curLoc);
	}
	
	if (trapped(curLoc)) {
		return vector<Location>();
	}

	unordered_set<Location> destinations;

	// Find all destinations sliding clockwise
	vector<Location> curLocations;
	vector<Location> nextLocations;
	vector<Location> nextLoc;
	curLocations.push_back(curLoc);
	for (int i = 0; i < moves; i++) {
		//Get next location for every current location
		for (Location l : curLocations) {
			nextLoc = slideCW(l);
			nextLocations.insert(nextLocations.end(), nextLoc.begin(), nextLoc.end());
		}
		curLocations = nextLocations;
		nextLocations.clear();
	}
	destinations.insert(curLocations.begin(), curLocations.end());

	// Find all destinations sliding counter-clockwise
	curLocations.clear();
	nextLocations.clear();
	nextLoc.clear();
	curLocations.push_back(curLoc);
	for (int i = 0; i < moves; i++) {
		//Get next location for every current location
		for (Location l : curLocations) {
			nextLoc = slideCCW(l);
			nextLocations.insert(nextLocations.end(), nextLoc.begin(), nextLoc.end());
		}
		curLocations = nextLocations;
		nextLocations.clear();
	}
	destinations.insert(curLocations.begin(), curLocations.end());

	return vector<Location> (destinations.begin(), destinations.end());
}

vector<Location> Board::slideCCW(const Location & curLoc) const
{
	vector<Location> adj = curLoc.adjecentLocations();
	vector<Location> nextLoc;

	//iterate through all six adjecent pieces
	for (int i = 0; i < 6; i++) {
		//to be able to slide, there needs to be a two piece gap after a piece
		if (exists(adj[(i + 5) % 6]) && !exists(adj[i]) && !exists(adj[(i + 1) % 6])) {
			nextLoc.push_back(adj[i]);
		}
	}
	return nextLoc;
}

bool Board::isAdjecent2D(const Location & start, const Location & end) const
{
	int xDelta = start.x - end.x;
	int yDelta = start.y - end.y;
	if (abs(xDelta) + abs(yDelta) > 2) {
		return false;
	}
	return true;
}

bool Board::canSlide(const Location & end, const Location & start) const
{
	if (!isAdjecent2D(start, end)) {
		throw std::invalid_argument("Pieces must be adjecent");
	}
	int zCoord = max(start.z, end.z);
	vector<Location> guardPieces;
	vector<Location> startAdj = adjecentPieces(Location(start.x, start.y, zCoord));
	
	//Find mutually adjecent pieces
	for (Location l : startAdj) {
		if (isAdjecent2D(l, end)) {
			guardPieces.push_back(l);
		}
	}

	//Can't slide if both mutually adjecent pieces exist
	if (guardPieces.size() > 1) {
		return false;
	}
	return true;
}

vector<Location> Board::descend(const Location & curLoc) const
{
	vector<Location> destinations;
	//On the ground, cannot descend
	if (curLoc.z == 0) {
		return destinations;
	}

	vector<Location> adj = curLoc.adjecentLocations();
	
	for (Location l : adj) {
		int xCoord = l.x;
		int yCoord = l.y;
		Location ground = Location(xCoord, yCoord);
		if (!exists(ground) && canSlide(ground, curLoc)) {
			destinations.push_back(ground);
		}
	}

	return destinations;
}

Location Board::locationOnTop(Location curLoc) const
{
	while (exists(curLoc)) {
		curLoc.move(0, 0, 1);
	}
	return curLoc;
}

Location Board::topPieceLocation(Location curLoc) const
{
	if (!exists(curLoc))
		throw std::invalid_argument("Piece must exist in location");

	while (exists(curLoc.above())) {
		curLoc.move(0, 0, 1);
	}
	return curLoc;
}

vector<Location> Board::climb(const Location & curLoc) const
{
	vector<Location> destinations;
	vector<Location> adj = adjecentPieces(curLoc);
	for (Location loc : adj) {
		loc = locationOnTop(loc);
		if (canSlide(loc, curLoc)) {
			destinations.push_back(locationOnTop(loc));
		}
	}

	return destinations;
}

vector<Location> Board::slideCW(const Location & curLoc) const
{
	vector<Location> adj = curLoc.adjecentLocations();
	vector<Location> nextLoc;

	//iterate through all six adjecent pieces
	for (int i = 0; i < 6; i++) {
		//to be able to slide, there needs to be a two piece gap after a piece
		if (exists(adj[(i + 1) % 6]) && !exists(adj[i]) && !exists(adj[(i + 5) % 6])) {
			nextLoc.push_back(adj[i]);
		}
	}
	return nextLoc;
}