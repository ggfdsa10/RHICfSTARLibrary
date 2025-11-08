#ifndef StRHICfEventDst_HH
#define StRHICfEventDst_HH

#include "TChain.h"
#include "TTree.h"
#include "TObject.h"
#include "TClonesArray.h"

#include "StRHICfParameters.h"
#include "StRHICfEvent.h"
#include "StRHICfTPCTrack.h"
#include "StRHICfBTof.h"
#include "StRHICfBBC.h"
#include "StRHICfVPD.h"
#include "StRHICfZDC.h"
#include "StRHICfDetHit.h"
#include "StRHICfDetPoint.h"
#include "StRHICfFMS.h"
#include "StRHICfRPS.h"

using namespace std;

class StRHICfEventDst : public TObject
{
    public:
        StRHICfEventDst();
        virtual ~StRHICfEventDst();

        void Clear(Option_t* option = "");

        Int_t CreateDstArray(TTree* tree);
        Int_t ReadDstArray(TTree* tree);
        Int_t ReadDstArray(TChain* chain);

        void OffAllSTARDet();
        void OffTPCTrack();
        void OffBTof();
        void OffBBC();
        void OffVPD();
        void OffZDC();
        void OffFMS();
        void OffRPS();

        StRHICfEvent* GetEvent();
        StRHICfDetHit* GetRHICfDetHit();
        StRHICfDetPoint* GetRHICfDetPoint(int idx);
        StRHICfTPCTrack* GetTPCTrack(int idx);
        StRHICfBTof* GetBTof(int idx);
        StRHICfBBC* GetBBC();
        StRHICfVPD* GetVPD();
        StRHICfZDC* GetZDC();
        StRHICfFMS* GetFMS(int idx);
        StRHICfRPS* GetRPS(int idx);

        Int_t GetRHICfDetPointNum();
        Int_t GetTPCTrackNum();
        Int_t GetBTofNum();
        Int_t GetFMSNum();
        Int_t GetRPSNum();
    
    private:
        void InitClonesArray();

        TClonesArray* mEvent;
        TClonesArray* mRHICfDetHit;
        TClonesArray* mRHICfDetPoint;
        TClonesArray* mTPCTrack;
        TClonesArray* mBTof;
        TClonesArray* mBBC;
        TClonesArray* mVPD;
        TClonesArray* mZDC;
        TClonesArray* mFMS;
        TClonesArray* mRPS;

        bool mOffTPCTrack;
        bool mOffBTof;
        bool mOffBBC;
        bool mOffVPD;
        bool mOffZDC;
        bool mOffFMS;
        bool mOffRPS;

    ClassDef(StRHICfEventDst, 1)
};

#endif