#include "TGraphErrors.h"
#include "TCanvas.h" 
#include "TAxis.h"
#include "TMultiGraph.h"
#include "TLegend.h"

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
  vector<string> calib_output_pdf; calib_output_pdf.clear();
  vector<string> calib_output_root; calib_output_root.clear();
  vector<string> calib_title; calib_title.clear();
  vector<string> calib_variable; calib_variable.clear();
  vector<double> calib_errx; calib_errx.clear();

  calib_errx.push_back(0.001); calib_variable.push_back("Tension (kV)"); calib_input.push_back("data/CalibPM4.txt"); calib_output_pdf.push_back("plot/CalibPM4.pdf");calib_output_root.push_back("plot/CalibPM4.root"); calib_title.push_back("PM4");
  calib_errx.push_back(0.001); calib_variable.push_back("Tension (kV)");  calib_input.push_back("data/CalibPM5.txt"); calib_output_pdf.push_back("plot/CalibPM5.pdf");calib_output_root.push_back("plot/CalibPM5.root"); calib_title.push_back("PM5");
  calib_errx.push_back(0.001); calib_variable.push_back("Tension (kV)");  calib_input.push_back("data/CalibR_1.txt"); calib_output_pdf.push_back("plot/CalibR_1.pdf");calib_output_root.push_back("plot/CalibR_1.root"); calib_title.push_back("R-1");
  calib_errx.push_back(0.001); calib_variable.push_back("Tension (kV)");  calib_input.push_back("data/CalibR1.txt"); calib_output_pdf.push_back("plot/CalibR1.pdf"); calib_output_root.push_back("plot/CalibR1.root");calib_title.push_back("R1");
  calib_errx.push_back(0.001); calib_variable.push_back("Tension (kV)");  calib_input.push_back("data/CalibR2.txt"); calib_output_pdf.push_back("plot/CalibR2.pdf");calib_output_root.push_back("plot/CalibR2.root"); calib_title.push_back("R2");
  

  //Declare variables of the study
  TGraphErrors *pm_eff[calib_input.size()];
  TCanvas* canvas=new TCanvas("canvas","canvas",800,600);
  TMultiGraph* commonGraph=new TMultiGraph();
  TLegend *legend=new TLegend(0.15,0.7,0.3,0.9);
  //  legend->SetFontColor(0);

  //  commonGraph->SetTitle("Common Calibration");
  double time,events,tension,freq, errfreq;
  fstream datastream;
  string buffer;

  
  for (unsigned int pm=0; pm<calib_input.size(); pm++) {
    //    if (calib_type[pm]<2) continue;
    datastream.open(calib_input[pm].c_str(),fstream::in); getline(datastream,buffer);
    if (DEBUG) cout << buffer << endl;
    pm_eff[pm]=new TGraphErrors(0);
    //read data from .txt files
    while (datastream >> tension >> time >> events ) {
      //if (DEBUG) cout << "tension : " << tension << endl;
      freq=events/time;
      errfreq=sqrt(5)*freq/time;
      pm_eff[pm]->Set(pm_eff[pm]->GetN()+1);
      pm_eff[pm]->SetPoint(pm_eff[pm]->GetN()-1,tension,freq);
      //set statistical errors      
      pm_eff[pm]->SetPointError(pm_eff[pm]->GetN()-1,calib_errx[pm],errfreq);
    }
    if (DEBUG==1)  cout << "Number of points : " << pm_eff[pm]->GetN() << endl;
    
    //set graph titles
    pm_eff[pm]->SetTitle(calib_title[pm].c_str());
    pm_eff[pm]->GetXaxis()->SetTitle(calib_variable[pm].c_str());
    pm_eff[pm]->GetYaxis()->SetTitle("events frequency (Hz)");
    pm_eff[pm]->SetMarkerStyle(8);
    pm_eff[pm]->SetLineColor(kRed);
    pm_eff[pm]->SetMarkerColor(kRed);
    pm_eff[pm]->SetMarkerSize(1);
    
    //drawing, saving and cleaning pointers
    pm_eff[pm]->Draw("AP");
    canvas->SaveAs(calib_output_pdf[pm].c_str());
    canvas->SaveAs(calib_output_root[pm].c_str());

    pm_eff[pm]->SetLineColor(1+pm);
    pm_eff[pm]->SetMarkerColor(1+pm);
    commonGraph->Add(pm_eff[pm]);
    legend->AddEntry(pm_eff[pm],calib_title[pm].c_str(),"lpe");    
    datastream.close();
  }

  //Draw Multigraph and set titles
  canvas->Clear();
  commonGraph->Draw("AP");
  legend->Draw();
  commonGraph->SetTitle("PM Calibration");
  commonGraph->GetXaxis()->SetTitle(calib_variable[0].c_str());
  commonGraph->GetYaxis()->SetTitle("Event Frequency (Hz)");
  canvas->Update(); 
  
  canvas->SaveAs("plot/CommonCalib.pdf") ;
  canvas->SaveAs("plot/CommonCalib.root") ;

  cout << "Went up to the end" << endl;
  return 0;
}
