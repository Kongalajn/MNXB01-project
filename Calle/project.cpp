#include "tempTrender.h"
#include <string>

void project();

void project() {
	string pathToFile = "/home/calle/MNXB01/Project/datasets/smhi-opendata_Lund.csv"; //Put the path to your data file here
	tempTrender t(pathToFile); //Instantiate your analysis object
	
	t.tempOnDay(1, 1, 20); //Call some functions that you've implemented
	//t.tempOnDay(235);
	//t.tempPerDay();
	//t.hotCold();
	//t.tempPerYear(2050);
}
