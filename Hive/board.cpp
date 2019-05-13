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


using std::cout;
using std::endl;
using std::string;
using std::unordered_map;
using std::unordered_set;
using std::queue;
using std::unique_ptr;


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
		if (pieces.at(a)->color != color) {
			return false;
		}
	}
	return true;
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
							if (exists2D(curLocation)) {
								cout << prefix << "/ " << number << " ";						
							}
							else {
								cout << prefix << "/    ";
							}
						}
						else {
							cout << "      ";
						}
					}
					//Line 3 of hexagon
					else {
						if (exists2D(Location(x, y + 1))) {
							color = pieces[topPieceLocation(Location(x, y + 1))]->paddedColor();
							cout << "\\" << color;
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
							name = pieces[topPieceLocation(curLocation)]->paddedName();
							cout << "/" << name;
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
							cout << "____";
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
	unordered_set<Location> visited;
	vector<Location> adj = adjecentPieces(curLoc);
	queue<Location> toVisit;
	vector<Location> addToVisit;
	Location visiting;

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
	//Can't physically move
	//If there are two consecutive empty adjecent spaces, it isn't surrounded
	vector<Location> adj = l.adjecentLocations();
	for (int i = 0; i < 6; i++) { 
		if (!exists(adj[i]) && !exists(adj[(i + 1) % 6])) {
			return false;
		}
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
	unordered_set<Location> visited;
	queue<Location> toVisit;
	vector<Location> addToVisit;
	Location visiting;

	if (trapped(curLoc)){
		return vector<Location>();
	}

	addToVisit = slideCW(curLoc);
	for (Location l : addToVisit) {
		toVisit.push(l);
	}

	while (!toVisit.empty()) {
		visiting = toVisit.front();
		visited.emplace(visiting);

		addToVisit = slideCW(visiting);
		for (Location l : addToVisit) {
			//Don't add places we've already checked
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

vector<Location> Board::slideOnTop(const Location & curLoc) const
{
	if (curLoc.z <= 0) {
		throw std::invalid_argument("Current location must be above ground level");
	}

	vector<Location> destinations;
	if (covered(curLoc)) {
		return destinations;
	}

	vector<Location> groundAdj = groundedAdjecentLocations(curLoc);
	for (Location dest : groundAdj) {
		if (exists(dest)){
			if (canSlide(dest, curLoc)) {
				destinations.push_back(dest);
			}
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
	if (trapped(curLoc)){
		throw std::invalid_argument("Piece cannot be moved");
	}
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
	int zCoord = std::max(start.z, end.z);
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

	vector<Location> adj = adjecentPieces(curLoc);
	
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
	if (trapped(curLoc)) {
		throw std::invalid_argument("Piece cannot be moved");
	}
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