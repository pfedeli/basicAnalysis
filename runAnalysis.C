void runAnalysis() {
    gSystem->Load("lib/libAnalysis.so");
    gROOT->ProcessLine(".L Analysis.C++");  // Carica la classe

    // Crea oggetto e chiama Loop in una singola stringa C++
    gROOT->ProcessLine("Analysis t(\"config/alignROOT.conf\"); t.Loop();");
}
