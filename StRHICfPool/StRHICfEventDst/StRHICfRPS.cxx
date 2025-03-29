#include "StRHICfRPS.h"

ClassImp(StRHICfRPS)

StRHICfRPS::StRHICfRPS()
{
    Clear();
}

StRHICfRPS::~StRHICfRPS()
{
}

void StRHICfRPS::Clear(Option_t *option)
{
    mRpsUsedPlane = -1;
    mRpsTrkType = -1;
    mRpsTrkBranch = -1;
    fill_n(&mPosition[0][0], kRPSPointNum*3, -999.);
    fill_n(&mMomentum[0], 3, -999.);
}

void StRHICfRPS::SetUsedPlane(int plane){mRpsUsedPlane = plane;}
void StRHICfRPS::SetTrkType(int type){mRpsTrkType = type;}
void StRHICfRPS::SetTrkBranch(int idx){mRpsTrkBranch = idx;}

void StRHICfRPS::SetRPSHitPos(int point, double x, double y, double z)
{
    mPosition[point][0] = x;
    mPosition[point][1] = y;
    mPosition[point][2] = z;
}

void StRHICfRPS::SetMomentum(double px, double py, double pz)
{
    mMomentum[0] = px;
    mMomentum[1] = py;
    mMomentum[2] = pz;
}

Int_t StRHICfRPS::GetUsedPlane(){return mRpsUsedPlane;}
Int_t StRHICfRPS::GetTrkType(){return mRpsTrkType;}
Int_t StRHICfRPS::GetTrkBranch(){return mRpsTrkBranch;}
Double_t StRHICfRPS::GetRPSHitPosX(int point){return mPosition[point][0];}
Double_t StRHICfRPS::GetRPSHitPosY(int point){return mPosition[point][1];}
Double_t StRHICfRPS::GetRPSHitPosZ(int point){return mPosition[point][2];}
Double_t StRHICfRPS::GetPx(){return mMomentum[0];}
Double_t StRHICfRPS::GetPy(){return mMomentum[1];}
Double_t StRHICfRPS::GetPz(){return mMomentum[2];}
