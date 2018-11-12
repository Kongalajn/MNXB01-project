#ifndef TEMPTRENDER_H
#define TEMPTRENDER_H

#include <string>
#include <fstream>
#include <iostream>
#include <algorithm>
#include <limits>
#include <vector>
#include <cstdlib>

using namespace std;

#include <TH1.h>
#include <TCanvas.h>
#include <TGraph2D.h>
#include <TStyle.h>
#include <TAxis.h>
#include <TROOT.h>

class tempTrender {
	public:
	string helpString;
	
	tempTrender(string pathToFile); //Construct using the specified file
	~tempTrender() {} //Destructor
	
	void readFile(string pathToFile) {pathToFile_ = pathToFile;}
	
	// algorithm from: https://alcor.concordia.ca//~gpkatch/gdate-algorithm.html
	// Returns the day of the year starting from the 1st of january being day 1
	Int_t dayNumber(Int_t year, Int_t month, Int_t day){
		month = (month + 9) % 12;
		year = year - month/10;
		return 365*year + year/4 - year/100 + year/400 + (month*306 + 5)/10 + ( day - 1 );
	;}


	// Input is a string which you want to split into a vector.
	// The seperation is defined in the second argument
	vector<string> split(const char *str, char c = ' '){
	    vector<string> result;

	    do
	    {
	        const char *begin = str;

	        while(*str != c && *str)
	            str++;

	        result.push_back(string(begin, str));
	    } while (0 != *str++);

	    return result;
	;}


	// First choose which year like startYear <= year < endYear
	// Then choose if the file is the uppsala file with true, else its false
	// Then if you have data with many timepoints during the days you can specify which
	// time to look at. If you write 100, all the measurments will be included.

	void DataView3D(Int_t startYear, Int_t endYear, bool isUppsala, Int_t startTime=666, Int_t endTime=666){

		// Making sure that there are no old objects
		delete gROOT->FindObject("c");
		delete gROOT->FindObject("gr");

		string line;
		string helpString;
		ifstream file(pathToFile_.c_str());

		TCanvas* c = new TCanvas("c"," ",10,20,600,400);
		TGraph2D* gr = new TGraph2D(); 


		if(isUppsala){
			Int_t n = 0;
			Double_t year, month, day, temp;
			while(file.good()){
				n++;
				getline(file,line);
				file >> year >> month >> day >> temp >> helpString >> helpString;
				Int_t currentDay = dayNumber(year, month, day) - dayNumber(year, 1, 1) + 1;
				//cout << currentDay << endl;
				//cout << n << endl;

				if(startYear <= year && year < endYear){
					gr->SetPoint(n,year,currentDay,temp);
				}
			}
		}
		else{
			Int_t n = 0, N = 0;
			Double_t year, month, day, time, temp;
			while(file.good()){
				n++;
				getline(file,line);

				// A very ugly way of ignoring the initial "junk" in the data. In some files
				// there will be a couple of missed datapoints because of this.
				if(n > 15){
					N++;
					//below are the steps to seperate out the data from the lines
					replace(line.begin(), line.end(), '-', ' ');
					replace(line.begin(), line.end(), ';', ' ');
					replace(line.begin(), line.end(), ':', ' ');

					vector<string> elements = split(line.c_str(), ' ');

					// The atof function is used to get doubles from the vector<string>
					year = atof(elements[0].c_str());
					month = atof(elements[1].c_str());
					day = atof(elements[2].c_str());
					time = atof(elements[3].c_str());

					// Since we replace the '-' sign with ' ', the negative values will be shifted one index
					// They can be retrived by the following logic where if both index 6 & 7 are equal,
					// the temperature must be zero.
					if(atof(elements[6].c_str()) > 0){
						temp = atof(elements[6].c_str());
					}
					else if(atof(elements[6].c_str()) == atof(elements[7].c_str())){
						temp = 0;
					}
					else{
						temp = -atof(elements[7].c_str());
					}
					// Clearing the vector to make sure there is no junk left
					elements.clear();

					Int_t currentDay = dayNumber(year, month, day) - dayNumber(year, 1, 1) + 1;

					// just to make sure it stops the iteration instead of stopping the plotting
					// but continuing the iterations. This is to prevent the atof function to
					// go crazy.
					if(year == endYear){
						break;
					}

					
					if(startYear <= year && year < endYear && startTime <= time && time <= endTime){
						gr->SetPoint(N,year,currentDay,temp);
					}
				}
			}
		}

		file.close();
		
		gStyle->SetPalette(1);
		
		// Very stupid attempt of making the SetRangeUser actually show the right range...
		Int_t stupidVar = 10;
		
		if(endYear - startYear < 60){
			stupidVar = (endYear - startYear)/2;
		}

		//gr->Draw("PCOL");

		// This SetRangeUser thing really is a mystery to me. It lives its own life. 
		gr->GetXaxis()->SetRangeUser(startYear+stupidVar,endYear-stupidVar);
		gr->GetXaxis()->SetTitle("Year");
		gr->GetYaxis()->SetTitle("Day of the Year");
		gr->GetZaxis()->SetTitle("Degrees Celsius");
		gr->SetMarkerStyle(7);
	        gr->Draw("PCOL");
	        //c->Update();
	;}

	//void tempOnDay(int dateToCalculate); //Make a histogram of the temperature on this date
	//void tempPerDay(); //Make a histogram of the average temperature of each day of the year
	//void hotCold(); //Make a histogram of the hottest and coldest day of the year
	//void tempPerYear(int yearToExtrapolate); //Make a histogram of average temperature per year, then fit and extrapolate to the given year

	private:
	string pathToFile_;
};

#endif
