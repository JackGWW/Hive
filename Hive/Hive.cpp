// Hive.cpp : This file contains the 'main' function. Program execution begins and ends there.

#include "pch.h"
#include <iostream>
#include <stdlib.h>
#include <string>

using std::cout;
using std::endl;
using std::string;

int main()
{
	cout << "Welcome to Hive!\n";

	string white = "WHITE";
	string black = "BLACK";

	cout << "  ____" << endl;
	cout << " /    \\" << endl;
	cout << "/" << black << " \\____" << endl;

	for (int i = 0; i < 5; i++) {
		int c = rand() % 2;
		string color = c == 0 ? white : black;
		cout << "\\ Ant  /    \\" << endl;
		cout << " \\____/" << color << " \\" << endl;
		cout << " /    \\ Ant  /" << endl;
		cout << "/" << color << " \\____/" << endl;
	}

	cout << "\\ Ant  /" << endl;
	cout << " \\____/" << endl;
}