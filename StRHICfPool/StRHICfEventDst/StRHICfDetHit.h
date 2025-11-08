#ifndef StRHICfDetHit_HH
#define StRHICfDetHit_HH

#include "TMath.h"
#include "TObject.h"
#include "StRHICfParameters.h"

using namespace std;

class StRHICfDetHit : public TObject
{
    public: 
        StRHICfDetHit();
        ~StRHICfDetHit();

        void Clear(Option_t *option = "");

        void SetL20(Int_t tower, Float_t val);
        void SetL90(Int_t tower, Float_t val);
        void SetPlateEnergy(Int_t tower, Int_t plate, Float_t val);

        void SetGSOBarMaxLayer(Int_t tower, Int_t order, Int_t val);

        void SetResultHitNum(Int_t tower, Int_t num);
        void SetEvalHitNum(Int_t tower, Int_t layer, Int_t xy, Int_t num);
        void SetSingleHit(Int_t tower, Int_t layer, Int_t xy, Float_t pos, Float_t height);
        void SetMultiHit(Int_t tower, Int_t layer, Int_t xy, Int_t order, Float_t pos, Float_t height);

        Float_t GetL20(Int_t tower);
        Float_t GetL90(Int_t tower);
        Float_t GetPlateEnergy(Int_t tower, Int_t plate);
        Float_t GetTowerSumEnergy(Int_t tower, Int_t order);
        Float_t GetPlateSumEnergy(Int_t tower);

        Int_t GetGSOBarMaxLayer(Int_t tower, Int_t order=0);

        Int_t GetResultHitNum(Int_t tower);
        Int_t GetEvalHitNum(Int_t tower, Int_t layer, Int_t xy);
        Float_t GetSingleHitPos(Int_t tower, Int_t layer, Int_t xy);
        Float_t GetSingleHitHeight(Int_t tower, Int_t layer, Int_t xy);
        Float_t GetMultiHitPos(Int_t tower, Int_t layer, Int_t xy, Int_t order);
        Float_t GetMultiHitHeight(Int_t tower, Int_t layer, Int_t xy, Int_t order);

    private:
        Float_t mL20[kTowerNum];
        Float_t mL90[kTowerNum];
        Float_t mPlateEnergy[kTowerNum][kPlateNum];

        Char_t mGSObarMaxLayer[kTowerNum][2]; // [1st Max layer, 2nd Max layer]
        Char_t mResultHitNum[kTowerNum];
        Char_t mEvalHitNum[kTowerNum][kLayerNum][kXYNum];
        Float_t mSingleHit[kTowerNum][kLayerNum][kXYNum][2]; // [pos, height]
        Float_t mMultiHit[kTowerNum][kLayerNum][kXYNum][2][2]; // [multi order][pos, height]

    ClassDef(StRHICfDetHit,1)
};

#endif
