#ifndef __StRHICfSimGenerator_h__
#define __StRHICfSimGenerator_h__

#include <map>

#include "TFile.h"
#include "TTree.h"
#include "TString.h"
#include "TSystem.h"
#include "TParticle.h"

#include "StarGenerator/BASE/StarGenerator.h"
#include "StRHICfPool/StRHICfSimDst/StRHICfSimPar.h"

class StarGenEvent;
class StarGenPPEvent;

class StRHICfSimGenerator : public StarGenerator
{
    public:
        StRHICfSimGenerator( const char *name="StRHICfSimGenerator" );
        ~StRHICfSimGenerator(){};

        int Init();
        int Generate();

        void SetGenFile(TString file);
        void SetGeneratorModel(int generatorId);
        void SetRHICfRunType(int runtype);

        int InitTree();

        void SetTotalEventNumber(int num);
        int GetTotalEventNumber();

        /// Return end-of-run statistics
        StarGenStats Stats();

    protected:
        void FillPP( StarGenEvent *event );

        Int_t mGeneratorId;
        Int_t mRHICfRunType;

        TString mFileName;
        TFile* mGenFile;
        TTree* mGenTree;
        Int_t mEventIdx;
        Int_t mSetEventNumber;
        Int_t mTotalEventNumber;

        Int_t mEventProcess;
        Double_t mEvent_sHat;
        Double_t mEvent_tHat;
        Double_t mEvent_uHat;
        TClonesArray* mParticleArr;
        TParticle* mParticle;

        ClassDef(StRHICfSimGenerator,0);
};

#endif