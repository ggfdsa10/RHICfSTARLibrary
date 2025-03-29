#ifndef StRHICfDetPoint_hh
#define StRHICfDetPoint_hh

#include "TObject.h"
#include "StRHICfParameters.h"

class StRHICfDetPoint : public TObject 
{
  public:
    StRHICfDetPoint();
    ~StRHICfDetPoint();

    void clear();

    void SetTowerIdx(Int_t val);
    void SetPID(Int_t pid);
    void SetPointPos(Float_t x, Float_t y);
    void SetPointEnergy(Float_t pid1, Float_t pid2);
    void SetTowerSumEnergy(Float_t all, Float_t part);

    Int_t GetTowerIdx();
    Int_t GetPID();
    Float_t GetPointPos(Int_t xy);
    Float_t GetPointEnergy(Int_t particle);
    Float_t GetTowerSumEnergy(Int_t order);

  private:
    Int_t mTowerIdx;
    Int_t mParticleID;

    Float_t mPointPos[2];
    Float_t mPointEnergy[2];
    Float_t mTowerSumEnergy[2];

  ClassDef(StRHICfDetPoint,1)
};

#endif