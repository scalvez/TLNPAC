#ifndef Decode_Matacq_hh_
#define Decode_Matacq_hh_

/// those matacq cards have 4 channels
#define NMAXCH 4


#include <vector>
#include <string>
#include <fstream>
#include <memory>


#include "TH1F.h"
class MEvent;
class MRun;
typedef unsigned uint;
typedef std::auto_ptr<TH1F>   pTH1F;


///@ Structure MPulse:  storage of pulse attributes

///@ Class MRun reads a file containing a collection of events decode
///  Prefer only 1 run per file, but this will be checked
///  if more than one run per file, specify the run to be analyze
/// otherwise the first run found will be analyzed.

///@ Class MEvent decode


struct MPulse {
  Float_t tUp;    /// time when passing threshold
  Float_t tMax;   /// time at maximum of the pulse
  Float_t vMax;   /// maximum amplitude
  Float_t qTot;   /// collected charge
};


//////////////////////////////////////////////////////////////////////////////
/////////////////////// Define a simple Channel class ////////////////////////
//////////////////////////////////////////////////////////////////////////////
class MChannel {
public:
  MChannel(void);  
  ~MChannel(void);
  
  void CreateChannel(uint nSampling, double tMin, double deltaT, uint color=1);
  bool UpdateChannel(double t, double V);
  void ResetChannel(void);
  
  ///@ Get a clone of the channel histogram (offset is not corrected for yet)
  pTH1F GetChannelHisto(const char *hName=0);
  
  ///@ Return the pedestal value
  double GetPedestal(void);
  
  ///@return secondary pulses
  std::vector<MPulse> GetPulses(void);
  
  void SetThreshold( double seuil = -40 ) {
    _seuil = seuil;
  }

  MPulse GetFirstPulse(void);
  MPulse GetSecondPulse(void);

private:
  uint _nSampling; /// number of samples
  TH1F *_hData;    /// data    
  double _seuil;
  double _pedestal;     /// pedestal  
  friend class MEvent;
};


//////////////////////////////////////////////////////////////////////////////
///////////////////////// Define a simple Event class ////////////////////////
//////////////////////////////////////////////////////////////////////////////
class MEvent {
      
   public:
      MEvent(void);
      MEvent(int runNumber,uint evtNumber, uint nChannels, uint nSampling);
      ~MEvent(void);
      
      ///// "Setting" functions /////////
      ///@ Create a channel
      bool CreateChannel(uint index_ich,uint nSamples, double t0, double deltaT, uint color=1);
      ///@ Reset Channel ich
      void ResetEvent(void);
      ///@ add a new data point to channel ich
      bool AddSampleToChannel(uint ich, double x, double y);


      ///// "Getting" functions /////////
      ///@ draw event output (y vs x) for channel ich
      bool DrawChannel(uint ich, std::string option="",double xmin = 0, double xmax = -1);
      ///@ get back channel
      MChannel* GetChannel(int ich);
      
      ///@ get number of channels
      uint GetNumOfChannels(void) const {
         return _nChannels;}
  
      ///@ get event number
      uint EventNo( void ) const {
	return _evtNumber;
      }
  
   private:
      int   _runNumber;    ///! run number to be analyzed
      uint  _evtNumber;    ///! current event number
      uint  _nChannels;    ///! number of channels
      uint  _nSampling;    ///! number of points per channel
      

      std::vector<MChannel> _channels;
};


class MRun {
   public:
      MRun(void);
      MRun(const char *fileIn, int runNumber = -999);
      ~MRun(void);
      
      ///@ get next event in file 
      MEvent* GetNextEvent(void);
      
   private:
      int     _runNumber;
      MEvent *_currentEvt;

      std::string   _fileName;  ///! input file name
      std::ifstream _datafile;  ///! input file
};


std::string GetAName(std::string base= "name_tmp_");

#endif

