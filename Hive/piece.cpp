#include "pch.h"
#include "piece.h"
#include <string>
#include <iostream>
#include "board.h"


using std::string;
using std::cout;
using std::endl;

string addPadding(string word) {
	int lineWidth = 6;
	int spacesToAdd = lineWidth - word.length();
	if (spacesToAdd <= 0) {
		return word;
	}

	int spacesToInsert = spacesToAdd / 2;
	int spacesToAppend = spacesToAdd - spacesToInsert;

	word.insert(0, spacesToInsert, ' ');
	word.append(spacesToAppend, ' ');

	return word;
}


string Piece::paddedColor()
{
	return addPadding(color);
}

string Piece::paddedName()
{
	return addPadding(name);
}

Piece::Piece() {
	color = "B/W";
	name = "??";
}

Piece::Piece(string _color, string _name) : color(_color), name(_name)
{
}

Piece::~Piece()
{
}

void Piece::print()
{	
	cout << "  ____" << endl;
	cout << " /    \\" << endl;
	cout << "/" << addPadding(name) << "\\" << endl;
	cout << "\\" << addPadding(color) << "/" << endl;
	cout << " \\____/" << endl;
}

bool Piece::isQueen() 
{
	return false;
}

vector<Location> Piece::moves(const Board &, Location)
{
	return vector<Location>();
}
