#ifndef TEMPTRENDER_H
#define TEMPTRENDER_H

#include <string>
#include <fstream>
#include <iostream>
#include <limits>

using namespace std;

#include <TH1.h>
#include <TCanvas.h>

class tempTrender {
	public:
	string helpString;
	
	tempTrender(string pathToFile); //Construct using the specified file
	~tempTrender() {} //Destructor
	
	void readFile(string pathToFile) {pathToFile_ = pathToFile;}
	
	void tempOnDayDat(int monthToCalculate, int dayToCalculate, double specTemp = std::numeric_limits<double>::quiet_NaN()) {
		ifstream file(pathToFile_);
		
		if (file.is_open()) {
			TH1I* hist = new TH1I("temperature", "Temperature;Temperature[#circC];Entries", 300, -20, 40);
			hist->SetFillColor(kRed + 1);
			
			int month = -1, day = -1;
			double temp = -1;
			while(file >> helpString) {
				file >> month >> day >> helpString >> temp >> helpString;
				if(month == monthToCalculate and day == dayToCalculate) {
					hist->Fill(temp);
				}
			}
			
			file.close();
			
			if(specTemp == specTemp){
				int binNum = hist->FindBin(specTemp);
				double binEntries = hist->GetEntries();
				double specBinEntries = hist->GetBinContent(binNum);
				double tempProb = specBinEntries/binEntries;
				cout << "Probability of temp being " << specTemp << " degrees: " << tempProb*100 << " %" << endl;
			}

			double mean = hist->GetMean(); //The mean of the distribution
			double stdev = hist->GetRMS(); //The standard deviation
			TCanvas* can = new TCanvas();
			hist->Draw();
				
			cout << "Mean temp: " << mean << endl;
			cout << "Std Dev : " << stdev << endl;
		}
		
		else cout << "Cannot read file format" << endl;
		
	;}
	
	void tempOnDayCsv(int monthToCalculate, int dayToCalculate, double specTemp = std::numeric_limits<double>::quiet_NaN()) {
		ifstream file(pathToFile_);
		
		if (file.is_open()) {
			TH1I* hist = new TH1I("temperature", "Temperature;Temperature[#circC];Entries", 300, -20, 40);
			hist->SetFillColor(kRed + 1);
			
			string sMonthToCalculate = to_string(monthToCalculate);
			string sDayToCalculate = to_string(dayToCalculate);			
			if(monthToCalculate < 10) {
				sMonthToCalculate = "0" + sMonthToCalculate;
			}
			if(dayToCalculate < 10) {
				sDayToCalculate = "0" + sDayToCalculate;					
			}
					
			for(int i=0; i<10; i++) {
				getline(file,helpString);
			}
				
			double avgTemp = 0, temp =0;
			int avgN = 0;
			string year, month, day, sTemp, isOK;
			for(int i=0; i<12; i++) {
				getline(file, year, '-');
				getline(file, month, '-');
				getline(file, day, ';');
				getline(file, helpString, ';');
				getline(file, sTemp, ';');
				getline(file, isOK, ';');
				getline(file, helpString);
				if(isOK == "G"){
					temp = stod(sTemp);
					if(month == sMonthToCalculate && day == sDayToCalculate) {
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
				
			while(getline(file, year, '-')){
				getline(file, month, '-');
				getline(file, day, ';');
				getline(file, helpString, ';');
				getline(file, sTemp, ';');
				getline(file, isOK);
				if(isOK == "G"){
					temp = stod(sTemp);
					if(month == sMonthToCalculate && day == sDayToCalculate) {
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
				
			file.close();
			
			if(specTemp == specTemp){
				int binNum = hist->FindBin(specTemp);
				double binEntries = hist->GetEntries();
				double specBinEntries = hist->GetBinContent(binNum);
				double tempProb = specBinEntries/binEntries;
				cout << "Probability of temp being " << specTemp << " degrees: " << tempProb*100 << " %" << endl;
			}

			double mean = hist->GetMean(); //The mean of the distribution
			double stdev = hist->GetRMS(); //The standard deviation
			TCanvas* can = new TCanvas();
			hist->Draw();
				
			cout << "Mean temp: " << mean << endl;
			cout << "Std Dev : " << stdev << endl;
		}
	
		else cout << "Cannot read file format" << endl;
		
	;} //Make a histogram of the temperature on this day
	//void tempOnDay(int dateToCalculate); //Make a histogram of the temperature on this date
	//void tempPerDay(); //Make a histogram of the average temperature of each day of the year
	//void hotCold(); //Make a histogram of the hottest and coldest day of the year
	//void tempPerYear(int yearToExtrapolate); //Make a histogram of average temperature per year, then fit and extrapolate to the given year

	private:
	string pathToFile_;
};

#endif
