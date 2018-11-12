#ifndef TEMPTRENDER_H
#define TEMPTRENDER_H

#include <string>
#include <fstream>
#include <iostream>
#include <sstream>
#include <cstdlib>

using namespace std;

#include <TH1.h>
#include <TCanvas.h>
#include <TLegend.h> 

class tempTrender {
	public:
	
	tempTrender(string pathToFile); //Construct using the specified file
	~tempTrender() {} //Destructor
	
	//Creating a private file path variable
	void readFile(string pathToFile) {pathToFile_ = pathToFile;}
	
	/*
	 Creates a histogram of the temperature on a given day over the years.
		The mean temperature as well as the standard deviation is also 
		computed.
	 
	 Args:
		dayToCalculate (int): Integer between 1-31, the day of the month.
		
	 	monthToCalculate (int): Integer between 1-12, the month.
	 	
		specTemp (double, optional): input to compute probability of temp
			being the input temp on the input day.	 
	*/
	void tempOnDay(int dayToCalculate, int monthToCalculate, double specTemp = std::numeric_limits<double>::quiet_NaN()) {
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

				string histTitle = "Histogram of temperature in " + townName + " on " + sDay.str() + "/" + sMonth.str();
				
				TCanvas* can = new TCanvas("tempOnDay", histTitle.c_str());
				hist->Draw();
				TLegend* legend = new TLegend(0.5,0.16,0.95,0.26);
				legend->AddEntry(hist,histTitle.c_str(),"f");
				legend->SetTextSize(0.025);
				legend->Draw();
				
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
	;}

	void tempPerYear(int yearToExtrapolate)//Make a histogram of average temperature per year, then fit and extrapolate to the given year
	{
		int StartYear = 1722;
		int EndYear = 2013;
		int IsOkay=0, year;
		int i=1, j=1, k=1;
		int bin=1, bin2=1, bin3=1, bincount = 292, bincount2 = 57, bincount3 = 29;
		double tempTot[bincount], meanThree[bincount2]={0}, meanFive[bincount3]={0};
		double DayCount=0, tempTot1=0, tempTot2 = 0, DayCount2=0, mean, testthree=0, testfive=0;
		
		ifstream file(pathToFile_.c_str());
		THStack *histTot = new THStack("temperature","Temperature;Year;Temperature[#circC]"); 
		TH1D* hist2 = new TH1D("temperature", "Temperature;Year;Temperature[#circC]", 292, 1722, 2013);
		hist2->SetFillColor(kRed + 1);
		histTot->Add(hist2);
		TH1D* hist3 = new TH1D("temperature", "Temperature;Year;Temperature[#circC]", 292, 1722, 2013);
		hist3->SetFillColor(kBlue + 1);
		histTot->Add(hist3);
		TH1D* hist4 = new TH1D("temperature", "Temperature;Year;Temperature[#circC]", 292, 1722, 2013);
		hist4->SetFillColor(10);
		histTot->Add(hist4);
		
		TH1D* hist5 = new TH1D("temperature", "Temperature;Year;Temperature[#circC]", bincount2, 1722, 2013);
		hist5->SetFillColor(kRed + 1);
		TH1D* hist6 = new TH1D("temperature", "Temperature;Year;Temperature[#circC]", bincount3, 1722, 2013);
		hist6->SetFillColor(kRed + 1);
		
		TF1* MyFit = new TF1("MyFit", "pol1", 1722, 2013);
		TF1* MyFit2 = new TF1("MyFit2", "pol1", 1722, 2013);
		TF1* MyFunc = new TF1(); 
		TF1* MyFunc2 = new TF1();
		int month = -1, day = -1;
		double temp = -1;
		
		while(StartYear < EndYear) 
		{
			
			while(file >> year) 
			{
				file >> month >> day >> helpString >> temp >> IsOkay;
				if(year == StartYear)
				{
					tempTot1 +=temp;
					tempTot2 +=temp;
					DayCount++;
					DayCount2++;
				
				
				}
				else 
				{
					tempTot[bin] = tempTot1/=DayCount;

					bin++;
					DayCount = 0;
					StartYear++;
				}
			}
			tempTot[bin] = tempTot1/=DayCount;
		}
		
		//Making the Stacked histograms
		mean = tempTot2/DayCount2;
		bin=1;
		while(bin <= bincount)
		{
			if (tempTot[bin] > mean)
			{
				hist2->SetBinContent(bin, tempTot[bin]);
				hist4->SetBinContent(bin, mean);
			}
			else if (tempTot[bin] < mean)
			{
				hist3->SetBinContent(bin, mean);
				hist4->SetBinContent(bin, tempTot[bin]);
					
			}
			else if(tempTot[bin] == mean)
			{
				hist4->SetBinContent(bin, mean);
			}
			bin++;
		}
		
		//Trying to get the smooth curves
		bin=1;
		while(bin2 < bincount2) //While loop for average over three years
		{
			for(i=0;i<5;i++)
			{
				meanThree[bin2]+=tempTot[bin+i];		
			}
		testthree = meanThree[bin2];
		testthree =testthree/5.0;
		hist5->SetBinContent(bin2, testthree);
		bin+=5;
		bin2++;
		}
		
		bin=1;
		while(bin3 < bincount3) //While loop for average over 5  years
		{
			for(i=0;i<10;i++)
			{
				meanFive[bin3]+=tempTot[bin+i];
			}
		testfive = meanFive[bin3];
		testfive =testfive/10.0;
		hist6->SetBinContent(bin3, testfive);
		bin+=10;
		bin3++;
		}
		
		TCanvas* can2 = new TCanvas();
		histTot->Draw("nostack"); //Draws the stacked histograms
			
		TGraph* graph = new TGraph();
		graph->SetLineWidth(3);
		for(int bin = 1; bin < hist5->GetNbinsX(); ++bin) 
			{
			graph->Expand(graph->GetN() + 1, 100);
			graph->SetPoint(graph->GetN(), hist5->GetBinCenter(bin),
			hist5->GetBinContent(bin));
			}
		graph->Draw("SAME C"); //Draws the three year average curve on the stacked histograms
		
		TGraph* graph2 = new TGraph();
		graph2->SetLineWidth(5);
		for(int bin = 1; bin < hist6->GetNbinsX(); ++bin) 
			{
			graph2->Expand(graph2->GetN() + 1, 100);
			graph2->SetPoint(graph2->GetN(), hist6->GetBinCenter(bin),
			hist6->GetBinContent(bin));
			}
		graph2->Draw("SAME C"); //Draws the five year average curve on the stacked histograms
		

		
		
		
		TCanvas* can3 = new TCanvas();
		//hist5->Draw();
		hist5->Fit(MyFit, "Q");
		
		TCanvas* can4 = new TCanvas();
		//hist6->Draw();
		hist6->Fit(MyFit2, "Q");
	
	
	} 
	
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

	
	//void hotCold(); //Make a histogram of the hottest and coldest day of the year

	private:
	string pathToFile_;
};

#endif
