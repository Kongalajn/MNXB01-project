#include "tempTrender.h"
#include <string>

void project();

void project() {
	string pathToFile = "/home/calle/MNXB01/Project/datasets/uppsala_tm_1722-2013.dat"; //Put the path to your data file here
	tempTrender t(pathToFile); //Instantiate your analysis object
	
	//t.tempOnDay(8, 23, 20); //Call some functions that you've implemented
	t.tempSpectrogram();
	//t.tempOnDay(235);
	//t.tempPerDay();
	//t.hotCold();
	//t.tempPerYear(2050);
}
