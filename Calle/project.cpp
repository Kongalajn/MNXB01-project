#include "tempTrender.h"
#include <string>

void project();

void project() {
	string pathToFile = "/home/courseuser/MNXB01-project/datasets/smhi-opendata_Lund.csv"; //Put the path to your data file here
	tempTrender t(pathToFile); //Instantiate your analysis object
	
	t.tempOnDay(31, 12, 2); //Call some functions that you've implemented
	
	t.DataView3D(1961, 2015, false);
	t.tempPerYear(2050);
}
