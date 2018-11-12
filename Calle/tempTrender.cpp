#include <iostream>
#include "tempTrender.h"

tempTrender::tempTrender(string pathToFile) {
	readFile(pathToFile);
	cout << "The user supplied " << pathToFile << " as the path to the data file." << endl;
}

//tempTrender::~tempTrender() { }
