
#ifndef StRHICfSimConvertor_H
#define StRHICfSimConvertor_H

#include <map>

#include "TString.h"
#include "TChain.h"
#include "TFile.h"
#include "TTree.h"
#include "TObjArray.h"
#include "TClonesArray.h"
#include "TH2Poly.h"
#include "TObjString.h"
#include "TDatabasePDG.h"

#include "StMaker.h"
#include "StRHICfPointMaker/StRHICfPointMaker.h"

class StEvent;
class StMuEvent;
class StMuDst;
class StMuDstMaker;
class StMuMcTrack;
class StMuMcVertex;
class StMuBTofHit;

class StarGenPPEvent;
class StarGenEvent;
class StarGenParticle;

class StRHICfSimDst;
class StRHICfSimEvent;
class StRHICfSimTrack;
class StRHICfSimBBC;
class StRHICfSimBTof;
class StRHICfSimRHICfHit;
class StRHICfSimRHICfPoint;
class StRHICfSimZDC;

class StRHICfCollection;
class StMuRHICfCollection;
class StRHICfHit;
class StRHICfPoint;
class StMuRHICfHit;
class StMuRHICfPoint;

using namespace std;

class StRHICfSimConvertor : public StMaker
{
	enum kConvertFlag{
		kMuDst2SimDst = 0,
		kSimRecoMode = 1
	};

	public: 
		StRHICfSimConvertor(int convertFlag, const char* fileName, const Char_t* name="RHICfSimConvertor");
		~StRHICfSimConvertor();

		void SetRHICfRunType(int type){mRHICfRunType = type;}

		Int_t Init();
		Int_t Make();
		Int_t Finish();
		Int_t clear();

	private:
		Int_t InitMuDst2SimDst();
		Int_t ConvertMuDst2SimDst();
		Int_t GetGeneratorData();

		bool IsSimPropagate(StRHICfSimTrack* simTrk);
		Int_t GetGePid2PDG(int gepid);

		void InitRHICfGeometry();
		Int_t GetRHICfGeoHit(double posX, double posY, double posZ, double px, double py, double pz, double e);

		Int_t InitSimRecoMode();
		Int_t RecoSimulation();
		Int_t FillMCData();
		Int_t SaveRecoData();

		Int_t mConvertFlag;
		TString mInputFile;
		TString mOutputFile;
		Int_t mRHICfRunType;
		Int_t mGeneratorIdx;

		TH2Poly* mRHICfPoly;
		TDatabasePDG* mDatabasePDG;

		// SimDst Coll
		TFile* mSimDstFile;
		TTree* mSimDstTree;
		StRHICfSimDst* mSimDst;
		StRHICfSimEvent* mSimEvent;
		StRHICfSimTrack* mSimTrk;
		StRHICfSimBBC* mSimBBC;
		StRHICfSimBTof* mSimBTof;
		StRHICfSimRHICfHit* mSimRHICfHit;
		StRHICfSimRHICfPoint* mSimRHICfPoint;
		StRHICfSimZDC* mSimZDC;

		// MuDst Coll
		StMuDst* mMuDst;
		StMuEvent* mMuEvent;
		StEvent* mStEvent;

		TClonesArray* mMcVtxArray;
		TClonesArray* mMcTrkArray;
		StMuMcVertex* mMcVtx;
		StMuMcTrack* mMcTrk;
		StMuBTofHit* mMuBTofHit;

		// Generator-Level Coll
		TChain* mChain;
		UInt_t mTmpChainEvent;
		StarGenPPEvent* mGenPPEvent;
		StarGenEvent* mGenEvent;
		StarGenParticle* mGenParticle;

		vector<int> mRHICfGammaIdx;
		vector<int> mRHICfNeuIdx;

		// Reconstruction Coll
		Long64_t mEvent;
		TFile* mOutSimDstFile;
		TTree* mOutSimDstTree;
		StRHICfPointMaker* mRHICfPointMaker;
		StRHICfCollection* mRHICfColl;
		StRHICfHit* mRHICfHit;
		StRHICfPoint* mRHICfPoint;


	ClassDef(StRHICfSimConvertor,0);
};

#endif
