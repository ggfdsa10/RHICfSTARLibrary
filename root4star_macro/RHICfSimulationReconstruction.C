
#include "TSpectrum.h"
#include <iostream> // time test
#include <time.h> // time test 

using namespace std;

class StMaker;
class StBFChain;
class StMuDstMaker;

TString CheckIntputFile(TString input);
void loadLibs();

void RHICfSimulationReconstruction(TString inputFile = "/star/u/slee5/RHICfSTARSim/StarSim_Pythia8_TL_HitMult1_25092242046_.rhicfsim.RHICfSimDst.root")
{
	loadLibs();

	StChain* chain = new StChain();

	St_db_Maker* dbMk = new St_db_Maker("db", "MySQL:StarDb", "$STAR/StarDb", "StarDb");
	dbMk->SetDateTime(20221124,215905);
	// ================== makers =========================

	StRHICfDbMaker* rhicfDb = new StRHICfDbMaker();

	inputFile = CheckIntputFile(inputFile);
	StRHICfSimConvertor* simConvertor = new StRHICfSimConvertor(1, inputFile);

	// ================ makers end =======================

	chain->Init();
	int event = 0;
	while(1){
		if(chain->Make(event)>=kStEOF){break;}
		cout << "RHICfSimulationReconstruction -- RHICf Reconstruction mode, Event: " << event << " has done. " << endl;
		event++;
	}

	cout << "****************************************** " << endl;
	cout << "Work done... now its time to close up shop!" << endl;
	cout << "****************************************** " << endl;
	chain->Finish();
	cout << "****************************************** " << endl;
	cout << "total number of events  " << event << endl;
	cout << "****************************************** " << endl;

	delete chain;
}


TString CheckIntputFile(TString input)
{
    if(input.Index(".list") != -1 || input.Index(".lis") != -1 || input.Index(".txt") != -1 || input.Index(".rhicfsim.RHICfSimDst.root") != -1){
        return input; // input exist for MuDst
    }
    else if(input.Index(".") == -1){
        // Find a MuDst in current directory
        TString currentPath = gSystem -> pwd();
        TSystemDirectory dir("dir", currentPath);

        TList *listOfDirs = dir.GetListOfFiles();
        TObject *objDir;
        TIter next(listOfDirs);
        while((objDir = next())){
            TSystemFile* dirPtr = dynamic_cast<TSystemFile*>(objDir);
            if(dirPtr && !dirPtr->IsDirectory()){
                TString dirName = dirPtr->GetName();

                if(dirName.Index(".rhicfsim.RHICfSimDst.root") != -1 && dirName.Index(input) != -1){
                    return dirName;
                }
            }
        }
        return "";
    }
    return input;
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
	gSystem->Load(localPath+"StRHICfPointMaker.so");

	gSystem->Load( "libVMC.so");
    gSystem->Load( "St_g2t.so" );
    gSystem->Load( "St_geant_Maker.so" );
    gSystem->Load( "StarGeneratorUtil.so" );
    gSystem->Load( "StarGeneratorEvent.so" );
    gSystem->Load( "StarGeneratorBase.so" );

    gSystem->Load(localPath+"StRHICfSimDst.so" );
	gSystem->Load(localPath+"StRHICfSimConvertor.so");

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