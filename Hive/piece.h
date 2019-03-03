#pragma once
#include <string>

using std::string;

class Piece
{
private:
public:
	string name;
	string color;
	string paddedColor();
	string paddedName();
	Piece(string _color, string _name);
	Piece();
	~Piece();
	void print();
};
