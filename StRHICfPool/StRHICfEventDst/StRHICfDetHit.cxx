#include "StRHICfDetHit.h"

ClassImp(StRHICfDetHit)

StRHICfDetHit::StRHICfDetHit()
{
    Clear();
}

StRHICfDetHit::~StRHICfDetHit()
{
}

void StRHICfDetHit::Clear(Option_t *option)
{
    memset(mL20, 0., sizeof(mL20));
    memset(mL90, 0., sizeof(mL90));
    memset(mPlateEnergy, 0., sizeof(mPlateEnergy));

    fill_n(&mGSObarMaxLayer[0][0], kTowerNum*2, -1);
    fill_n(&mResultHitNum[0], kTowerNum, -1);
    fill_n(&mEvalHitNum[0][0][0], kTowerNum*kLayerNum*kXYNum, -1);

    memset(mSingleHit, 0., sizeof(mSingleHit));
    memset(mMultiHit, 0., sizeof(mMultiHit));
}

void StRHICfDetHit::SetL20(Int_t tower, Float_t val){mL20[tower] = val;}
void StRHICfDetHit::SetL90(Int_t tower, Float_t val){mL90[tower] = val;}
void StRHICfDetHit::SetPlateEnergy(Int_t tower, Int_t plate, Float_t val){mPlateEnergy[tower][plate] = val;}

void StRHICfDetHit::SetGSOBarMaxLayer(Int_t tower, Int_t order, Int_t val){mGSObarMaxLayer[tower][order] = val;}

void StRHICfDetHit::SetResultHitNum(Int_t tower, Int_t num){mResultHitNum[tower] = num;}
void StRHICfDetHit::SetEvalHitNum(Int_t tower, Int_t layer, Int_t xy, Int_t num){mEvalHitNum[tower][layer][xy] = num;}
void StRHICfDetHit::SetSingleHit(Int_t tower, Int_t layer, Int_t xy, Float_t pos, Float_t height)
{
    mSingleHit[tower][layer][xy][0] = pos;
    mSingleHit[tower][layer][xy][1] = height;
}
void StRHICfDetHit::SetMultiHit(Int_t tower, Int_t layer, Int_t xy, Int_t order, Float_t pos, Float_t height)
{
    mMultiHit[tower][layer][xy][order][0] = pos;
    mMultiHit[tower][layer][xy][order][1] = height;
}

Float_t StRHICfDetHit::GetL20(Int_t tower){return mL20[tower];}
Float_t StRHICfDetHit::GetL90(Int_t tower){return mL90[tower];}
Float_t StRHICfDetHit::GetPlateEnergy(Int_t tower, Int_t plate){return mPlateEnergy[tower][plate];}

Float_t StRHICfDetHit::GetTowerSumEnergy(Int_t tower, Int_t order)
{
    double sumEnergy = 0.;
    if(order==0){
        for(int ip=0; ip<kPlateNum; ip++){
            if(ip<11){sumEnergy += mPlateEnergy[tower][ip];}
            else{sumEnergy += mPlateEnergy[tower][ip]*2.;}
        }
        return sumEnergy;
    }
    else{
        for(int ip=1; ip<=11; ip++){
            if(ip<11){sumEnergy += mPlateEnergy[tower][ip];}
            else{sumEnergy += mPlateEnergy[tower][ip]*2.;}
        }
    }
    return -1;
}

Float_t StRHICfDetHit::GetPlateSumEnergy(Int_t tower)
{
    double sumEnergy = 0.;
    for(int ip=0; ip<kPlateNum; ip++){
        sumEnergy += mPlateEnergy[tower][ip];
    }
    return sumEnergy;
}

Int_t StRHICfDetHit::GetResultHitNum(Int_t tower){return mResultHitNum[tower];}
Int_t StRHICfDetHit::GetEvalHitNum(Int_t tower, Int_t layer, Int_t xy){return mEvalHitNum[tower][layer][xy];}

Int_t StRHICfDetHit::GetGSOBarMaxLayer(Int_t tower, Int_t order){return mGSObarMaxLayer[tower][order];}

Float_t StRHICfDetHit::GetSingleHitPos(Int_t tower, Int_t layer, Int_t xy){return mSingleHit[tower][layer][xy][0];}
Float_t StRHICfDetHit::GetSingleHitHeight(Int_t tower, Int_t layer, Int_t xy){return mSingleHit[tower][layer][xy][1];}
Float_t StRHICfDetHit::GetMultiHitPos(Int_t tower, Int_t layer, Int_t xy, Int_t order){return mMultiHit[tower][layer][xy][order][0];}
Float_t StRHICfDetHit::GetMultiHitHeight(Int_t tower, Int_t layer, Int_t xy, Int_t order){return mMultiHit[tower][layer][xy][order][1];}
