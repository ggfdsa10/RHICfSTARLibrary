#include "StRHICfEvent.h"

ClassImp(StRHICfEvent)

StRHICfEvent::StRHICfEvent()
{
    Clear();
}

StRHICfEvent::~StRHICfEvent()
{
}

void StRHICfEvent::Clear(Option_t *option)
{
    mRunNumber = 0;
    mEventNumber = 0;
    mFillNumber = 0;
    mBunchNumber = 0;
    mSpinBit = 0;
    mBeamEnergy = 0.;

    mRHICfTrig = false;
    mDiffractiveTrig = false;
    mVPDMB30Trig = false;
    mTPCTrig = false;

    mRHICfRunNumber = 0;
    mRHICfEventNumber = 0;
    mRHICfTriggerNumber = 0;
    mRHICfRunType = -1;

    mRHICfL20[0] = 0.;
    mRHICfL20[1] = 0.;
    mRHICfL90[0] = 0.;
    mRHICfL90[1] = 0.;

    mTofMult = -1;
    mBTofMult = -1;
    mRefMult = -1;
    mGRefMult = -1;

    mPrimaryTrkNum = -1;
    mGlobalTrkNum = -1;

    mPrimaryVtx[0] = -999.;
    mPrimaryVtx[1] = -999.;
    mPrimaryVtx[2] = -999.;
}

void StRHICfEvent::SetRunNumber(unsigned int num){mRunNumber = num;}
void StRHICfEvent::SetEventNumber(unsigned int num){mEventNumber = num;}
void StRHICfEvent::SetFillNumber(int num){mFillNumber = num;}
void StRHICfEvent::SetBunchNumber(int num){mBunchNumber = num;}
void StRHICfEvent::SetSpinBit(int num){mSpinBit = num;}
void StRHICfEvent::SetBeamEnergy(float energy){mBeamEnergy = energy;}

void StRHICfEvent::SetRHICfTrig(bool trig){mRHICfTrig = trig;}
void StRHICfEvent::SetDiffractiveTrig(bool trig){mDiffractiveTrig = trig;}
void StRHICfEvent::SetVPDMB30Trig(bool trig){mVPDMB30Trig = trig;}
void StRHICfEvent::SetTPCTrig(bool trig){mTPCTrig = trig;}

void StRHICfEvent::SetRHICfRunNumber(unsigned int num){mRHICfRunNumber = num;}
void StRHICfEvent::SetRHICfEventNumber(unsigned int num){mRHICfEventNumber = num;}
void StRHICfEvent::SetRHICfTriggerNumber(unsigned int num){mRHICfTriggerNumber = num;}
void StRHICfEvent::SetRHICfRunType(int num){mRHICfRunType = num;}
void StRHICfEvent::SetRHICfL20(int tower, double val){mRHICfL20[tower] = val;}
void StRHICfEvent::SetRHICfL90(int tower, double val){mRHICfL90[tower] = val;}

void StRHICfEvent::SetTofMult(int num){mTofMult = num;}
void StRHICfEvent::SetBTofMult(int num){mBTofMult = num;}
void StRHICfEvent::SetRefMult(int num){mRefMult = num;}
void StRHICfEvent::SetGRefMult(int num){mGRefMult = num;}

void StRHICfEvent::SetPrimaryTrkNum(int num){mPrimaryTrkNum = num;}
void StRHICfEvent::SetGlobalTrkNum(int num){mGlobalTrkNum = num;}

void StRHICfEvent::SetPrimaryVertex(double vx, double vy, double vz)
{
    mPrimaryVtx[0] = vx;
    mPrimaryVtx[1] = vy;
    mPrimaryVtx[2] = vz;
}

UInt_t StRHICfEvent::GetRunNumber(){return mRunNumber;}
UInt_t StRHICfEvent::GetEventNumber(){return mEventNumber;}
Int_t StRHICfEvent::GetFillNumber(){return mFillNumber;}
Int_t StRHICfEvent::GetBunchNumber(){return mBunchNumber;}
Int_t StRHICfEvent::GetSpinBit(){return mSpinBit;}
Float_t StRHICfEvent::GetBeamEnergy(){return mBeamEnergy;}

Bool_t StRHICfEvent::GetRHICfTrig(){return mRHICfTrig;}
Bool_t StRHICfEvent::GetDiffractiveTrig(){return mDiffractiveTrig;}
Bool_t StRHICfEvent::GetVPDMB30Trig(){return mVPDMB30Trig;}
Bool_t StRHICfEvent::GetTPCTrig(){return mTPCTrig;}

UInt_t StRHICfEvent::GetRHICfRunNumber(){return mRHICfRunNumber;}
UInt_t StRHICfEvent::GetRHICfEventNumber(){return mRHICfEventNumber;}
UInt_t StRHICfEvent::GetRHICfTriggerNumber(){return mRHICfTriggerNumber;}
Int_t StRHICfEvent::GetRHICfRunType(){return mRHICfRunType;}
Bool_t StRHICfEvent::GetRHICfShowerTrig(){return (mRHICfTriggerNumber & 0x010)? true : false;}
Bool_t StRHICfEvent::GetRHICfPi0Trig(){return (mRHICfTriggerNumber & 0x080)? true : false;}
Bool_t StRHICfEvent::GetRHICfHighEMTrig(){return (mRHICfTriggerNumber & 0x200)? true : false;}
Double_t StRHICfEvent::GetRHICfL20(int tower){return mRHICfL20[tower];}
Double_t StRHICfEvent::GetRHICfL90(int tower){return mRHICfL90[tower];}

Int_t StRHICfEvent::GetTofMult(){return mTofMult;}
Int_t StRHICfEvent::GetBTofMult(){return mBTofMult;}
Int_t StRHICfEvent::GetRefMult(){return mRefMult;}
Int_t StRHICfEvent::GetGRefMult(){return mGRefMult;}

Int_t StRHICfEvent::GetPrimaryTrkNum(){return mPrimaryTrkNum;}
Int_t StRHICfEvent::GetGlobalTrkNum(){return mGlobalTrkNum;}

Double_t StRHICfEvent::GetPrimaryVtxX(){return mPrimaryVtx[0];}
Double_t StRHICfEvent::GetPrimaryVtxY(){return mPrimaryVtx[1];}
Double_t StRHICfEvent::GetPrimaryVtxZ(){return mPrimaryVtx[2];}
