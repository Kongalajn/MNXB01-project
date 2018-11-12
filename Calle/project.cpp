#include "tempTrender.h"
#include <string>

void project() {
	string pathToFile = "/home/hurvan/MNXB01_project/MNXB01-project/datasets/smhi-opendata_Lulea.csv"; //Put the path to your data file here
	tempTrender t(pathToFile); //Instantiate your analysis object
	
	t.tempOnDay(31, 12, 2); //Call some functions that you've implemented


	// The DataView3D(Int_t startYear, Int_t endYear, bool isUppsala, Int_t startTime=666, Int_t endTime=666)
	// plots a 2D/3D graph that shows the data points in a window specified by the user.
	// The first two arguments specify between which year to look, the third argument is true if
	// you want to look at the uppsala data, else its false.
	// The two last arguments are only for the CSV files. There you can specify which timewindow
	// of the day you want to view datapoints from. Put time window to 0,24 if you want the full day.
	// 
	//
	// There is no error catching for when asking for years that are not in the data file.
	//
	// There seems to be some very mysterious problem with how the range of the axis are set, sometimes
	// it works and sometimes it doesnt. I have not been able to figure out why this is. 

	//t.DataView3D(1722,2014,true); //if uppsala just specify start year and end year
	t.DataView3D(1962,2015,false,10,17); //if other files, the last two args are the timewindow of the days



	//t.tempPerYear(2050);
}
