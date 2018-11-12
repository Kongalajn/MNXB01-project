#include "tempTrender.h"
#include <string>

void project() {
	string pathToFile = "/home/courseuser/Downloads/MNXB01-project/datasets/smhi-opendata_Boras.csv"; //Put the path to your data file here
	tempTrender t(pathToFile); //Instantiate your analysis object
	
	//t.tempOnDay(8, 23); //Call some functions that you've implemented
	//t.tempOnDay(235);
	//t.tempPerDay();
	t.hotCold();
	//t.tempPerYear(2050);
}
