#include <iostream>
#include <fstream>

#include "FunctionAnalyse.h"
#include "DecodeMatacq_TL.hh"

using namespace std;

int main() {

  FabMuonLifeTime("raw/test_data2.ecor");

  cout << "Went up to the end" << endl;
  return 0;
}
