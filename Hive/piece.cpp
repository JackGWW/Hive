#include "pch.h"
#include "piece.h"
#include <string>
#include <iostream>

using std::string;
using std::cout;
using std::endl;


void addPadding(string &word, int &lineWidth) {
	int spacesToAdd = lineWidth - word.length();
	int spacesToInsert = spacesToAdd / 2;
	int spacesToAppend = spacesToAdd - spacesToInsert;

	word.insert(0, spacesToInsert, ' ');
	word.append(spacesToAppend, ' ');
}


Piece::Piece(string _color, string _name) : color(_color), name(_name)
{
}

Piece::~Piece()
{
}

void Piece::print()
{
	int lineWidth = 6;
	
	addPadding(color, lineWidth);
	addPadding(name, lineWidth);

	cout << "  ____" << endl;
	cout << " /    \\" << endl;
	cout << "/" << name << "\\" << endl;
	cout << "\\" << color << "/" << endl;
	cout << " \\____/" << endl;
}
