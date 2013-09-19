/// my headers
#include "DecodeMatacq_TL.hh"

/// C++ headers
#include <string>
#include <iostream>
#include <sstream>
#include <fstream>
#include <cstdlib> 
#include <iomanip>
#include<cmath>
using namespace std;

/// ROOT headers
#include <TGraph.h>
#include <TCanvas.h>
#include <TH1.h>
#include <TFile.h>
#include <vector>


//ClassImp(MPulse);

//////////////////////////////////////////////////////////////////////////////
/////////////////////// Define a simple Channel class ////////////////////////
//////////////////////////////////////////////////////////////////////////////
MChannel::MChannel(void) {
  _seuil     = -40;
  _nSampling = 0;
  _hData     = 0;
  _pedestal     = +9999; /// default: pedestal is not computed yet
}

MChannel::~MChannel(void) { 
  if( _hData == 0 ) delete _hData;
}


void MChannel::CreateChannel( uint nSampling, double tMin, double deltaT, uint color) {
  if( _hData != 0 ) delete _hData;
  _nSampling = nSampling;
  double ti = tMin - deltaT/2.;
  double tf = ti + deltaT*nSampling;
  string hName = GetAName("hChannel_");
  _hData = new TH1F( hName.c_str(), hName.c_str(), nSampling, ti, tf );
  _hData->GetXaxis()->SetTitle("time [ns]");
  _hData->GetYaxis()->SetTitle("voltage [mV]");
  _hData->SetMinimum(-1000.);  /// -1000mV is the lowest voltage accepted by Matacq cards
  _hData->SetMaximum(   10.); 
  _hData->SetLineColor(  color);
  _hData->SetMarkerColor(color);
}

bool MChannel::UpdateChannel(double t, double V) {
  if( _hData == 0 ) {
    //   cerr << "ERROR [MChannel::UpdateChannel(...)]: "
    //     << "Channel was not created properly. Use CreateChannel function before"
    //     <<endl;
    return false;
  }
  uint ibin = _hData->FindBin(t);
  _hData->SetBinContent(ibin,V);
  return true;
}


pTH1F MChannel::GetChannelHisto(const char *hName) {
  if( _hData == 0 ) return pTH1F( (TH1F*) 0);
   
  string myName = string(_hData->GetName()) + string("_CLONE");
  if( hName != 0 ) myName = string(hName);
  return pTH1F( (TH1F*) _hData->Clone( myName.c_str() ) );
}

void MChannel::ResetChannel(void) {
  _pedestal = 9999;
   
  if( _hData == 0 ) { return;}
  _hData->Reset(); 
   
}
   

double MChannel::GetPedestal(void) {
  if( _pedestal > 80 )  {
    /// need to compute the pedestal
    _pedestal = _hData->Integral(2,150)/149;
  }
   
  return _pedestal;
}



MPulse MChannel::GetFirstPulse(void){
  //// ceci est un exemple et pas l'ultime solution

  double   AmplMax = 0;
  int      ibin_max = 1;
  int      ibup     = -1;
  int      ibdown   = -1;
  bool passThreshold = false;

  for( int ibin = 1; ibin <= _hData->GetNbinsX(); ibin++ )  {         
    
    ///  montee du signal (premier passage du seuil)
    if( !passThreshold && _hData->GetBinContent(ibin) < _seuil )  {
      ibup          = ibin;
      passThreshold = true;
    } 

    if( _hData->GetBinContent(ibin) < AmplMax && passThreshold)  {
      AmplMax = _hData->GetBinContent(ibin);
      ibin_max = ibin;
    };      

    /// descente du signal (deuxieme passage du seuil)
    if(  passThreshold && _hData->GetBinContent(ibin) > _seuil )  {
      ibdown        = ibin;
      passThreshold = false;
      break;
      /// end after first pulse
    }
  }

  
  MPulse pulse1;

  pulse1.vMax = AmplMax;
  pulse1.tMax = _hData->GetXaxis()->GetBinCenter(ibin_max);
  pulse1.tUp  = _hData->GetXaxis()->GetBinCenter(ibup); 
  pulse1.qTot = _hData->Integral(ibup,ibdown);// - (ibup-ibdown+1)*GetPedestal();
      
  return pulse1;
}

MPulse MChannel::GetSecondPulse(void){

  double   AmplMax = 0;
  int      ibin_max = 1;
  int      ibup     = -1;
  int      ibdown   = -1;
  bool passThreshold = false;

  for( int ibin = 1; ibin <= _hData->GetNbinsX(); ibin++ )  {         
    
    ///  montee du signal (premier passage du seuil)
    if( !passThreshold && _hData->GetBinContent(ibin) < _seuil )  {
      passThreshold = true;
    }     

    /// descente du signal (deuxieme passage du seuil)
    if(  passThreshold && _hData->GetBinContent(ibin) > _seuil )  {
      ibdown        = ibin;
      passThreshold = false;
      break;
      /// end after first pulse
    }
  }
for( int ibin = ibdown; ibin <= _hData->GetNbinsX(); ibin++ )  {         
    
    ///  montee du signal (premier passage du seuil)
    if( !passThreshold && _hData->GetBinContent(ibin) < _seuil )  {
      ibup          = ibin;
      passThreshold = true;
    } 

    if( _hData->GetBinContent(ibin) < AmplMax && passThreshold)  {
      AmplMax = _hData->GetBinContent(ibin);
      ibin_max = ibin;
    };      

    /// descente du signal (deuxieme passage du seuil)
    if(  passThreshold && _hData->GetBinContent(ibin) > _seuil )  {
      ibdown        = ibin;
      passThreshold = false;
      break;
      /// end after second pulse
    }



  }

  
  MPulse pulse2;

  pulse2.vMax = AmplMax;
  pulse2.tMax = _hData->GetXaxis()->GetBinCenter(ibin_max);
  pulse2.tUp  = _hData->GetXaxis()->GetBinCenter(ibup); 
  pulse2.qTot = _hData->Integral(ibup,ibdown);// - (ibup-ibdown+1)*GetPedestal();
      
  return pulse2;
}



 vector<MPulse> MChannel::GetPulses(void) {
   vector<MPulse> pulses;

  //// ceci est un exemple et pas l'ultime solution

  double   AmplMax = 0;
  int      ibin_max = 1;
  int      ibup     = -1;
  int      ibdown   = -1;
  bool passThreshold = false;

  for( int ibin = 1; ibin <= _hData->GetNbinsX(); ibin++ )  {             
    ///  montee du signal (premier passage du seuil)
    if( !passThreshold && _hData->GetBinContent(ibin) < _seuil )  {
      ibup          = ibin;
      passThreshold = true;
    } 

    if( _hData->GetBinContent(ibin) < AmplMax && passThreshold )  {
      AmplMax = _hData->GetBinContent(ibin);
      ibin_max = ibin;
    };      

    /// descente du signal (deuxieme passage du seuil)
    if(  passThreshold && _hData->GetBinContent(ibin) > _seuil )  {
      ibdown        = ibin;
      passThreshold = false;


      MPulse aPulse;
      aPulse.vMax = AmplMax;
      aPulse.tMax = _hData->GetXaxis()->GetBinCenter(ibin_max);
      aPulse.tUp  = _hData->GetXaxis()->GetBinCenter(ibup); 
      aPulse.qTot = _hData->Integral(ibup,ibdown) - (ibup-ibdown+1)*GetPedestal();

      pulses.push_back( aPulse );
      /// reset everything
      ibup = ibdown = -1;
      AmplMax = 0;
      passThreshold = false;
      /// end after first pulse
    }
  }

   return pulses;
 }


//////////////////////////////////////////////////////////////////////////////
///////////////////////// Define a simple Event class ////////////////////////
//////////////////////////////////////////////////////////////////////////////
MEvent::MEvent(void) {
  _runNumber   = -999;
  _evtNumber   = 0;
  _nChannels   = 0;
  _nSampling   = 0;

}

MEvent::MEvent(int runNumber,uint evtNumber, uint nChannels, uint nSampling) {
  _runNumber   = runNumber;
  _evtNumber   = evtNumber;
  _nChannels   = nChannels;
  _nSampling   = nSampling;

  _channels.resize(nChannels);
}

MEvent::~MEvent(void) {
}

bool MEvent::CreateChannel(uint ich,uint nSamples, double t0, double deltaT,uint color) {
  if(  ich > _nChannels ) {
    cerr << "ERROR[MEvent::AddSampleToChannel(...)]: "
	 <<" ich(" << ich << ") out of range" << endl;
    return false;
  }
   
  _channels[ich].CreateChannel(nSamples,t0,deltaT,color);
  return true;
} 


bool MEvent::AddSampleToChannel(uint ich, double x, double y){
  if(  ich > _nChannels ) {
    cerr << "ERROR[MEvent::AddSampleToChannel(...)]: "
	 <<" ich(" << ich << ") out of range" << endl;
    return false;
  }

  return _channels[ich].UpdateChannel(x,y);
}


bool MEvent::DrawChannel( uint ich, string option, double xmin, double xmax ) {
  if(  ich > _nChannels ) {
    cerr << "ERROR[MEvent::AddSampleToChannel(...)]: "
	 <<" ich(" << ich << ") out of range" << endl;
    return false;
  }

  if( xmax < xmin ) {
    _channels[ich]._hData->Print();
    _channels[ich]._hData->DrawCopy(option.c_str());
    return true;
  } 
  _channels[ich]._hData->SetStats(false);
  _channels[ich]._hData->GetXaxis()->SetRangeUser(xmin,xmax);
  _channels[ich]._hData->DrawCopy(option.c_str());
   
  return true;
}


void MEvent::ResetEvent(void) {
  for( uint ic = 0 ; ic < _nChannels; ic++ )
    _channels[ic].ResetChannel();

}


MChannel* MEvent::GetChannel(int ich) {
  return &_channels[ich];
}

//////////////////////////////////////////////////////////////////////////////
////////////////////////// Define a simple run class /////////////////////////
//////////////////////////////////////////////////////////////////////////////
//--------------------- basic contructors, destructors ---------------------//
MRun::MRun(void) {
  _fileName="UNKNOWN";
  _runNumber = -999;
  _currentEvt = 0;
}

MRun::MRun(const char *fileIn, int runNumber) {
  _runNumber = runNumber;
  _fileName = fileIn;
  cout << "MRun contructed from file: " << _fileName << endl;


  _datafile.open(_fileName.c_str(),ios::in);
  
  if( !_datafile ) {
    cerr << "ERROR: Datafile " << _fileName << "does not exist"<< endl;
  }
  
  if( !_datafile.is_open() ) {
    cerr << "ERROR: Can not open datafile " << _fileName  <<  endl;
  }
  _currentEvt = 0;
}

MRun::~MRun(void) { 
  if( _datafile && _datafile.is_open() ) _datafile.close();
  if( _currentEvt != 0 ) delete _currentEvt;
}
//------------------------------ Functions ----------------------------------//
string format(string sIn) {
  string sOut = sIn;
  for( unsigned c = 0 ; c < sOut.length(); c++) 
    if( sOut[c] == ';' ) sOut[c] = ' ';  
  return sOut;
}

MEvent* MRun::GetNextEvent(void) {
  string line;
  string comments = "#";
  
  string aString;
  double aDouble;
  double tc[NMAXCH], Vc[NMAXCH];
  double t0[NMAXCH], V0[NMAXCH];
  uint iline = 0;
  int myRunNumber = -999;
  uint  myEvtNumber, myNofChannels, myNofPoints;
  double myDay, myTime;
  
  myEvtNumber = myNofChannels = myNofPoints = 0;

  bool stop = false;
  bool readSomething = false;

  uint ch_colors[4];
  ch_colors[0] = 1;
  ch_colors[1] = 2;
  ch_colors[2] = 8;
  ch_colors[3] = 9;

  while( !stop )
    if( getline( _datafile,line) ) {
      if( !(line.find(comments.c_str()) < line.size()) ) {
        /// this is not a comment
        istringstream iss( line );
        if(iline == 0 ) {iss >> aDouble; myRunNumber   =  uint(aDouble);}
        if(iline == 1 ) {iss >> aDouble; myEvtNumber   =  uint(aDouble);}
        if(iline == 2 ) {iss >> aDouble; myNofChannels =  uint(aDouble);}
        if(iline == 3 ) {iss >> aDouble; myNofPoints   =  uint(aDouble);}
        if(iline == 4 ) {iss >> aDouble; myDay  =  aDouble;}
        if(iline == 5 ) {iss >> aDouble; myTime =  aDouble;}

        if( iline >= 5+myNofPoints ) stop = true;
   
        if( iline == 6 ) {
	  if( _currentEvt ) _currentEvt->ResetEvent();
	  /// first sample, do not know yet deltaT
	  istringstream iss2( format(line) );
	  for( uint ic = 0 ; ic < myNofChannels; ic++ ) {              
	    iss2 >> t0[ic] >> V0[ic];
	    if( _runNumber != -999 ) 
	      _currentEvt->AddSampleToChannel(ic, t0[ic], V0[ic]);
	  }
        }
        if( iline > 6 ) {           
	  readSomething = true;
	  istringstream iss2( format(line) );
	  bool init = false;
	  if(  iline == 7 && _runNumber == -999 ) {
	    cout << " -- Find new run" << endl;
	    _runNumber  = myRunNumber;
	    _currentEvt = new MEvent( myRunNumber, myEvtNumber, 
				      myNofChannels, myNofPoints);
	    init = true;
	  }


	  for( uint ic = 0 ; ic < myNofChannels; ic++ ) {
	    iss2 >> tc[ic] >> Vc[ic] ;              
	    if( init ) {		
	      _currentEvt->CreateChannel(ic, myNofPoints,t0[ic],tc[ic]-t0[ic],ch_colors[ic]);
	      _currentEvt->AddSampleToChannel(ic, t0[ic], V0[ic]);
	    }
	    _currentEvt->AddSampleToChannel(ic, tc[ic], Vc[ic]);
	  }
        }
        iline++;
      }
    } else {
      stop = true;
    }
     
        
  if( readSomething ) { 
    //    cout << "runnumber | evtNumber | nCh | nSampling: " 
    //         << myRunNumber << " | " << myEvtNumber << " | "
    //         << myNofChannels << " | " << myNofPoints << endl;
     
  }
  
  else return (MEvent*) 0;
  return _currentEvt; 
}




//////////////////////////////////////////////
//////// function GetAName to get random name
//////////////////////////////////////////////
static uint itmp_name = 0;
string GetAName(string base) {
  std::ostringstream oss; oss << itmp_name;
  string num  = oss.str();
  itmp_name++;
  return base+num;

}
