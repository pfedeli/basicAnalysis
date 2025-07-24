//////////////////////////////////////////////////////////
// This class has been automatically generated on
// Tue Jul 22 17:51:09 2025 by ROOT version 6.34.02
// from TTree t/
// found on file: ../../../tmpmount/TREE/run580113.ROOT
//////////////////////////////////////////////////////////

#ifndef Analysis_h
#define Analysis_h

#include <TROOT.h>
#include <TChain.h>
#include <TFile.h>
#include <sstream>
#include "Config.hh"
#include "Helper.hh"
#include "Histograms.hh"
#include <iostream>

class Analysis
{
public:
   TTree *fChain;  //! poInt_ter to the analyzed TTree or TChain
   Int_t fCurrent; //! current Tree number in a TChain

   // Fixed size dimensions of array or collections stored in the TTree if any.

   // Declaration of leaf types
   Double_t xpos[6];
   Long64_t nstrip[6];
   Double_t xinfo[5];
   Long64_t info_plus[2];
   Long64_t ievent;

   // List of branches
   TBranch *b_xpos;      //!
   TBranch *b_nstrip;    //!
   TBranch *b_xinfo;     //!
   TBranch *b_info_plus; //!
   TBranch *b_ievent;    //!

   Analysis(const std::string &configFile, TTree *tree);
   virtual ~Analysis();
   virtual Int_t Cut(Long64_t entry);
   virtual Int_t GetEntry(Long64_t entry);
   virtual Long64_t LoadTree(Long64_t entry);
   virtual void Init(TTree *tree);
   virtual void Loop();
   virtual bool Notify();
   virtual void Show(Long64_t entry = -1);
   virtual void RecoEvent(Long64_t entry);

private:
   Config cfg;
   Helper help;

   Double_t SD1SD2_Z_ = 330.0;
   Double_t SD1CRY_Z_ = 383.2;
   Double_t CRYSD3_Z_ = 10.7;
   Double_t SD2SD3_Z_ = 66.8;

   Double_t SD1xcorr_ = 0;
   Double_t SD1ycorr_ = 0;
   Double_t SD2xcorr_ = 0;
   Double_t SD2ycorr_ = 0;
   Double_t SD3xcorr_ = 0;
   Double_t SD3ycorr_ = 0;

   Int_t cut_flag_ = 0;

   Double_t cut_x1_ = -10;
   Double_t cut_x2_ = 10;
   Double_t cut_y1_ = -10;
   Double_t cut_y2_ = 10;

   Double_t cherry_ene_cut_ = 0;
   Double_t LG_ene_cut_ = 0;
   Double_t time_cut_ = 1000;
   Double_t theta_crit_ = 1000000;

   Int_t clucut_active_ = 0;

   // calibration
   Double_t m_ = 1;
   Double_t q_ = 0;

   Double_t thetax_in_;
   Double_t thetay_in_;
   Double_t thetax_out_;
   Double_t thetay_out_;
   Double_t xcry_;
   Double_t ycry_;

};

#endif

#ifdef Analysis_cxx

Analysis::Analysis(const std::string &configFile, TTree *tree = nullptr) : fChain(0)
{
   if (!cfg.Load(configFile))
   {
      std::cerr << "Errore nel caricamento del file di configurazione nel costruttore: " << configFile << std::endl;
      return;
   }

   std::string runDir = cfg.GetString("run_dir");
   std::vector<std::string> runList = cfg.GetStringList("run_list");
   for(long unsigned int a =0; a< runList.size(); a++){
      std::cout << "loading runs: " << runList[a] << std::endl;
   }
   cut_flag_ = cfg.GetInt("cutflag");
   if(!cut_flag_)
      std::cout << "NO CUT SELECTED " << std::endl;
   cut_x1_ = cfg.GetDouble("cut_x1");
   cut_x2_ = cfg.GetDouble("cut_x2");
   cut_y1_ = cfg.GetDouble("cut_y1");
   cut_y2_ = cfg.GetDouble("cut_y2");
   cherry_ene_cut_ = cfg.GetDouble("ene_cut");
   LG_ene_cut_ = cfg.GetDouble("cut_eneLG");
   time_cut_ = cfg.GetDouble("timecut");
   theta_crit_ = cfg.GetDouble("theta_crit");
   clucut_active_ = cfg.GetInt("clucut_active");
   SD1SD2_Z_ = cfg.GetDouble("SD1SD2_Z");
   SD1CRY_Z_ = cfg.GetDouble("SD1CRY_Z");
   CRYSD3_Z_ = cfg.GetDouble("CRYSD3_Z");
   SD2SD3_Z_ = cfg.GetDouble("SD2SD3_Z");
   SD1xcorr_ = cfg.GetDouble("SD1xcorr");
   SD1ycorr_ = cfg.GetDouble("SD1ycorr");
   SD2xcorr_ = cfg.GetDouble("SD2xcorr");
   SD2ycorr_ = cfg.GetDouble("SD2ycorr");
   SD3xcorr_ = cfg.GetDouble("SD3xcorr");
   SD3ycorr_ = cfg.GetDouble("SD3ycorr");

   if (tree == 0)
   {

      TChain *chain = new TChain("t"); // "t" è il nome dell'albero

      for (const auto &run : runList)
      {
         std::string filePath = runDir + "run" + run + ".ROOT";
         chain->Add(filePath.c_str());
      }

      tree = chain;
   }

   Init(tree);
}

Analysis::~Analysis()
{
   if (fChain)
      delete fChain;
}

Int_t Analysis::GetEntry(Long64_t entry)
{
   // Read contents of entry.
   if (!fChain)
      return 0;
   return fChain->GetEntry(entry);
}
Long64_t Analysis::LoadTree(Long64_t entry)
{
   // Set the environment to read one entry
   if (!fChain)
      return -5;
   Long64_t centry = fChain->LoadTree(entry);
   if (centry < 0)
      return centry;
   if (fChain->GetTreeNumber() != fCurrent)
   {
      fCurrent = fChain->GetTreeNumber();
      Notify();
   }
   return centry;
}

void Analysis::Init(TTree *tree)
{

   if (!tree)
      return;
   fChain = tree;
   fCurrent = -1;
   fChain->SetMakeClass(1);

   fChain->SetBranchAddress("xpos", xpos, &b_xpos);
   fChain->SetBranchAddress("nstrip", nstrip, &b_nstrip);
   fChain->SetBranchAddress("xinfo", xinfo, &b_xinfo);
   fChain->SetBranchAddress("info_plus", info_plus, &b_info_plus);
   fChain->SetBranchAddress("ievent", &ievent, &b_ievent);
   Notify();
}

bool Analysis::Notify()
{
   // The Notify() function is called when a new file is opened. This
   // can be either for a new TTree in a TChain or when when a new TTree
   // is started when using PROOF. It is normally not necessary to make changes
   // to the generated code, but the routine can be extended by the
   // user if needed. The return value is currently not used.

   return true;
}

void Analysis::Show(Long64_t entry)
{
   // PrInt_t contents of entry.
   // If entry is not specified, prInt_t current entry
   if (!fChain)
      return;
   fChain->Show(entry);
}
Int_t Analysis::Cut(Long64_t entry)
{
   double theta_in = help.GetQuadratureSum(thetax_in_, thetay_in_);

   bool xposcuts = (xcry_ > cut_x1_ && xcry_ < cut_x2_);
   bool yposcuts = (ycry_ > cut_y1_ && ycry_ < cut_y2_);

   bool theta_cut = theta_in < theta_crit_;

   if(!cut_flag_){
      return 1;
   }

   
   if (theta_cut && xposcuts && yposcuts)
   {
      return 1;
   }

   return -1;
}

void Analysis::RecoEvent(Long64_t entry){
   thetax_in_ = help.GetTheta(xpos[0], xpos[2], SD1SD2_Z_);
   thetay_in_ = help.GetTheta(xpos[1], xpos[3], SD1SD2_Z_);

   xcry_ = xpos[0] + help.GetCryPos(thetax_in_, SD1CRY_Z_);
   ycry_ = xpos[1] + help.GetCryPos(thetay_in_, SD1CRY_Z_);

   thetax_out_ = help.GetTheta(xcry_, xpos[4], CRYSD3_Z_);
   thetay_out_ = help.GetTheta(ycry_, xpos[5], CRYSD3_Z_);

   thetax_in_*=1000.0; //rad -> mrad
   thetay_in_*=1000.0; //rad -> mrad
   thetax_out_*=1000.0; //rad -> mrad
   thetay_out_*=1000.0; //rad -> mrad
}
#endif // #ifdef Analysis_cxx
