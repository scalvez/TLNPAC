#include "TGraphErrors.h"
#include "TCanvas.h" 
#include "TAxis.h"


#include <fstream>
#include <iostream>
#include <vector>
#include <string>
#include "math.h"

using namespace std;

#define DEBUG 0

int main() {  
  // Set the calibration data files for all pm
  vector<int> calib_type; calib_type.clear(); //0:tension, 1:threshold, 2:width
  vector<string> calib_input; calib_input.clear();
  vector<string> calib_output; calib_output.clear();
  vector<string> calib_title; calib_title.clear();
  vector<string> calib_variable; calib_variable.clear();
  vector<double> calib_errx; calib_errx.clear();

  calib_errx.push_back(0.001); calib_variable.push_back("Tension (kV)"); calib_type.push_back(0); calib_input.push_back("data/CalibS2.txt");  calib_output.push_back("plot/CalibS2.pdf");  calib_title.push_back("PMCalibration : S2");
  calib_errx.push_back(0.001); calib_variable.push_back("Tension (kV)"); calib_type.push_back(0); calib_input.push_back("data/CalibS1.txt");  calib_output.push_back("plot/CalibS1.pdf");  calib_title.push_back("PMCalibration : S1");
  calib_errx.push_back(0.001); calib_variable.push_back("Tension (kV)"); calib_type.push_back(0); calib_input.push_back("data/CalibPM3.txt"); calib_output.push_back("plot/CalibPM3.pdf"); calib_title.push_back("PMCalibration : PM3");
  calib_errx.push_back(0.001); calib_variable.push_back("Tension (kV)"); calib_type.push_back(0); calib_input.push_back("data/CalibPM4.txt"); calib_output.push_back("plot/CalibPM4.pdf"); calib_title.push_back("PM Calibration : PM4");
  calib_errx.push_back(0.001); calib_variable.push_back("Tension (kV)"); calib_type.push_back(0); calib_input.push_back("data/CalibPM5.txt"); calib_output.push_back("plot/CalibPM5.pdf"); calib_title.push_back("PM Calibration : PM5");
  calib_errx.push_back(1); calib_variable.push_back("Threshold (mV)"); calib_type.push_back(1); calib_input.push_back("data/CalibThreshold.txt"); calib_output.push_back("plot/CalibThreshold.pdf"); calib_title.push_back("Threshold Calibration");
  calib_errx.push_back(0.5); calib_variable.push_back("Width (ns)"); calib_type.push_back(2); calib_input.push_back("data/CalibWidth.txt"); calib_output.push_back("plot/CalibWidth.pdf"); calib_title.push_back("Width Calibration");

  //Declare variables of the study
  TGraphErrors *pm_eff=0;
  TCanvas* canvas=new TCanvas("canvas","canvas",800,600);
  double N_in,N_trig,tension,epsilon,errepsilon;
  fstream datastream;
  string buffer;

  
  for (unsigned int pm=0; pm<calib_input.size(); pm++) {
    //    if (calib_type[pm]<2) continue;
    datastream.open(calib_input[pm].c_str(),fstream::in); getline(datastream,buffer);
    if (DEBUG) cout << buffer << endl;
    pm_eff=new TGraphErrors(0);
    //read data from .txt files
    while (datastream >> tension >> N_in >> N_trig ) {
      //if (DEBUG) cout << "tension : " << tension << endl;
      pm_eff->Set(pm_eff->GetN()+1);
      epsilon=N_trig/N_in; 
      errepsilon=sqrt(epsilon/N_in*(1+epsilon));
      pm_eff->SetPoint(pm_eff->GetN()-1,tension,epsilon);
      //set statistical errors      
      pm_eff->SetPointError(pm_eff->GetN()-1,calib_errx[pm],errepsilon);
    }
    if (DEBUG==1)  cout << "Number of points : " << pm_eff->GetN() << endl;
    
    //set graph titles
    pm_eff->SetTitle(calib_title[pm].c_str());
    pm_eff->GetXaxis()->SetTitle(calib_variable[pm].c_str());
    pm_eff->GetYaxis()->SetTitle("N_{trigg}/N_{in}");
    pm_eff->SetMarkerStyle(8);
    pm_eff->SetLineColor(kRed);
    pm_eff->SetMarkerColor(kRed);
    pm_eff->SetMarkerSize(0.3);
    
    //drawing, saving and cleaning pointers
    pm_eff->Draw("AP");
    canvas->SaveAs(calib_output[pm].c_str());
    pm_eff->Delete();
    datastream.close();
  }
 
  cout << "Went up to the end" << endl;
  return 0;
}
