/// my own heades
#include "DecodeMatacq_TL.cc"

//#pragma link C++ class MPulse+;
//#pragma link C++ class std::vector<MPulse>+;


/// ROOT headers
#include <TCanvas.h>
#include <TROOT.h>
#include <TF1.h>
#include <TH2.h>
#include <TMath.h>
#include <TTree.h>
#include <TFile.h>
#include <TSystem.h>

/// C++ headers
#include <string>
#include <iostream>
using namespace std;	






int DrawEventsByEvents(  string fileData) {
  //  gROOT->SetStyle("plain");
  gSystem->Load("DecodeMatacq_TL_cc.so");
 
 
  MRun myRun(fileData.c_str());

  TCanvas *can = new TCanvas("canvas","canvas",800,800);
  can->SetGridx();
  can->SetGridy();

  MEvent *anEvent = 0;
  unsigned nevt = 0;
  while( ( anEvent = myRun.GetNextEvent()) && nevt < 100000 ) {
    
    can->cd();
    for( int ich = 0; ich < 4; ich++ ) {
      string option = "" ;
      if( ich > 0 ) option += "same";
      
      //      anEvent->DrawChannel(ich,option,-15000,100);
      anEvent->DrawChannel(ich,option);
    }
    can->Update();


    cout << "Hit key to get next event" << endl;
    getchar();
    nevt++;
  }

  return 1;
}



int CaracterizeFirstPulse(  string fileData ) {
  //  gROOT->SetStyle("plain");
  gSystem->Load("DecodeMatacq_TL_cc.so");
 

  MRun myRun0(fileData.c_str());
  MRun myRun1(fileData.c_str());


  TH1F *hT = 0; /// time   first pulse
  TH1F *hQ = 0; /// charge first pulse
  TH1F *hV = 0; /// amplitude first pulse 

  MEvent *anEvent = 0;
  unsigned nevt = 0;

  for(int i = 0 ; i<1; i++){
    anEvent=myRun0.GetNextEvent();
    if ( anEvent==0 ) {
      cerr << "No event in Dataset" << endl;
      return -1;
    }

    while( !anEvent == 0 ) {
      //      if(i==0) anEvent = myRun0.GetNextEvent();
      //      if(i==1)anEvent = myRun1.GetNextEvent();
      
      if( nevt%100 == 0 )cout << " Event: " << nevt << endl;

      MPulse p1 = anEvent->GetChannel(i)->GetFirstPulse();

      if( nevt == 0 ) {
	/// first event: create histo
	hT = new TH1F("hT","hT", 100,-5000,0);
	hQ = new TH1F("hQ","hQ", 50,-1100,0);
	hV = new TH1F("hV","hV", 50,-1100,0);
      }
      
      hT->Fill(p1.tMax);
      hQ->Fill(p1.qTot);
      hV->Fill(p1.vMax);
      cout << nevt << endl;
      nevt++;
      anEvent=myRun0.GetNextEvent();
    }


    nevt = 0;

    if(i==0){
      TCanvas *can0 = new TCanvas("canvas_Channel_0","canvas_Channel_0",1000,1000);
      can0->Divide(2,2);
    
      can0->cd(1);  hT->DrawCopy();
      can0->cd(2);  hQ->DrawCopy();
      can0->cd(3);  hV->DrawCopy();
      can0->Update();
    }


    if(i==1){
      TCanvas *can1 = new TCanvas("canvas_Channel_1","canvas_Channel_1",1000,1000);
      can1->Divide(2,2);
      
      can1->cd(1);  hT->DrawCopy();
      can1->cd(2);  hQ->DrawCopy();
      can1->cd(3);  hV->DrawCopy();
      can1->Update();
    }
    
  }

  return 1;
}

int CaracterizeSecondPulse(  string fileData ) {
  //  gROOT->SetStyle("plain");
  gSystem->Load("DecodeMatacq_TL_cc.so");
 
  MRun myRun0(fileData.c_str());
  MRun myRun1(fileData.c_str());


  MEvent *anEvent = 0;
  unsigned nevt = 0;

  TH1F *hT = 0; /// time   second pulse
  TH1F *hQ = 0; /// charge second pulse
  TH1F *hV = 0; /// amplitude second pulse 

  for(int i = 0 ; i<1; i++){
    anEvent=myRun0.GetNextEvent();
    if ( anEvent==0 ) {
      cerr << "No event in Dataset" << endl;
      return -1;
    }

  while(! anEvent==0 ) {
    MPulse p2 = anEvent->GetChannel(i)->GetSecondPulse();
    
    if( nevt == 0 ) {
      /// first event: create histo
      hT = new TH1F("hT","hT", 100,-5000,0);
      hQ = new TH1F("hQ","hQ", 50,-1100,0);
      hV = new TH1F("hV","hV", 50,-1100,0);
    }
    if(p2.vMax != 0) {
    hT->Fill(p2.tMax);
    hQ->Fill(p2.qTot);
    hV->Fill(p2.vMax);
    }
    nevt++;
    anEvent=myRun0.GetNextEvent();
  }
  nevt=0;
  if(i==0){
  TCanvas *can2 = new TCanvas("canvas_Channel0","canvas_Channel0",1000,1000);
  can2->Divide(2,2);

  can2->cd(1);  hT->DrawCopy();
  can2->cd(2);  hQ->DrawCopy();
  can2->cd(3);  hV->DrawCopy();
  can2->Update();
  }

  if(i==1){
  TCanvas *can3 = new TCanvas("canvas_Channel1","canvas_Channel1",1000,1000);
  can3->Divide(2,2);

  can3->cd(1);  hT->DrawCopy();
  can3->cd(2);  hQ->DrawCopy();
  can3->cd(3);  hV->DrawCopy();
  can3->Update();
  }

 }
  return 1;
}



int FabMuonLifeTime(string fileData) {
  gSystem->Load("DecodeMatacq_TL_cc.so");
 
  //  gROOT->SetStyle("plain");
  MEvent *anEvent = 0;
  unsigned nevt = 0;
  
  MRun myRun (fileData.c_str());

  Int_t event, npulse1, npulse2, npulse3;
  vector<MPulse> p1,p2,p3;
  MPulse a1,a2,a3;
  MPulse b1,b2,b3;
  MPulse c1,c2,c3;
  TFile *fOut  = new TFile(string(fileData + ".root" ).c_str(), "recreate");
  TTree *ttree = new TTree("AnaMuon","MuonAna");
  ttree->Branch("evt",&event,"evt/I");
  ttree->Branch("npulse1",&npulse1,"npulse1/I");
  ttree->Branch("npulse2",&npulse2,"npulse2/I");
  ttree->Branch("npulse3",&npulse3,"npulse3/I");
  ttree->Branch("a1",&a1,"tUp/F:tMax/F:vMax/F:qTot/F");
  ttree->Branch("a2",&a2,"tUp/F:tMax/F:vMax/F:qTot/F");
  ttree->Branch("a3",&a3,"tUp/F:tMax/F:vMax/F:qTot/F");
  ttree->Branch("b1",&b1,"tUp/F:tMax/F:vMax/F:qTot/F");
  ttree->Branch("b2",&b2,"tUp/F:tMax/F:vMax/F:qTot/F");
  ttree->Branch("b3",&b3,"tUp/F:tMax/F:vMax/F:qTot/F");
  ttree->Branch("c1",&c1,"tUp/F:tMax/F:vMax/F:qTot/F");
  ttree->Branch("c2",&c2,"tUp/F:tMax/F:vMax/F:qTot/F");
  ttree->Branch("c3",&c3,"tUp/F:tMax/F:vMax/F:qTot/F");


  
  while( (anEvent = myRun.GetNextEvent())  ) {

    event = anEvent->EventNo();
    if( nevt == 0 ) {
      anEvent->GetChannel(0)->SetThreshold(-100);
      anEvent->GetChannel(1)->SetThreshold(-100);
      anEvent->GetChannel(2)->SetThreshold(-100);
    }

    if( nevt%100 == 0 ) cout << " Event: " << nevt << endl;
    p1 = anEvent->GetChannel(0)->GetPulses();
    p2 = anEvent->GetChannel(1)->GetPulses();
    p3 = anEvent->GetChannel(2)->GetPulses();
    


    if( p1.size() < 2 ) continue;
    double tUp0  = p1[0].tUp;
    double tMax0 = p1[0].tMax;
    npulse1 = p1.size();
    npulse2 = p2.size();
    npulse3 = p3.size();

    for( int ip = 0 ; ip < p1.size(); ip++ ) {
      p1[ip].tUp  -= tUp0;
      p1[ip].tMax -= tMax0;
      if( ip == 0 ) a1 = p1[ip];
      if( ip == 1 ) a2 = p1[ip];
      if( ip == 2 ) a3 = p1[ip];

    }

    for( int ip = 0 ; ip < p2.size(); ip++ ) {
      p2[ip].tUp  -= tUp0;
      p2[ip].tMax -= tMax0;
      if( ip == 0 ) b1 = p2[ip];
      if( ip == 1 ) b2 = p2[ip];
      if( ip == 2 ) b3 = p2[ip];
    }

    for( int ip = 0 ; ip < p3.size(); ip++ ) {
      p3[ip].tUp  -= tUp0;
      p3[ip].tMax -= tMax0;
      if( ip == 0 ) c1 = p3[ip];
      if( ip == 1 ) c2 = p3[ip];
      if( ip == 2 ) c3 = p3[ip];
    }

    ttree->Fill();
    nevt++;
  }
  //  fOut->WriteTObject(ttree);
  fOut->Write();
  return 1; 
}








