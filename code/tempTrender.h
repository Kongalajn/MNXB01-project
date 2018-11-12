#ifndef TEMPTRENDER_H
#define TEMPTRENDER_H

#include <string>
#include <fstream>
#include <iostream>
#include <limits>
#include <vector>
#include <cstdlib>

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
		
			double temp = -1;
			float maxTemp = 0, minTemp = 0;
			int year, month, day;
			string sTemp, isOK;
			
			if (file.is_open()) {
					getline(file, sTemp, ';');
					getline(file, isOK);
					
					//Separating the data
					replace(line.begin(), line.end(), '-', ' ');
					replace(line.begin(), line.end(), ';', ' ');
					replace(line.begin(), line.end(), ':', ' ');

					vector<string> elements = split(line.c_str(), ' ');

					//Gget integers from the vector<string>
					year = atoi(elements[0].c_str());
					month = atoi(elements[1].c_str());
					day = atoi(elements[2].c_str());

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
					// Clearing the vector afterwards
					elements.clear();
					
					if(isOK == "G"){ //If the data is deemed correct, check if the current temperature 
									//is higher or lower than the previous maximum/minimum
						if(temp > hottest[year]) {
							hottestTemp[year] = temp;
							hottestDay[year] = day;
						}
					
						if(temp < coldest[year]) {
							coldestTemp[year] = temp;
							coldestDay[year] = day;
						}
					
				}
				
				file.close();
				
				for(int y=1900;y<2050;y++) { //Forms histograms based on previously read data
					
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
