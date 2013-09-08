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
  vector<int> calib_type; calib_type.clear(); //0:tension, 
  vector<string> calib_input; calib_input.clear();
  vector<string> calib_output; calib_output.clear();
  vector<string> calib_title; calib_title.clear();

calib_type.push_back(0); calib_input.push_back("data/CalibS2.txt");  calib_output.push_back("plot/CalibS2.pdf");  calib_title.push_back("PMCalibration : S2");
calib_type.push_back(0); calib_input.push_back("data/CalibS1.txt");  calib_output.push_back("plot/CalibS1.pdf");  calib_title.push_back("PMCalibration : S1");
calib_type.push_back(0); calib_input.push_back("data/CalibPM3.txt"); calib_output.push_back("plot/CalibPM3.pdf"); calib_title.push_back("PMCalibration : PM3");
calib_type.push_back(0); calib_input.push_back("data/CalibPM4.txt"); calib_output.push_back("plot/CalibPM4.pdf"); calib_title.push_back("PM Calibration : PM4");
calib_type.push_back(0); calib_input.push_back("data/CalibPM5.txt"); calib_output.push_back("plot/CalibPM5.pdf"); calib_title.push_back("PM Calibration : PM5");

  //Declare variables of the study
  TGraphErrors *pm_eff=0;
  TCanvas* canvas=new TCanvas("canvas","canvas",800,600);
  double N_in,N_trig,tension,epsilon,errepsilon;
  fstream datastream;
  string buffer;

  
  for (unsigned int pm=0; pm<calib_input.size(); pm++) {
    datastream.open(calib_input[pm].c_str(),fstream::in); getline(datastream,buffer);
    pm_eff=new TGraphErrors(0);
    
    //read data from .txt files
    while (datastream >> tension >> N_in >> N_trig ) {
      pm_eff->Set(pm_eff->GetN()+1);
      epsilon=N_trig/N_in; 
      errepsilon=sqrt(epsilon/N_in*(1+epsilon));
      if ( DEBUG==1 ) cout << "N points : " << pm_eff->GetN() << endl << "eff : " << epsilon << endl << "Deff : " << errepsilon << endl ; 
      pm_eff->SetPoint(pm_eff->GetN()-1,tension,epsilon);
      pm_eff->SetPointError(pm_eff->GetN()-1,1e-3,errepsilon);
    }
    if (DEBUG==1)  cout << "Number of points : " << pm_eff->GetN() << endl;
    
    //set graph titles
    pm_eff->SetTitle(calib_title[pm].c_str());
    pm_eff->GetXaxis()->SetTitle("PM Tension (kV)");
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
