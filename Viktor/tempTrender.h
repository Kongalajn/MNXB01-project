#ifndef TEMPTRENDER_H
#define TEMPTRENDER_H

#include <string>
#include <fstream>
#include <iostream>
#include <limits>
#include <cstdlib>

using namespace std;

#include <TH1.h>
#include <TH2.h>
#include <TGraph.h>
#include <THStack.h>
#include <TCanvas.h>

class tempTrender {	
	public:
	string helpString;
	
	
	tempTrender(string pathToFile); //Construct using the specified file
	~tempTrender() {} //Destructor
	
	void readFile(string pathToFile) {pathToFile_ = pathToFile;}
	
	void tempOnDay(int monthToCalculate, int dayToCalculate) {
		ifstream file(pathToFile_.c_str());
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
		

		double mean = hist->GetMean(); //The mean of the distribution
		double stdev = hist->GetRMS(); //The standard deviation
		TCanvas* can = new TCanvas();
		hist->Draw();
			
		cout << "Mean temp: " << mean << endl;
		cout << "Std Dev : " << stdev << endl;
		
	;} //Make a histogram of the temperature on this day
	//void tempOnDay(int dateToCalculate); //Make a histogram of the temperature on this date
	//void tempPerDay(); //Make a histogram of the average temperature of each day of the year
	//void hotCold(); //Make a histogram of the hottest and coldest day of the year
	void tempPerYear(int yearToExtrapolate)//Make a histogram of average temperature per year, then fit and extrapolate to the given year
	{
		int StartYear = 1722;
		int EndYear = 2013;
		int IsOkay=0, year;
		int i=1, j=1, k=1;
		int bin=1, bin2=1, bin3=1, bincount = 292, bincount2 = 97, bincount3 = 58;
		double tempTot[bincount], meanThree[bincount2]={0}, meanFive[bincount3]={0};
		double DayCount=0, tempTot1=0, tempTot2 = 0, DayCount2=0, mean, testthree=0, testfive=0;
		
		cout << *meanThree << endl;
		
		ifstream file(pathToFile_.c_str());
		THStack *histTot = new THStack("histTot",""); 
		TH1I* hist2 = new TH1I("temperature", "Temperature;Year;Temperature[#circC]", 292, 1722, 2013);
		hist2->SetFillColor(kRed + 1);
		histTot->Add(hist2);
		TH1I* hist3 = new TH1I("temperature", "Temperature;Year;Temperature[#circC]", 292, 1722, 2013);
		hist3->SetFillColor(kBlue + 1);
		histTot->Add(hist3);
		TH1I* hist4 = new TH1I("temperature", "Temperature;Year;Temperature[#circC]", 292, 1722, 2013);
		hist4->SetFillColor(10);
		histTot->Add(hist4);
		
		TH1I* hist5 = new TH1I("temperature", "Temperature;Year;Temperature[#circC]", 97, 1722, 2013);
		hist5->SetFillColor(kRed + 1);
		TH1I* hist6 = new TH1I("temperature", "Temperature;Year;Temperature[#circC]", 58, 1722, 2013);
		hist6->SetFillColor(kRed + 1);
		
		 //TF1* MyFit = new TF1("MyFit", "pol2", 1722, 2013);
		 //TF1* MyFit2 = new TF1("MyFit2", "pol2", 1722, 2013);
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
		while(bin2 < bincount2+1) //While loop for average over three years
		{
			for(i=0;i<3;i++)
			{
				cout<<"bin+i: "<<bin+i<<endl<<"tempTot:"<<tempTot[bin+i]<<endl;
				meanThree[bin2]+=tempTot[bin+i];
				cout<<"MeanThree[bin2]: "<<meanThree[bin2]<<endl;		
			}
		testthree = meanThree[bin2];
		testthree =testthree/3.0;
		cout<<"bin2: "<<bin2<<endl<<"meanThree: "<<testthree<<endl;
		hist5->SetBinContent(bin2, testthree);
		bin+=3;
		bin2++;
		}
		
		bin=1;
		while(bin3 < bincount3+1) //While loop for average over 5  years
		{
			for(i=0;i<5;i++)
			{
				meanFive[bin3]+=tempTot[bin+i];
			}
		testfive = meanFive[bin3];
		testfive =testfive/5.0;
		//cout<<"bin3: "<<bin3<<endl<<"meanfive("<<bin3<<"): "<<testfive<<endl;
		hist6->SetBinContent(bin3, testfive);
		bin+=5;
		bin3++;
		}
		
		cout<<mean<<endl;
		TCanvas* can2 = new TCanvas();
		histTot->Draw("nostack"); //Draws the stacked histograms
			
		TGraph* graph = new TGraph();
		for(int bin = 1; bin < hist5->GetNbinsX(); ++bin) 
			{
			graph->Expand(graph->GetN() + 1, 100);
			graph->SetPoint(graph->GetN(), hist5->GetBinCenter(bin),
			hist5->GetBinContent(bin));
			}
		graph->Draw("SAME C"); //Draws the three year average curve
		
		TGraph* graph2 = new TGraph();
		for(int bin = 1; bin < hist6->GetNbinsX(); ++bin) 
			{
			graph2->Expand(graph2->GetN() + 1, 100);
			graph2->SetPoint(graph2->GetN(), hist6->GetBinCenter(bin),
			hist6->GetBinContent(bin));
			}
		graph2->Draw("SAME C"); //Draws the five year average curve
		
		TCanvas* can3 = new TCanvas();
		hist5->Draw();
		TCanvas* can4 = new TCanvas();
		hist6->Draw();
		
		//hist5->Fit(MyFit);
		//MyFit->Draw();
	
	
	} 
	private:
	string pathToFile_;
	
	};


#endif
