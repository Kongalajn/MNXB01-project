#ifndef TEMPTRENDER_H
#define TEMPTRENDER_H

#include <string>
#include <fstream>
#include <iostream>
#include <limits>
#include <map>

using namespace std;

#include <TH1.h>
#include <TH2.h>
#include <TGraph.h>
#include <THStack.h>
#include <TCanvas.h>

class tempTrender {
	
	public:
	
	tempTrender(std::string filePath); //Construct using the specified file
	~tempTrender() {} //Destructor

	void readFile(string pathToFile) {pathToFile_ = pathToFile;}

	void hotCold(int year, int day, float hot, float cold){ //Make a histogram of the hottest and coldest day of the year
		
		int hottest[3000], hottestTemp[3000], hottestDay[3000], coldest[3000], coldestTemp[3000], coldestDay[3000];
		//int myint = stoi(day)
		
		for(int i = 0; i <3000; i++) { //Reset temperatures
			hottestTemp[i] = -1000; // really cold
			coldestTemp[i] = 1000; // really hot
		}
	
		if(pathToFile_.substr(pathToFile_.length() - 4) == ".dat" || pathToFile_.substr(pathToFile_.length() - 4) == ".csv"){
	
			ifstream file(pathToFile_);
			
			THStack *histTot = new THStack("histTot","");
			TH1I* histHot = new TH1I("temperature", "Temperature;Year;Temperature[#circC]", 366, 1, 366);
			histHot->SetFillColor(kRed + 1);
			histTot->Add(histHot);
			TH1I* histCold = new TH1I("temperature", "Temperature;Year;Temperature[#circC]", 366, 1, 366);
			histCold->SetFillColor(kBlue + 1);
			histTot->Add(histCold);
		
			int monthDayOffset[] = { 0, 0, 31, 60, 91, 121, 152, 182, 213, 244, 274, 305, 335};
			double temp = -1;
			float maxTemp = 0, minTemp = 0;
			string year, month, day, sTemp, isOK, helpString;
			
			if (file.is_open()) {
			
				while(getline(file, year, '-')){
					getline(file, month, '-');
					getline(file, day, ';');
					getline(file, helpString, ';');
					getline(file, sTemp, ';');
					getline(file, isOK);
					if(isOK == "G"){
						temp = stod(sTemp);
					
						if(temp > hottest[year]) {
							hottestTemp[year] = temp;
							hottestDay[year] =  monthDayOffset[atoi(month)] + atoi(day);
						}
					
						if(temp < coldest[year]) {
							coldestTemp[year] = temp;
							coldestDay[year] =  monthDayOffset[atoi(month)] + atoi(day);
						}
					}
				}
				
				file.close();
				
				for(int y=1900;y<2050;y++) {
					
					if (hottestTemp[y] != -1000) {
						histHot->Fill(hottestDay[y]);
					}
					
					if (coldestTemp[y] != 1000) {
						histCold->Fill(coldestDay[y]);
					}
				}
				
				TCanvas* can2 = new Tcanvas();
				histTot-> Draw();
			
			}
			
			else {cout << "Unable to open file" << endl;}
		
		}
		
		private:
	}
};

#endif
