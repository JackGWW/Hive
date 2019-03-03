#pragma once
#include <string>

using std::string;

class Piece
{
private:
	string color;
	string name;
public:
	Piece(string _color, string _name);
	~Piece();
	void print();
};
