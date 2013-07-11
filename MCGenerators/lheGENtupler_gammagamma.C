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

TGenGammaGammaData data;

//====================================================================================================

void lheGENtupler_gammagamma(Int_t Option = 0) {

  gBenchmark->Start("lheGENtupler");
  
//   const char* outfname = "/data/blue/sixie/Madgraph/WW/ww_scaleUp_gentuple.root";
  vector<string> fnamev;
  string outfname; 
  if (Option == 0) {
    outfname = "JHU_gg_Spin0_PChannel2_PDFSet1.root";
    fnamev.push_back("JHUGenerator/JHU_gg_Spin0_PChannel2_PDFSet1.lhe");
  } 
  if (Option == 1) {
    outfname = "JHU_gg_Spin2_PChannel2_PDFSet1.root";
    fnamev.push_back("JHUGenerator/JHU_gg_Spin2_PChannel2_PDFSet1.lhe");
  } 


  TFile* outfile = new TFile(outfname.c_str(),"RECREATE");

  TTree::SetMaxTreeSize(kMaxLong64);
  
  TTree* tree = new TTree("Events","Events");  
  tree->Branch("Events",&data.scalePdf,"scalePdf/F:weight:higgsmass:higgspt:higgsy:higgstheta:higgsphi:ph1mass:ph1pt:ph1y:ph1theta:ph1phi:ph2mass:ph2pt:ph2y:ph2theta:ph2phi:x_1:x_2:id_1/I:id_2:acqd/F:aqed"); 

  //--------------------------------------------------------------------------------------------------
  Int_t NEvents = 0;

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
    
    while(getline(ifs,line) && NEvents<300000 ) {
      NEvents++;

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
	
        if (!(nup == 5)) cout << "Error: number of particles is not right \n" 
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
        data.higgstheta= higgs.Theta();
        data.higgsphi  = higgs.Phi();

	//get ph1 info
        getline(ifs,line);
        stringstream ssph1(line);
        ssph1 >> idup >> istup >> mothup1 >> mothup2 >> icolup1 >> icolup2 >> pup1 >> pup2 >> pup3 >> pup4 >> pup5 >> vtimup >> spinup;
        TLorentzVector ph1;
        ph1.SetPxPyPzE(pup1,pup2,pup3,pup4);
        data.ph1mass = ph1.M();
        data.ph1pt   = ph1.Pt();
        data.ph1y    = ph1.Rapidity();
        data.ph1theta= ph1.Theta();
        data.ph1phi  = ph1.Phi();

	//get ph2 info
        getline(ifs,line);
        stringstream ssph2(line);
        ssph2 >> idup >> istup >> mothup1 >> mothup2 >> icolup1 >> icolup2 >> pup1 >> pup2 >> pup3 >> pup4 >> pup5 >> vtimup >> spinup;
        TLorentzVector ph2;
        ph2.SetPxPyPzE(pup1,pup2,pup3,pup4);
        data.ph2mass = ph2.M();
        data.ph2pt   = ph2.Pt();
        data.ph2y    = ph2.Rapidity();
        data.ph2theta= ph2.Theta();
        data.ph2phi  = ph2.Phi();

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
