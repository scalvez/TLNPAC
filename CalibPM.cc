#include <fstream>


int main() {  
  fstream datastream;
  datastream.open("R=1M_FFT.txt",fstream::in);
  gStyle->SetOptTitle(0);
  gStyle->SetOptStat(0);
  double x,y,errx,erry;
  int counter=0;
  
  TGraphErrors* graph=new TGraphErrors(0);
  graph->GetXaxis()->SetTitle("");
  graph->GetYaxis()->SetTitle("");
  graph->SetMarkerStyle(8);
  graph->SetLineColor(kRed);
  graph->SetMarkerColor(kRed);
  graph->SetMarkerSize(0.3);
  
  while (datastream >> x >> y ) {
     graph->Set(graph->GetN()+1);
    graph->SetPoint(counter,x,y);
    counter++;
  }
 
  cout << graph->GetN() << endl;

 TCanvas* canvas=new TCanvas("canvas","canvas",800,600);
 canvas->SetLogy(1);
 graph->Draw("AP");
 // canvas->SaveAs("");
}
