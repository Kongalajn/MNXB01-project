#ifndef TEMPTRENDER_H
#define TEMPTRENDER_H

#include <string>
#include <fstream>
#include <iostream>
#include <limits>
#include <sstream>
#include <cstdlib>

using namespace std;

#include <TH1.h>
#include <TCanvas.h>

class tempTrender {
	public:
	
	tempTrender(string pathToFile); //Construct using the specified file
	~tempTrender() {} //Destructor
	
	//Creating a private file path variable
	void readFile(string pathToFile) {pathToFile_ = pathToFile;}
	
	/*
	 Creates a histogram of the temperature on a given day
	 
	*/
	void tempOnDay(int monthToCalculate, int dayToCalculate, double specTemp = std::numeric_limits<double>::quiet_NaN()) {
		// Checking if the file format is correct
		string fileType = pathToFile_.substr(pathToFile_.length() - 4);
		if(fileType == ".dat" || fileType  == ".csv"){
			
			ifstream file (pathToFile_.c_str()); 
			
			if (file.is_open()) {
				
				TH1I* hist = new TH1I("temperature", "Temperature;Temperature[#circC];Entries", 300, -20, 40);
				hist->SetFillColor(kRed + 1);
				
				string helpString, townName;
				double temp = -1;
				
				//Converting input date into string 
				string sMonthToCalculate, sDayToCalculate;
				stringstream sMonth, sDay;
				sMonth << monthToCalculate;
				sDay << dayToCalculate;
				sMonthToCalculate = sMonth.str();
				sDayToCalculate = sDay.str();
			
				//Creating histrogram if loaded file is of the type .dat
				if(fileType == ".dat"){
					townName = "Uppsala";
					
					int month = -1, day = -1;
					while(file >> helpString) {
						file >> month >> day >> helpString >> temp >> helpString;
						if(month == monthToCalculate and day == dayToCalculate) {
							hist->Fill(temp);
						}
					}
				}
				
				//Creating histrogram if loaded file is of the type .csv
				else if(fileType == ".csv"){
					//Extracting the town name from the file path
					size_t strSearch = pathToFile_.rfind("_");
					size_t strSearch2 = pathToFile_.rfind(".");					
					townName = pathToFile_.substr(strSearch+1, strSearch2-strSearch-1);
					
					//Making the input compatible with the data
					if(monthToCalculate < 10) {
						sMonthToCalculate = "0" + sMonthToCalculate;
					}
					if(dayToCalculate < 10) {
						sDayToCalculate = "0" + sDayToCalculate;					
					}
					
					string dateString;					
					//Sorting out the initial description notes
					while(dateString != "Datum") {
						getline(file,helpString);
						dateString = helpString.substr(0,5);
					}
				
					double avgTemp = 0;
					int avgN = 0;
					string year, month, day, sTemp, isOK;
					//Reading the data that is on the same lines as description notes
					for(int i=0; i<12; i++) {
						getline(file, year, '-');
						getline(file, month, '-');
						getline(file, day, ';');
						getline(file, helpString, ';');
						getline(file, sTemp, ';');
						getline(file, isOK, ';');
						getline(file, helpString);
						//Checking if data is good
						if(isOK == "G"){
							//Calculating the average temp for the input day and putting it into the histogram
							if(month == sMonthToCalculate && day == sDayToCalculate) {
								stringstream(sTemp) >> temp;
								avgTemp += temp;
								avgN++;
							}
							else if(avgN != 0){
								avgTemp /= avgN;
								hist->Fill(avgTemp);
								avgN = 0;
								avgTemp = 0;
							}
						}
					} 
					
					//Reading the remaining data
					while(getline(file, year, '-')){
						getline(file, month, '-');
						getline(file, day, ';');
						getline(file, helpString, ';');
						getline(file, sTemp, ';');
						getline(file, isOK);
						//Checking if data is good
						if(isOK == "G"){
							//Calculating the average temp for the input day and putting it into the histogram
							if(month == sMonthToCalculate && day == sDayToCalculate) {
								stringstream(sTemp) >> temp;
								avgTemp += temp;
								avgN++;
							}
							else if(avgN != 0){
								avgTemp /= avgN;
								hist->Fill(avgTemp);
								avgN = 0;
								avgTemp = 0;
							}
						}
					}
				}
				
				file.close();
				
				//Calculates the probability of the temperature being the input temp on the input date
				if(specTemp == specTemp){
					int binNum = hist->FindBin(specTemp);
					double binEntries = hist->GetEntries();
					double specBinEntries = hist->GetBinContent(binNum);
					double tempProb = specBinEntries/binEntries;
					if(specTemp == 1){
						cout << "Probability of temperature being " << specTemp << " degree on " << dayToCalculate << "/" << monthToCalculate << " in " << townName << " is: " << tempProb*100 << " %" << endl;
					}
					else {
						cout << "Probability of temperature being " << specTemp << " degrees on " << dayToCalculate << "/" << monthToCalculate << " in " << townName << " is: " << tempProb*100 << " %" << endl;
					}
				}

				string histTitle = "Temperature in " + townName + " on " + sDay.str() + "/" + sMonth.str();
				hist->SetTitle(histTitle.c_str());
				TCanvas* can = new TCanvas("tempOnDay", histTitle.c_str());
				hist->Draw();
				
				double mean = hist->GetMean(); //The mean of the distribution
				double stdev = hist->GetRMS(); //The standard deviation
					
				if(mean == 1) {
					cout << "Mean temperature on " << dayToCalculate << "/" << monthToCalculate << " in " << townName << " is: " << mean << " degree" << endl;
				}
				else cout << "Mean temperature on " << dayToCalculate << "/" << monthToCalculate << " in " << townName << " is: " << mean << " degrees" << endl;
				
				if(stdev == 1) {
					cout << "Standard Deviation: " << stdev << " degrees" << endl;
				}
				else cout << "Standard Deviation: " << stdev << " degrees" << endl;
			}
			else cout << "Unable to open file" << endl;
		}
		else cout << "Cannot read file format" << endl;
	;}//Make a histogram of the temperature on this day
	//void tempOnDay(int dateToCalculate); //Make a histogram of the temperature on this date
	//void tempPerDay(); //Make a histogram of the average temperature of each day of the year
	//void hotCold(); //Make a histogram of the hottest and coldest day of the year
	//void tempPerYear(int yearToExtrapolate); //Make a histogram of average temperature per year, then fit and extrapolate to the given year

	private:
	string pathToFile_;
};

#endif
