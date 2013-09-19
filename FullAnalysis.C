#include <iostream>
#include <fstream>

#include "FunctionAnalyse.h"
#include "DecodeMatacq_TL.hh"

using namespace std;

int main() {

  //  FabMuonLifeTime("raw/ex_raw_data.ecor");
  CaracterizeFirstPulse("raw/test_data2.ecor");
  CaracterizeSecondPulse("raw/test_data2.ecor");

  cout << "Went up to the end" << endl;
  return 0;
}
