#include "StRHICfParticle.h"

ClassImp(StRHICfParticle)

StRHICfParticle::StRHICfParticle()
{
    Clear();
}

StRHICfParticle::~StRHICfParticle()
{
}

void StRHICfParticle::Clear(Option_t *option)
{
    mPID = -1;
    mPi0Type = -1;
    mTowerIdx = -1;
    fill_n(&mPosition[0], 3, -999.);
    fill_n(&mMomentum[0], 3, -999.);
    mEnergy = -999.;
    mMass = -999.;
}

void StRHICfParticle::SetPID(int pid){mPID = pid;}
void StRHICfParticle::SetPi0Type(int type){mPi0Type = type;}
void StRHICfParticle::SetTowerIdx(int idx){mTowerIdx = idx;}

void StRHICfParticle::SetRHICfHitPos(double x, double y, double z)
{
    mPosition[0] = x;
    mPosition[1] = y;
    mPosition[2] = z;
}

void StRHICfParticle::SetMomentum(double px, double py, double pz)
{
    mMomentum[0] = px;
    mMomentum[1] = py;
    mMomentum[2] = pz;
}

void StRHICfParticle::SetEnergy(double e){mEnergy = e;}
void StRHICfParticle::SetMass(double mass){mMass = mass;}

Int_t StRHICfParticle::GetPID(){return mPID;}
Int_t StRHICfParticle::GetPi0Type(){return mPi0Type;}
Int_t StRHICfParticle::GetTowerIdx(){return mTowerIdx;}
Double_t StRHICfParticle::GetRHICfHitPosX(){return mPosition[0];}
Double_t StRHICfParticle::GetRHICfHitPosY(){return mPosition[1];}
Double_t StRHICfParticle::GetRHICfHitPosZ(){return mPosition[2];}
Double_t StRHICfParticle::GetPx(){return mMomentum[0];}
Double_t StRHICfParticle::GetPy(){return mMomentum[1];}
Double_t StRHICfParticle::GetPz(){return mMomentum[2];}
Double_t StRHICfParticle::GetEnergy(){return mEnergy;}
Double_t StRHICfParticle::GetMass(){return mMass;}
Double_t StRHICfParticle::GetPt(){return sqrt(mMomentum[0]*mMomentum[0] + mMomentum[1]*mMomentum[1]);}
Double_t StRHICfParticle::GetXf(){return mMomentum[2]/255.;}

