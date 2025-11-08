#include <TCanvas.h>
#include <TH1D.h>
#include <TH2D.h>
#include <TGraphErrors.h>
#include <fstream>
#include <iostream>
#include <string>
#include <sstream>
#include <algorithm>
#include <TString.h>

using namespace std;

void loadLibs();

void readRHICfEvent(const Char_t* inputFile = "./st_rhicf_18178002_pp500_4000016_RHICfEventDst.root")
{
    loadLibs();

	StRHICfEventDst* rhicfEventDst;
	StRHICfEvent* rhicfEvent;
	StRHICfParticle* rhicfParticle;
	StRHICfDetPoint* rhicfDetPoint;
	StRHICfTPCTrack* rhicfTPCTrack;
	StRHICfBTof* rhicfBTof;
	StRHICfBBC* rhicfBBC;
	StRHICfVPD* rhicfVPD;
	StRHICfZDC* rhicfZDC;
	StRHICfFMS* rhicfFMS;
	StRHICfRPS* rhicfRPS;

    StRHICfEventMaker* eventMaker = new StRHICfEventMaker(0, inputFile);
	// eventMaker -> SetOffBBC();
	eventMaker -> Init();
	

    int eventNum = eventMaker -> GetEventNum();
    for(int event=0; event<eventNum; event++){
		if(event%1000==0){cout << "event: " << event << " / " << eventNum << endl;}
        rhicfEventDst = eventMaker -> GetEventDst(event);

		// General event data
		rhicfEvent = rhicfEventDst -> GetEvent();
		int runNumber = rhicfEvent -> GetRunNumber();
		int bunchNumber = rhicfEvent -> GetBunchNumber();
		int fillNumber = rhicfEvent -> GetFillNumber();

		int spinBit = rhicfEvent -> GetSpinBit();
		bool blueSpinUp = false;
		bool yellowSpinUp = false;
		if(spinBit == 5 || spinBit == 6){blueSpinUp = true;}
		if(spinBit == 5 || spinBit == 9){yellowSpinUp = true;}

		int btofMult = rhicfEvent -> GetBTofMult();
		int primaryTrkNum = rhicfEvent -> GetPrimaryTrkNum();
		double vertexZ = rhicfEvent -> GetPrimaryVtxZ();

		cout << "event data ----- runNum: " << runNumber << " btofMult: " << btofMult << endl; 

		// RHICf particle data
		int parNum = rhicfEventDst -> GetRHICfParticleNum();
		for(int i=0; i<parNum; i++){
			rhicfParticle = rhicfEventDst -> GetRHICfParticle(i);
			int pid = rhicfParticle -> GetPID();
			double e = rhicfParticle -> GetEnergy();

			cout << "rhicf particle  data ----- pid: " << pid << " e: " << e << endl; 
		}

		// RHICf detector point data
		int pointNum = rhicfEventDst -> GetRHICfDetPointNum();
		for(int i=0; i<pointNum; i++){
			rhicfDetPoint = rhicfEventDst -> GetRHICfDetPoint(i);
			int towerIdx = rhicfDetPoint -> GetTowerIdx();
			double ePoint = rhicfDetPoint -> GetPointEnergy(0); // [0=photon, 1=hadron]

			cout << "rhicf detpoint  data ----- towerIdx: " << towerIdx << " e: " << ePoint << endl; 
		}

		// TPCTrack data
		int trkNum = rhicfEventDst -> GetTPCTrackNum();
		cout << "trkNum " << trkNum << endl;
		for(int i=0; i<trkNum; i++){
			rhicfTPCTrack = rhicfEventDst -> GetTPCTrack(i);

			int pid = rhicfTPCTrack -> GetPID();
			double pt = rhicfTPCTrack -> GetPt();
			cout << "track data ----- pid: " << pid << " pt: " << pt << endl; 
		}

		// B-TOF data
		int btofNum = rhicfEventDst -> GetBTofNum();
		for(int i=0; i<btofNum; i++){
			rhicfBTof = rhicfEventDst -> GetBTof(i);
			bool isVPD = rhicfBTof -> IsVPD(); // is VPD cell
    		int trayId = rhicfBTof -> GetTray(); 

			cout << "btof data ----- isVPD: " << isVPD << " trayId: " << trayId << endl; 
		}

		// BBC data
		rhicfBBC = rhicfEventDst -> GetBBC();
		if(rhicfBBC){
			int smallEastSumADC = rhicfBBC -> GetBBCSmallSumADC(0); // [0=East, 1=West]
			int largeWestSumADC = rhicfBBC -> GetBBCSmallSumADC(1); // [0=East, 1=West]
			cout << "bbc data ----- smallEastSumADC: " << smallEastSumADC << " largeWestSumADC: " << largeWestSumADC << endl; 
		}

		// VPD data
		rhicfVPD = rhicfEventDst -> GetVPD();
		if(rhicfVPD){
			int eastSumADC = rhicfVPD -> GetVPDSumADC(0); // [0=East, 1=West]
			int westSumADC = rhicfVPD -> GetVPDSumADC(1); // [0=East, 1=West]
			cout << "vpd data ----- eastSumADC: " << eastSumADC << " westSumADC: " << westSumADC << endl; 
		}
		
		// ZDC data
		rhicfZDC = rhicfEventDst -> GetZDC();
		if(rhicfZDC){
			int eastPMTSumADC = rhicfZDC -> GetZDCPmtSumADC(0); // [0=East, 1=West]
			int westPMTSumADC = rhicfZDC -> GetZDCPmtSumADC(1); // [0=East, 1=West]
			cout << "zdc data ----- eastPMTSumADC: " << eastPMTSumADC << " westPMTSumADC: " << westPMTSumADC << endl; 
		}

		// FMS data
		int fmsNum = rhicfEventDst -> GetFMSNum();
		for(int i=0; i<fmsNum; i++){
			rhicfFMS = rhicfEventDst -> GetFMS(i);
			int category = rhicfFMS -> GetCategory();
			double efms  = rhicfFMS -> GetEnergy();
			cout << "fms data ----- category: " << category << " e: " << efms << endl; 
		}

		// RPS data
		int rpsNum = rhicfEventDst -> GetRPSNum();
		for(int i=0; i<rpsNum; i++){
			rhicfRPS = rhicfEventDst -> GetRPS(i);
			int trkUsedPlane = rhicfRPS -> GetUsedPlane();
			double pz = rhicfRPS -> GetPz();
			cout << "rps data ----- trkUsedPlane: " << trkUsedPlane << " pz: " << pz << endl; 
		}
    }

    delete eventMaker;
}

void loadLibs()
{
	TString localPath = "/star/u/slee5/.sl73_gcc485/lib/";
	gSystem->Load("libTable");
	gSystem->Load("libPhysics");
	gSystem->Load("St_base");
	gSystem->Load("StChain");
	gSystem->Load("St_Tables");
	gSystem->Load("StUtilities");        // new addition 22jul99
	gSystem->Load("StTreeMaker");
	gSystem->Load("StIOMaker");
	gSystem->Load("StarClassLibrary");
	gSystem->Load("StTriggerDataMaker"); // new starting from April 2003
	gSystem->Load("StBichsel");
	gSystem->Load("StEvent");
	gSystem->Load("StEventUtilities");
	gSystem->Load("StDbLib");
	gSystem->Load("StEmcUtil");
	gSystem->Load("StTofUtil");
	gSystem->Load("StPmdUtil");
	gSystem->Load("StRHICfUtil");
    gSystem->Load("StStrangeMuDstMaker");
	gSystem->Load("StMuDSTMaker");
    gSystem->Load(localPath+"StRHICfEventDst");
    gSystem->Load(localPath+"StRHICfEventMaker");

	gSystem->ListLibraries();
}
