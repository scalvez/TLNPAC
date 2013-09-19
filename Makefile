all : FullAnalysis.exe

FullAnalysis.exe : FunctionAnalyse.C FullAnalysis.C
	g++ FunctionAnalyse.C FullAnalysis.C  `root-config --libs --cflags --ldflags`  -Wall -Wextra  -lRooFit -lRooFitCore -o FullAnalysis.exe