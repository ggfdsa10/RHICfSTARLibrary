
#include "TSpectrum.h"
#include <iostream> // time test
#include <time.h> // time test 

using namespace std;

class StMaker;
class StBFChain;
class StMuDstMaker;

void loadLibs();

// cross checking run list
// TL, 21142, 2631 18175022 /star/data112/reco/pp500_production_rhicf_2017/ReversedFullField/P24ib/2017/175/18175022/st_rhicf_18175022_raw_3000009.MuDst.root
// TL, 21145, 2718 18176011 /star/data112/reco/pp500_production_rhicf_2017/ReversedFullField/P24ib/2017/176/18176011/st_rhicf_18176011_raw_1000009.MuDst.root
// TL, 21145, 2734 18176018 /star/data112/reco/pp500_production_rhicf_2017/ReversedFullField/P24ib/2017/176/18176018/st_rhicf_18176018_raw_3500003.MuDst.root
// TS, 21148, 2819 18177001 /star/data112/reco/pp500_production_rhicf_2017/ReversedFullField/P24ib/2017/177/18177001/st_rhicf_18177001_raw_1500015.MuDst.root
// TOP, 21149 2892 18177029, does not exist on STAR.
// TOP, 21149, 2896 18177031 /star/data112/reco/pp500_production_rhicf_2017/ReversedFullField/P24ib/2017/177/18177031/st_rhicf_18177031_raw_5000003.MuDst.root
// TOP, 21149, 2899 18177032 /star/data112/reco/pp500_production_rhicf_2017/ReversedFullField/P24ib/2017/177/18177032/st_rhicf_18177032_raw_4000011.MuDst.root
// TS, 21150, 2924 18177043 /star/data112/reco/pp500_production_rhicf_2017/ReversedFullField/P24ib/2017/177/18177043/st_rhicf_18177043_raw_2000007.MuDst.root
// TS, 21150, 2981 18178016 /star/data112/reco/pp500_production_rhicf_2017/ReversedFullField/P24ib/2017/178/18178016/st_rhicf_18178016_raw_2000010.MuDst.root

void makeRHICfEvent(const Char_t *inputFile = "/star/data112/reco/pp500_production_rhicf_2017/ReversedFullField/P24ib/2017/178/18178016/st_rhicf_18178016_raw_2000010.MuDst.root")
{
	loadLibs();

	StChain* chain = new StChain();
	StMuDstMaker* MuDstMaker = new StMuDstMaker(0, 0, "", inputFile, "MuDst", 100);
	MuDstMaker->SetStatus("*", 1);

	St_db_Maker* dbMk = new St_db_Maker("db", "MySQL:StarDb", "$STAR/StarDb", "StarDb");
	// ================== makers =========================

	StRHICfDbMaker* rhicfDb = new StRHICfDbMaker();
	StRHICfHitMaker* rhicfHit = new StRHICfHitMaker();
	StRHICfPointMaker* rhicfPoint = new StRHICfPointMaker();
	StRHICfEventMaker* RHICfEventMaker = new StRHICfEventMaker(1, inputFile, MuDstMaker);
	RHICfEventMaker -> OnSaveAllEvents();
	RHICfEventMaker -> SetOffTPCTrack();
	RHICfEventMaker -> SetOffFMS();

	// ================ makers end =======================

	chain->Init();

	Int_t totalEvents = MuDstMaker->chain()->GetEntries();
	cout << " Total Events: " << totalEvents << endl;
	chain->EventLoop(0, totalEvents);

	cout << "****************************************** " << endl;
	cout << "Work done... now its time to close up shop!" << endl;
	cout << "****************************************** " << endl;
	chain->Finish();
	cout << "****************************************** " << endl;
	cout << "total number of events  " << totalEvents << endl;
	cout << "****************************************** " << endl;

	delete chain;
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
	gSystem->Load("StPreEclMaker");
	gSystem->Load("StStrangeMuDstMaker");
	gSystem->Load("StMuDSTMaker");

	// for RHICf
	gSystem->Load("StRHICfDbMaker");
	gSystem->Load("StRHICfRawHitMaker");

	gSystem->Load(localPath+"StRHICfHitMaker");
	gSystem->Load(localPath+"StRHICfPointMaker");
    gSystem->Load(localPath+"StRHICfEventDst");
    gSystem->Load(localPath+"StRHICfEventMaker");

	gSystem->Load("libStarAgmlUtil");
	gSystem->Load("StTpcDb");
	gSystem->Load("StMcEvent");
	gSystem->Load("StMcEventMaker");
	gSystem->Load("StDaqLib");
	gSystem->Load("libgen_Tables");
	gSystem->Load("libsim_Tables");
	gSystem->Load("libglobal_Tables");
	gSystem->Load("StEmcTriggerMaker");
	gSystem->Load("StEmcRawMaker");
	gSystem->Load("StEmcADCtoEMaker");
	gSystem->Load("StPreEclMaker");
	gSystem->Load("StEpcMaker");
	gSystem->Load("StEmcSimulatorMaker");
	gSystem->Load("StDbBroker");
	gSystem->Load("StDetectorDbMaker");
	gSystem->Load("StDbUtilities");
	gSystem->Load("StEEmcUtil");
	gSystem->Load("StEEmcDbMaker");
	gSystem->Load("St_db_Maker");
	gSystem->Load("StTriggerUtilities");
	//  MTD libraries
	gSystem->Load("StMagF");
	gSystem->Load("StMtdUtil");
	gSystem->Load("StMtdHitMaker");
	gSystem->Load("StMtdMatchMaker");
	gSystem->Load("StMtdCalibMaker");

	// BTOF libraries
	gSystem->Load("StBTofUtil");
	gSystem->Load("StVpdCalibMaker");
	gSystem->Load("StBTofCalibMaker");

	gSystem->ListLibraries();
}
