#include "StRHICfFMS.h"

ClassImp(StRHICfFMS)

StRHICfFMS::StRHICfFMS()
{
    Clear();
}

StRHICfFMS::~StRHICfFMS()
{
}

void StRHICfFMS::Clear(Option_t *option)
{
    mFmsCategory = -1;
    fill_n(&mPosition[0], 3, -999.);
    fill_n(&mMomentum[0], 3, -999.);
    mEnergy = -999.;
}


void StRHICfFMS::SetCategory(int idx){mFmsCategory = idx;}

void StRHICfFMS::SetFMSHitPos(double x, double y, double z)
{
    mPosition[0] = x;
    mPosition[1] = y;
    mPosition[2] = z;
}

void StRHICfFMS::SetMomentum(double px, double py, double pz)
{
    mMomentum[0] = px;
    mMomentum[1] = py;
    mMomentum[2] = pz;
}

void StRHICfFMS::SetEnergy(double e){mEnergy = e;}

Int_t StRHICfFMS::GetCategory(){return mFmsCategory;}
Double_t StRHICfFMS::GetFMSHitPosX(){return mPosition[0];}
Double_t StRHICfFMS::GetFMSHitPosY(){return mPosition[1];}
Double_t StRHICfFMS::GetFMSHitPosZ(){return mPosition[2];}
Double_t StRHICfFMS::GetPx(){return mMomentum[0];}
Double_t StRHICfFMS::GetPy(){return mMomentum[1];}
Double_t StRHICfFMS::GetPz(){return mMomentum[2];}
Double_t StRHICfFMS::GetEnergy(){return mEnergy;}
