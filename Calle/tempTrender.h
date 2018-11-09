#ifndef TEMPTRENDER_H
#define TEMPTRENDER_H

#include <string>
#include <fstream>
#include <iostream>
#include <algorithm>
#include <limits>
#include <cstdlib>

using namespace std;

#include <TH1.h>
#include <TCanvas.h>
#include <TGraph2D.h>
#include <TStyle.h>
#include <TAxis.h>

class tempTrender {
	public:
	string helpString;
	
	tempTrender(string pathToFile); //Construct using the specified file
	~tempTrender() {} //Destructor
	
	void readFile(string pathToFile) {pathToFile_ = pathToFile;}
	
	
	
	// algorithm from: https://alcor.concordia.ca//~gpkatch/gdate-algorithm.html
	Int_t dayNumber(Int_t year, Int_t month, Int_t day){
		month = (month + 9) % 12;
		year = year - month/10;
		return 365*year + year/4 - year/100 + year/400 + (month*306 + 5)/10 + ( day - 1 );
	;}

	void tempSpectrogram(){
		string line;
		string helpString;
		ifstream file(pathToFile_.c_str());

		TCanvas* c = new TCanvas("c","Graph2D example",0,0,600,400);
		TGraph2D* gr = new TGraph2D();

		Int_t n = 0;
		Double_t year, month, day, temp;
		while(file.good()){
			n++;
			getline(file,line);
			file >> year >> month >> day >> temp >> helpString >> helpString;
			Int_t currentDay = dayNumber(year, month, day) - dayNumber(year, 1, 1) + 1;
			//cout << currentDay << endl;
			cout << n << endl;

			gr->SetPoint(n,year,currentDay,temp);
			
		}
		file.close();

		//gStyle->SetPalette(1);
		
	    gr->Draw("PCOL");
	    gr->GetXaxis()->SetRangeUser(1720,2015);
	;}
	
	
	//void tempOnDay(int dateToCalculate); //Make a histogram of the temperature on this date
	//void tempPerDay(); //Make a histogram of the average temperature of each day of the year
	//void hotCold(); //Make a histogram of the hottest and coldest day of the year
	//void tempPerYear(int yearToExtrapolate); //Make a histogram of average temperature per year, then fit and extrapolate to the given year

	private:
	string pathToFile_;
};

#endif
