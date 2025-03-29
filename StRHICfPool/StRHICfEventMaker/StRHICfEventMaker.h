// ============================================================ 
// | The StRHICfEventMaker class                                | 
// | for only RHICf asymmetries of the nuetral Pion analysis. | 
// |                                                          | 
// | Author: Seunghwan Lee.                                   | 
// ============================================================ 

#ifndef StRHICfEventMaker_HH
#define StRHICfEventMaker_HH


// final test !!!
// #include <iostream>
#include <fstream>
#include <string>
#include "TRandom3.h"


#include <iostream>
#include <TROOT.h>
#include <TString.h>
#include "TChain.h"
#include "TFile.h"
#include "TTree.h"
#include "TObjArray.h"
#include "TClonesArray.h"
#include "TMath.h"
#include "TVector3.h"
#include "TRandom3.h"
#include "TH1D.h"

#include "StMaker.h"

#include "StRHICfPool/StRHICfEventDst/StRHICfEventDst.h"
#include "StRHICfPool/StRHICfEventDst/StRHICfParameters.h"
#include "StRHICfPool/StRHICfEventDst/StRHICfEvent.h"
#include "StRHICfPool/StRHICfEventDst/StRHICfTPCTrack.h"
#include "StRHICfPool/StRHICfEventDst/StRHICfBTof.h"
#include "StRHICfPool/StRHICfEventDst/StRHICfBBC.h"
#include "StRHICfPool/StRHICfEventDst/StRHICfVPD.h"
#include "StRHICfPool/StRHICfEventDst/StRHICfZDC.h"
#include "StRHICfPool/StRHICfEventDst/StRHICfParticle.h"
#include "StRHICfPool/StRHICfEventDst/StRHICfDetPoint.h"
#include "StRHICfPool/StRHICfEventDst/StRHICfFMS.h"
#include "StRHICfPool/StRHICfEventDst/StRHICfRPS.h"

#include "StRHICfUtil/StRHICfFunction.h"

class StEvent;
class StMuEvent;
class StMuDst;
class StMuDstMaker;

class StMuRHICfCollection;
class StMuRHICfRawHit;
class StMuRHICfHit;
class StMuRHICfPoint;

class StMuFmsCollection;
class StMuRpsCollection;

using namespace std;
using namespace TMath;

class StRHICfEventMaker : public StMaker, StRHICfFunction
{
	public: 
		StRHICfEventMaker(int ioMode, const char* fileName, StMuDstMaker* muDstMaker = 0, const char* name="RHICfEvent");
		~StRHICfEventMaker();

		Int_t Init();
		Int_t Make();
		Int_t Finish();
		Int_t clear();

		void OnSaveAllEvents(){mOnAllEventSave = true;} // RHICfEventMaker is principally saved only has RHICf det signal exist. if you On it, RHICfEventDst can save all of triggered events

		// data attractive on-off
		void SetOffTPCTrack(){mOffTPCTrack = true;} // option to not saved TPCTrack 
		void SetOffBTof(){mOffBTof = true;} // option to not saved BBC data 
		void SetOffBBC(){mOffBBC = true;} // option to not saved BBC data 
		void SetOffVPD(){mOffVPD = true;} // option to not saved VPD data 
		void SetOffZDC(){mOffZDC = true;} // option to not saved ZDC data 
		void SetOffFMS(){mOffFMS = true;} // option to not saved FMS data 
		void SetOffRPS(){mOffRPS = true;} // option to not saved RPS data 

		// Reading RHICfEventDst
		Int_t GetEventNum();
		StRHICfEventDst* GetEventDst(int event);

	private:
		// general 
		Int_t openRead();
		Int_t openWrite();

		// Main data saving function
		void fillData();
		Int_t eventData();
		Int_t rhicfData();
		Int_t vertex();
		Int_t trackData();
		Int_t btofData();
		Int_t bbcData();
		Int_t vpdData();
		Int_t zdcData();
		Int_t fmsData();
		Int_t rpsData();

		// Utilized functions
		Bool_t eventCut(StMuRHICfPoint* point);
		void convertIPSystem(bool metOpt, bool referOpt=0);
		void kinematics();
		void spinPatternData();
		Double_t beamCenterPosition(int xy, bool metOpt, bool referOpt=0); // metOpt: scan & hit mode, referOpt: refer to fill number(21148, 21150) in only TOP type.
																		   // metOpt false = scan, true = hit mode , referOpt false = 21148, true = 21450 using mode.
		Int_t eventSummary();

		// Out data structrue
		int mIoMode;
		Int_t mNFiles;
		TString mInputFileName;
		TString mOutputFileName;

		bool mOnAllEventSave;

		bool mOffTPCTrack;
		bool mOffBTof;
		bool mOffBBC;
		bool mOffVPD;
		bool mOffZDC;
		bool mOffFMS;
		bool mOffRPS;
		
		TFile* mTFile;
		TChain* mChain;
		TTree* mTree;

		// RHICfEventDst
		StRHICfEventDst* mRHICfEventDst;
		StRHICfEvent* mRHICfEvent;
		StRHICfTPCTrack* mRHICfTPCTrack;
		StRHICfBTof* mRHICfBTof;
		StRHICfBBC* mRHICfBBC;
		StRHICfVPD* mRHICfVPD;
		StRHICfZDC* mRHICfZDC;
		StRHICfParticle* mRHICfParticle;
		StRHICfDetPoint* mRHICfDetPoint;
		StRHICfFMS* mRHICfFMS;
		StRHICfRPS* mRHICfRPS;

		// Event Coll.
		StMuDstMaker* muDstMaker;
		StMuDst* muDst;
		StMuEvent* muEvent;
		StEvent* dsEvent;
		Long64_t iEvent;

		// RHICf Coll.
		StMuRHICfCollection* muRHICfColl;
		StMuRHICfRawHit* muRHICfRawHit;
		StMuRHICfHit* muRHICfHit;

		// Track Coll.
		TObjArray* mMuGTrks; // global tracks
		TObjArray* mMuTrks; // primary tracks
		TVector3 mVertexVec;
		Float_t mTpcVpdVzDiffCut;
		
		// FMS Coll.
		StMuFmsCollection* muFmsColl;
		// Roman Pots Coll.
		StMuRpsCollection* mMuRpsColl;

		// const double distZatIP = 17812.0; // [mm] Distance of the beam from interaction point to RHICf detector. (exactly one)
		const double distZatIP = 18000.0; // [mm] Distance of the beam from interaction point to RHICf detector.
		const double geoCenterTL = 20.0; // [mm] geometrical large tower center
		const double geoCenterTS = 10.0; // [mm] geometrical small tower center
		const double distTStoTL = 47.4; // [mm] Distance between center of TL and cernter of TS

		int fillNumArray[5];
		int spinPatterns[5][120][2]; // [Fill number][bunch number][blue, yellow] for Fill no. 21142, 21145, 21148, 21149, 21150

		// final crosscheck test !!!
		ofstream txtFile;

	protected:
		enum ioMode {ioRead, ioWrite};


	ClassDef(StRHICfEventMaker,0);
};

#endif
