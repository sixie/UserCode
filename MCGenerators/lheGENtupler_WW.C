#if !defined(__CINT__) || defined(__MAKECINT__)
#include <TROOT.h>
#include <TFile.h>
#include <TTree.h>
#include <TCanvas.h>
#include <TBenchmark.h>
#include <vector>
#include <iostream>
#include <fstream>
#include <sstream>
#include <TLorentzVector.h>
#endif

#include "GenStructDefs.hh"

TGenWWData data;

//====================================================================================================

void lheGENtupler_WW(Int_t Option = 0) {

  gBenchmark->Start("lheGENtupler");
  
//   const char* outfname = "/data/blue/sixie/Madgraph/WW/ww_scaleUp_gentuple.root";
  vector<string> fnamev;
  string outfname; 
  if (Option == 0) {
    outfname = "JHU_WWlvlv_Spin0_PChannel2_PDFSet1.root";
    fnamev.push_back("JHUGenerator/JHU_WWlvlv_Spin0_PChannel2_PDFSet1.lhe");
  } 
  if (Option == 1) {
    outfname = "JHU_WWlvlv_Spin2_PChannel2_PDFSet1.root";
    fnamev.push_back("JHUGenerator/JHU_WWlvlv_Spin2_PChannel2_PDFSet1.lhe");
  } 


  TFile* outfile = new TFile(outfname.c_str(),"RECREATE");

  TTree::SetMaxTreeSize(kMaxLong64);
  
  TTree* tree = new TTree("Events","Events");  
  tree->Branch("Events",&data.scalePdf,"scalePdf/F:weight:higgsmass:higgspt:higgsy:higgsphi:w1mass:w1pt:w1y:w1phi:w2mass:w2pt:w2y:w2phi:nu1mass:nu1pt:nu1y:nu1phi:nu2mass:nu2pt:nu2y:nu2phi:lep1mass:lep1pt:lep1y:lep1phi:lep2mass:lep2pt:lep2y:lep2phi:x_1:x_2:id_1/I:id_2:acqd/F:aqed"); 
    
  //--------------------------------------------------------------------------------------------------
  
  for(UInt_t ifile=0; ifile<fnamev.size(); ifile++) {
  
    const char* lhefile = fnamev[ifile].c_str();
    ifstream ifs;
    ifs.open(lhefile);
    assert(ifs.is_open()); 
  
    string line; 
        
    cout << "Processing file: " << fnamev[ifile] << endl;
    
    do {
      getline(ifs,line);
    } while(line.compare("<init>")!=0);
    
    getline(ifs,line);
    stringstream ss1(line);
    int idbmup1, idbmup2; 
    double ebmup1, ebmup2; 
    int pdfgup1, pdfgup2, pdfsup1, pdfsup2, idwtup, nprup;
    ss1 >> idbmup1 >> idbmup2 >> ebmup1 >> ebmup2 >> pdfgup1 >> pdfgup2 >> pdfsup1 >> pdfsup2 >> idwtup >> nprup;
    
    getline(ifs,line);
    stringstream ss2(line);
    double xsecup, xerrup, xmaxup;
    int lprup;
    ss2 >> xsecup >> xerrup >> xmaxup >> lprup;
    
    getline(ifs,line);
    cout << line << endl;
    assert(line.compare("</init>")==0);
    
    while(getline(ifs,line)) {

      if(line.compare("<event>")==0) {
        getline(ifs,line);
	stringstream ss3(line);
	int nup, idprup;
	double xwgtup, scalup, aqedup, aqcdup;
	ss3 >> nup >> idprup >> xwgtup >> scalup >> aqedup >> aqcdup;
	data.scalePdf = scalup;
	data.weight   = xwgtup;
	data.aqcd     = aqcdup;
	data.aqed     = aqedup;
	
        if (!(nup == 9)) cout << "Error: number of particles is not right \n" 
                                       << nup << " " << idprup << " " << xwgtup 
                                       << " " << scalup << " " << aqedup << " " << aqcdup << endl;
		
	int idup, istup, mothup1, mothup2, icolup1, icolup2; 
	double  pup1, pup2, pup3, pup4, pup5, vtimup, spinup;
	
	// quark 1 info
        getline(ifs,line);
        stringstream ssq1(line);
	ssq1 >> idup >> istup >> mothup1 >> mothup2 >> icolup1 >> icolup2 >> pup1 >> pup2 >> pup3 >> pup4 >> pup5 >> vtimup >> spinup;
	data.x_1  = fabs(pup3/ebmup1);
	data.id_1 = idup;
	
	// quark 2 info
        getline(ifs,line);
        stringstream ssq2(line);
	ssq2 >> idup >> istup >> mothup1 >> mothup2 >> icolup1 >> icolup2 >> pup1 >> pup2 >> pup3 >> pup4 >> pup5 >> vtimup >> spinup;
	data.x_2  = fabs(pup3/ebmup2);
	data.id_2 = idup;
	
 
        //higgs
        getline(ifs,line);
        stringstream sshiggs(line);
        sshiggs >> idup >> istup >> mothup1 >> mothup2 >> icolup1 >> icolup2 >> pup1 >> pup2 >> pup3 >> pup4 >> pup5 >> vtimup >> spinup;
        TLorentzVector higgs;
        higgs.SetPxPyPzE(pup1,pup2,pup3,pup4);
        data.higgsmass = higgs.M();
        data.higgspt   = higgs.Pt();
        data.higgsy    = higgs.Rapidity();
        data.higgsphi  = higgs.Phi();

	//get w1 info
        getline(ifs,line);
        stringstream ssw1(line);
        ssw1 >> idup >> istup >> mothup1 >> mothup2 >> icolup1 >> icolup2 >> pup1 >> pup2 >> pup3 >> pup4 >> pup5 >> vtimup >> spinup;
        TLorentzVector wboson1;
        wboson1.SetPxPyPzE(pup1,pup2,pup3,pup4);
        data.w1mass = wboson1.M();
        data.w1pt   = wboson1.Pt();
        data.w1y    = wboson1.Rapidity();
        data.w1phi  = wboson1.Phi();

	//get w2 info
        getline(ifs,line);
        stringstream ssw2(line);
        ssw2 >> idup >> istup >> mothup1 >> mothup2 >> icolup1 >> icolup2 >> pup1 >> pup2 >> pup3 >> pup4 >> pup5 >> vtimup >> spinup;
        TLorentzVector wboson2;
        wboson2.SetPxPyPzE(pup1,pup2,pup3,pup4);
        data.w2mass = wboson2.M();
        data.w2pt   = wboson2.Pt();
        data.w2y    = wboson2.Rapidity();
        data.w2phi  = wboson2.Phi();

	//get nu1 info
        getline(ifs,line);
        stringstream ssnu1(line);
        ssnu1 >> idup >> istup >> mothup1 >> mothup2 >> icolup1 >> icolup2 >> pup1 >> pup2 >> pup3 >> pup4 >> pup5 >> vtimup >> spinup;
        TLorentzVector nu1;
        nu1.SetPxPyPzE(pup1,pup2,pup3,pup4);
        data.nu1mass = nu1.M();
        data.nu1pt   = nu1.Pt();
        data.nu1y    = nu1.Rapidity();
        data.nu1phi  = nu1.Phi();

	//get lep1 info
        getline(ifs,line);
        stringstream sslep1(line);
        sslep1 >> idup >> istup >> mothup1 >> mothup2 >> icolup1 >> icolup2 >> pup1 >> pup2 >> pup3 >> pup4 >> pup5 >> vtimup >> spinup;
        TLorentzVector lep1;
        lep1.SetPxPyPzE(pup1,pup2,pup3,pup4);
        data.lep1mass = lep1.M();
        data.lep1pt   = lep1.Pt();
        data.lep1y    = lep1.Rapidity();
        data.lep1phi  = lep1.Phi();

	//get lep2 info
        getline(ifs,line);
        stringstream sslep2(line);
        sslep2 >> idup >> istup >> mothup1 >> mothup2 >> icolup1 >> icolup2 >> pup1 >> pup2 >> pup3 >> pup4 >> pup5 >> vtimup >> spinup;
        TLorentzVector lep2;
        lep2.SetPxPyPzE(pup1,pup2,pup3,pup4);
        data.lep2mass = lep2.M();
        data.lep2pt   = lep2.Pt();
        data.lep2y    = lep2.Rapidity();
        data.lep2phi  = lep2.Phi();

	//get nu2 info
        getline(ifs,line);
        stringstream ssnu2(line);
        ssnu2 >> idup >> istup >> mothup1 >> mothup2 >> icolup1 >> icolup2 >> pup1 >> pup2 >> pup3 >> pup4 >> pup5 >> vtimup >> spinup;
        TLorentzVector nu2;
        nu2.SetPxPyPzE(pup1,pup2,pup3,pup4);
        data.nu2mass = nu2.M();
        data.nu2pt   = nu2.Pt();
        data.nu2y    = nu2.Rapidity();
        data.nu2phi  = nu2.Phi();

	tree->Fill();
      }  
    }
    ifs.close();
  }
    
  tree->Print();
  outfile->Write();  
  outfile->Close();
    
  gBenchmark->Show("lheGENtupler");
}
