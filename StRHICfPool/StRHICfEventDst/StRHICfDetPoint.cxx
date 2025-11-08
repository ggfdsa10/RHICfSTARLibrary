#include "StRHICfDetPoint.h"

ClassImp(StRHICfDetPoint)

StRHICfDetPoint::StRHICfDetPoint()
{
  clear();
}

StRHICfDetPoint::~StRHICfDetPoint()
{
}

void StRHICfDetPoint::clear()
{
  mTowerIdx = -999;
  mParticleID = -999;

  memset(mPointPos, 0, sizeof(mPointPos));
  memset(mPointEnergy, 0, sizeof(mPointEnergy));
}

void StRHICfDetPoint::SetTowerIdx(Int_t val){mTowerIdx = val;}
void StRHICfDetPoint::SetPID(Int_t pid){mParticleID = pid;}
void StRHICfDetPoint::SetPointPos(Float_t x, Float_t y)
{
  mPointPos[0] = x;
  mPointPos[1] = y;
}
void StRHICfDetPoint::SetPointEnergy(Float_t pid1, Float_t pid2)
{
  mPointEnergy[0] = pid1;
  mPointEnergy[1] = pid2;
}

Int_t StRHICfDetPoint::GetTowerIdx(){return mTowerIdx;}
Int_t StRHICfDetPoint::GetPID(){return mParticleID;}
Float_t StRHICfDetPoint::GetPointPos(Int_t xy){return mPointPos[xy];}
Float_t StRHICfDetPoint::GetPointEnergy(Int_t particle){return mPointEnergy[particle];}
